#include <Arduino.h>
#include <EEPROM.h>
#include <stdbool.h>
#include <stdint.h>

#include "Bridge/bridge.h"
#include "Thermistor/thermistor.h"
#include "Thermistor/thermistor_ntc_100k_3950K.h"
#include "buttons.h"
#include "persistent_memory.h"
#include "timebase.h"

// #define F_CPU 16000000

// Permanent storage header and footer are used to make sure EEPROM was already
// used and is valid. These are default constant values which is really
// unlikely we'll find in the EEPROM straight from factory. They will be used
// to invalidate cached values and trigger board auto-learning
#define PERMANENT_STORAGE_HEADER 0xDE
#define PERMANENT_STORAGE_FOOTER 0xAD

#define AMP_GAIN 10U                        /**> Hardware 2 stage amplifier gain                                    */

#define SAMPLES_PER_SINE  20U               /**> How many samples we are using to depict a full sine wave.          */
                                            /**> Appropriate values might range from 10 to 20                       */

#define STALLED_CURRENT_MULTIPLIER 5U       /**> Used to detect overcurrent conditions.                             */
                                            /**> Inrush current is several times bigger than normal current         */

#define STEADY_MOTOR_RUNTIME 5U             /**> Minimum time to wait after motor is triggered to consider it in    */
                                            /**> it's normal opeation mode                                          */

// Pin mapping
const uint8_t motor_control_pin = 2;  // D2
const uint8_t status_led_pin = 3;     // D3
const uint8_t minus_button_pin = 4;   // D4
const uint8_t plus_button_pin = 5;    // D5

const uint8_t temp_sensor_pin = A0;
const uint8_t current_sensor_pin = A1;
const uint16_t upper_resistance = 320U;
const uint16_t vcc_mv = 5000U;

/**
 * @brief controls the application state (state machine)
 */
typedef enum
{
    APP_STATE_NORMAL,         /**> Normal fridge operation         */
    APP_STATE_POST_BOOT_WAIT, /**> Post boot time window waits for 5 seconds
                                 without triggering the compressor. This allows
                                 the user to enter the Learning mode when
                                 exiting the boot.             */
    APP_STATE_MOTOR_STALLED   /**> Motor stalled condition was detected, waiting
                                 for   5 minutes before trying again    */
} app_state_t;

void set_motor_output(const uint8_t value)
{
    digitalWrite(motor_control_pin, LOW);
    digitalWrite(status_led_pin, LOW);
}

// Default configuration initialisation
static persistent_config_t config = {.header = PERMANENT_STORAGE_HEADER,
                                     .target_temperature = 4,
                                     .current_threshold = 0,
                                     .footer = PERMANENT_STORAGE_FOOTER};

static void read_buttons_events(button_event_t *const plus_button_event,
                                button_event_t *const minus_button_event,
                                uint32_t const *const seconds);

/**
 * @brief returns a fake RMS of the last 20 samples -> 20*50Hz = 1kHz.
 * Ideally we should use between 10 to 20 samples to represent a full sine wave
 * at 50Hz, so we would need to sample at around 500Hz - 1kHz.
 */
static uint16_t read_current_fake_rms(uint16_t const *const current_ma);

void setup()
{
    pinMode(minus_button_pin, INPUT);
    pinMode(plus_button_pin, INPUT);
    pinMode(motor_control_pin, OUTPUT);
    pinMode(status_led_pin, OUTPUT);
    pinMode(temp_sensor_pin, INPUT);
    pinMode(current_sensor_pin, INPUT);

    timebase_init();

    if (persistent_mem_is_first_boot(PERMANENT_STORAGE_HEADER, PERMANENT_STORAGE_FOOTER))
    {
        // Writes the default config on first boot so that it's a known starting
        // point for subsequent eeprom references.
        persistent_mem_write_config(&config);
    }
    else
    {
        // Otherwise, read back config from EEPROM
        persistent_mem_read_config(&config);
    }

    sei();
    // Serial.begin(9600);
}

void loop()
{
    // Keeps track of the previous time the system was toggled
    static uint32_t seconds = 0;
    static app_state_t app_state = APP_STATE_POST_BOOT_WAIT;
    static button_event_t plus_button_event = BUTTON_EVENT_DEFAULT;

    // We need to detect transistion from the PRESSED event to the HOLD event
    // exactly once
    static button_event_t prev_minus_button_event = BUTTON_EVENT_DEFAULT;
    static button_event_t minus_button_event = BUTTON_EVENT_DEFAULT;

    // Used to track time when we wait for the fridge's gases pressure to
    // equalize
    static uint32_t stalled_wait_start = 0;
    static uint32_t motor_run_time = 0;

    // Raw adc readings
    uint16_t temp_reading_raw = 0;
    uint16_t current_reading_raw = 0;

    // Transformed readings
    int8_t temperature = 0;
    uint16_t current_ma = 0;

    timebase_get_time(&seconds);
    temp_reading_raw = analogRead(temp_sensor_pin);
    current_reading_raw = analogRead(current_sensor_pin);

    // Using the x10 to lower aliasing but still retain a more accurate
    // millivolt reading Also, this remains right under the overflow : (5000 x
    // 10 < UINT16_MAX)
    uint16_t temp_reading_mv = (((vcc_mv * 10U) / 1024) * temp_reading_raw) / 10U;
    uint16_t currend_reading_mv = (((vcc_mv * 10U) / 1024) * current_reading_raw) / 10U;

    uint16_t ntc_resistance = bridge_get_lower_resistance(&upper_resistance, &temp_reading_raw, &vcc_mv);

    temperature = thermistor_read_temperature(&thermistor_ntc_100k_3950K_data, &ntc_resistance);

    // Usually we have a 1V for 1A CT with burden resistor couple.
    // As we are reading millivolt -> current_reading_mv / 1000 gives us
    // current_reading_volts <=> current_reading_amps and as we'd like to read
    // milliamps we need to multiply current_reading_amps : both /1000 and *
    // 1000 cancel each other out. However, we are using a dual stage amplifier
    // to accomodate for lower voltages. So we need to take that gain into
    // account for the final calculation :
    current_ma = currend_reading_mv / AMP_GAIN;
    uint16_t current_rms = read_current_fake_rms(&current_ma);

    read_buttons_events(&plus_button_event, &minus_button_event, &seconds);

    switch (app_state)
    {
        case APP_STATE_MOTOR_STALLED:
        {
            // Wait for 5 minutes before exiting this state
            uint32_t elapsed_time = seconds - stalled_wait_start;
            if (elapsed_time >= 5U * 60U)
            {
                // Revert to normal operation mode
                app_state = APP_STATE_NORMAL;
            }
            break;
        }

            // case APP_STATE_POST_BOOT_WAIT :
            // {
            //     // Only trigger this event once, at first detection of the
            //     button HOLD event, not the subsequent ones.
            //     if((BUTTON_EVENT_HOLD == minus_button_event)
            //     && (prev_minus_button_event != minus_button_event))
            //     {
            //         // Reset memory back to default (starts a new "Learning"
            //         mode) config.current_threshold = 0;
            //         persistent_mem_write_config(&config);
            //     }
            //     break;
            // }

        case APP_STATE_NORMAL:
        default:
        {
            // Only trigger this event once, at first detection of the button
            // HOLD event, not the subsequent ones.
            if ((BUTTON_EVENT_HOLD == minus_button_event)
                && (prev_minus_button_event != minus_button_event))
            {
                // Reset memory back to default (starts a new "Learning" mode)
                config.current_threshold = 0;
                persistent_mem_write_config(&config);
            }

            // Just learnt new "normal" motor behavior !
            // Save it to persistent memory
            if ((config.current_threshold == 0)
             && (motor_run_time > STEADY_MOTOR_RUNTIME))
            {
                config.current_threshold = current_rms;
                persistent_mem_write_config(&config);
            }

            // Detected stalled motor, stop trying to trigger the compressor for now
            if ((config.current_threshold > 0)
             && (current_rms > (STALLED_CURRENT_MULTIPLIER * config.current_threshold)))
            {
                app_state = APP_STATE_MOTOR_STALLED;
                set_motor_output(LOW);
                stalled_wait_start = seconds;

                motor_run_time = 0;
                // TODO : blink the LED to indicate we've entered this mode
            }

            break;
        }
    }

    prev_minus_button_event = minus_button_event;
    // Read temp
    // Process (filter) temp reading -> hysteresis
    // Read current
    // Process current reading

    // If motor current is too high, increment motor_current_high_seconds
    // counter for it Then if that counter goes above 3 seconds, we set the
    // forced stalled flag and reset the counter back to 0
    // -> We enter a "safe" loop where compressor won't be triggered

    // if temp is not in appropriate range
    //      -> Cooling is required
    // else, do nothing

    // Then, if already cooling and motor current was too high for at least 5
    // seconds,
}

static void read_buttons_events(button_event_t *const plus_button_event,
                                button_event_t *const minus_button_event,
                                uint32_t const *const seconds)
{
    static button_local_mem_t plus_button_mem = {
        .current = LOW,
        .previous = LOW,
        .pressed = 0,
        .event = BUTTON_EVENT_DEFAULT,
    };
    static button_local_mem_t minus_button_mem = {
        .current = LOW,
        .previous = LOW,
        .pressed = 0,
        .event = BUTTON_EVENT_DEFAULT,
    };

    plus_button_mem.current = digitalRead(plus_button_pin);
    minus_button_mem.current = digitalRead(minus_button_pin);

    read_single_button_event(&plus_button_mem, seconds);
    read_single_button_event(&minus_button_mem, seconds);

    *plus_button_event = plus_button_mem.event;
    *minus_button_event = minus_button_mem.event;
}

// Note : very naive implementation
static uint16_t read_current_fake_rms(uint16_t const *const current_ma)
{
    static uint16_t data[SAMPLES_PER_SINE];
    static uint8_t index = 0;

    // Circular buffer
    data[index] = *current_ma;
    index = (index + 1) % SAMPLES_PER_SINE;

    uint16_t max = 0;
    uint16_t min = 0;

    for (uint8_t i = 0; i < SAMPLES_PER_SINE; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
        if (data[i] < min)
        {
            min = data[i];
        }
    }

    uint16_t peak_to_peak = max - min;
    uint16_t magnitude = peak_to_peak / 2U;

    // Removing alias again on sqrt(2) with small error margin
    return (magnitude * 10U) / 14U;
}
