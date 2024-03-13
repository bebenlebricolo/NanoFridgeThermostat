#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#include "Core/bridge.h"
#include "Core/buttons.h"
#include "Core/current.h"
#include "Core/mcu_time.h"
#include "Core/thermistor.h"
#include "Core/thermistor_ntc_100k_3950K.h"

#include "Core/led.h"

#include "Hal/persistent_memory.h"
#include "Hal/timebase.h"


// clang-format off
// -> Clang has troubles treating multiline macros comments
// https://github.com/llvm/llvm-project/issues/54399

// Permanent storage header and footer are used to make sure EEPROM was already
// used and is valid. These are default constant values which is really
// unlikely we'll find in the EEPROM straight from factory. They will be used
// to invalidate cached values and trigger board auto-learning
#define PERMANENT_STORAGE_HEADER 0xDE
#define PERMANENT_STORAGE_FOOTER 0xAD

#define AMP_GAIN 10U                        /**> Hardware 2 stage amplifier gain                                        */

#define SAMPLES_PER_SINE  20U               /**> How many samples we are using to depict a full sine wave.              */
                                            /**> Appropriate values might range from 10 to 20                           */

#define STALLED_CURRENT_MULTIPLIER 5U       /**> Used to detect overcurrent conditions.                                 */
                                            /**> Inrush current is several times bigger than normal current             */

#define STEADY_MOTOR_RUNTIME 5U             /**> Minimum time to wait after motor is triggered to consider it in        */
                                            /**> it's normal operation mode                                             */

#define STALLED_MOTOR_WAIT_MINUTES 5U       /**> How long we'll need to wait between motor starts when motor is stalled */

#define MAINS_AC_FREQUENCY_HZ 50U           /**> Mains outlet AC frequency (50 Hz in France)                            */

#define CURRENT_SENSOR_CHECK_RATE_HZ uint16_t(SAMPLES_PER_SINE * MAINS_AC_FREQUENCY_HZ)    /**> Current sensor check rate (frequency) - Hz               */

// Compiles down to constant anyway !
#define CURRENT_SENSOR_CHECK_PERIOD_MS uint8_t(1000 / CURRENT_SENSOR_CHECK_RATE)        /**> Current sensor check time period in milliseconds (between 2 sensor reads) */

#define TEMP_HYSTERESIS_HIGH 2U     /**> Upper limit of the hysteresis window. If temp gets higher than 2°C above the target temp, we start the compressor  */
#define TEMP_HYSTERESIS_LOW 2U      /**> Lower limit of the hysteresis window. If temp gets lower than 2°C below the target temp, we stop the compressor    */

#define DEBUG_SERIAL
#define DEBUG_TEMP 0
#define DEBUG_REPORT_PERIODIC 1
#if DEBUG_REPORT_PERIODIC == 1
    #define DEBUG_REPORT_PERIOD_SECONDS 1U
#endif
#define FORCE_OVERWRITE_EEPROM 1

#ifdef DEBUG_SERIAL
    #define MSG_LENGTH 50U
    char msg[MSG_LENGTH] = {0};
    #define LOG_INIT() Serial.begin(9600)
    #define LOG(msg) Serial.print(msg)
    #define LOG_CUSTOM(format, ...)                     \
        snprintf(msg, MSG_LENGTH, format, __VA_ARGS__); \
        LOG(msg);
#else
    #define LOG_INIT()
    #define LOG(msg)
    #define LOG_CUSTOM(format, ...)
#endif
// clang-format on

// Pin mapping
const uint8_t motor_control_pin = 2; // D2
const uint8_t status_led_pin    = 3; // D3 -> PORTD3 of the atmega328PB Mcu
const uint8_t minus_button_pin  = 4; // D4
const uint8_t plus_button_pin   = 5; // D5

const uint8_t  temp_sensor_pin    = A0;    /**> NTC thermistor temperature sensor input pin             */
const uint8_t  current_sensor_pin = A1;    /**> Current sens input pin, sampled @ 500Hz - 1kHz          */
const uint16_t upper_resistance   = 330U;  /**> 320 kOhms resistor is used as the upper bridge resistor */
const uint16_t vcc_mv             = 5000U; /**> Board is powered via USB -> 5V                          */

const uint8_t led_driver_index = 0U;
/**
 * @brief controls the application state (state machine)
 */
typedef enum
{
    APP_STATE_NORMAL,         /**> Normal fridge operation                                                          */
    APP_STATE_POST_BOOT_WAIT, /**> Post boot time window waits for 5 seconds without triggering the compressor.
                                   This allows the user to enter the Learning mode when exiting the boot.           */
    APP_STATE_MOTOR_STALLED   /**> Motor stalled condition was detected, waiting for 5 minutes before trying again  */
} app_state_t;

typedef struct
{
    app_state_t app_state; /**> Tracks application current state                                               */

    /**
     * @brief maps the start time conditions in a union
     * (because we are only using one value at a time, they are mutually exclusive)
     */
    union
    {
        uint32_t stalled_cond_time; /**> Keeps track of the stalled condition start in time */
        uint32_t motor_start_time;  /**> Keeps track of the time where motor was started    */
    } tracking;

    /**
     * @brief Keeps track of button events (either Pressed, Released, or Hold)
     */
    struct
    {
        button_state_t plus_event;      /**> Plus button event                                                                                   */
        button_state_t prev_plus_event; /**> Used to detect the change in button event (detecting from Pressed to Hold and from Hold to Release) */

        // We need to detect transistion from the PRESSED event to the HOLD event exactly once
        button_state_t minus_event;      /**> Minus button event                                                                                  */
        button_state_t prev_minus_event; /**> Used to detect the change in button event (detecting from Pressed to Hold and from Hold to Release) */
    } buttons;

} app_working_mem_t;

typedef struct
{
    int8_t   temperature;
    uint16_t current_ma;
    uint16_t current_rms;
} app_sensors_t;

// Default configuration initialisation
static persistent_config_t config = {
    .header             = PERMANENT_STORAGE_HEADER,
    .target_temperature = 4,
    .current_threshold  = 500,
    .footer             = PERMANENT_STORAGE_FOOTER,
};

static led_io_t leds[1U] = {{.port = &PORTD, .pin = status_led_pin}};

static void read_buttons_events(button_state_t* const plus_button_event, button_state_t* const minus_button_event, const mcu_time_t* time);

static app_state_t handle_motor_stalled_loop(uint32_t const* const start_time, const mcu_time_t* time);
static void        handle_normal_operation_loop(app_working_mem_t* const app_mem, uint16_t const* const current_rms, const int8_t temperature,
                                                const mcu_time_t* time);
static void        set_motor_output(const uint8_t value);
static bool        is_motor_started(void);
static void        read_current(const mcu_time_t* time, uint16_t* current_ma);
static void        read_temperature(const mcu_time_t* time, int8_t* temperature);

/**
 * @brief returns a fake RMS of the last 20 samples -> 20*50Hz = 1kHz.
 * Ideally we should use between 10 to 20 samples to represent a full sine wave
 * at 50Hz, so we would need to sample at around 500Hz - 1kHz.
 */
static uint16_t read_current_fake_rms(uint16_t const* const current_ma);

void setup()
{
    pinMode(minus_button_pin, INPUT);
    pinMode(plus_button_pin, INPUT);
    pinMode(motor_control_pin, OUTPUT);
    pinMode(status_led_pin, OUTPUT);
    pinMode(temp_sensor_pin, INPUT);
    pinMode(current_sensor_pin, INPUT);

    timebase_init();

    LOG_INIT();

    if (FORCE_OVERWRITE_EEPROM || persistent_mem_is_first_boot(PERMANENT_STORAGE_HEADER, PERMANENT_STORAGE_FOOTER))
    {
        LOG("Detected first boot condition, writing default config to EEPROM.\n");
        // Writes the default config on first boot so that it's a known starting
        // point for subsequent eeprom references.
        persistent_mem_write_config(&config);

        persistent_mem_read_config(&config);
    }
    else
    {
        LOG("Reading config from EEPROM.\n");
        // Otherwise, read back config from EEPROM
        persistent_mem_read_config(&config);
        LOG_CUSTOM("Read target temp in config : %d°C\n", config.target_temperature)
        LOG_CUSTOM("Read current threshold in config : %umA\n", (unsigned int)config.current_threshold)
    }

    led_init(leds, 1U);
    // led_set_blink_pattern(led_driver_index, LED_BLINK_NONE);
    sei();
}

void loop()
{
    static const mcu_time_t* time        = NULL;
    static int8_t            temperature = 0;
    static uint16_t          current_ma  = 0;

#ifdef DEBUG_REPORT_PERIODIC
    static mcu_time_t previous_time;
#endif

    // Keeps track of the previous time the system was toggled
    static app_working_mem_t app_mem = {
        .app_state = APP_STATE_POST_BOOT_WAIT,
        .tracking  = {.motor_start_time = 0},
        .buttons =
            {
                .plus_event       = BUTTON_STATE_RELEASED,
                .prev_plus_event  = BUTTON_STATE_RELEASED,
                .minus_event      = BUTTON_STATE_RELEASED,
                .prev_minus_event = BUTTON_STATE_RELEASED,
            }, // Trailing comma is used to work around clang-format issues with struct fields initialization formatting
    };

    // Very important to process the current time as fast as we can (polling mode)
    timebase_process();
    time = timebase_get_time();
    led_process(time);

    bool config_changed = false;

    // Current is read at around 1kHz
    read_current(time, &current_ma);
    // Temperature is read once every 2 seconds
    read_temperature(time, &temperature);
    uint16_t current_rms = 0;

#if CURRENT_RMS_ARBITRARY_FCT == 1
    uint16_t dc_offset_current = 300;
    current_compute_rms_arbitrary(&current_ma, &current_rms, &dc_offset_current);
#else
    current_compute_rms_sine(&current_ma, &current_rms);
#endif

    // Process button events.
    // Used to trigger
    read_buttons_events(&app_mem.buttons.plus_event, &app_mem.buttons.minus_event, time);

    // User pressed and release the + button.
    // Raise temp set point by one degree
    if (app_mem.buttons.plus_event == BUTTON_STATE_RELEASED && app_mem.buttons.prev_plus_event != app_mem.buttons.plus_event
        && app_mem.buttons.prev_plus_event != BUTTON_STATE_HOLD)
    {
        config.target_temperature++;
        LOG("Button + Clicked !\n");

        // Clamp max temperature to max of NTC curve
        if (config.target_temperature > thermistor_ntc_100k_3950K_data.data[thermistor_ntc_100k_3950K_data.sample_count - 1U].temperature)
        {
            config.target_temperature = thermistor_ntc_100k_3950K_data.data[thermistor_ntc_100k_3950K_data.sample_count - 1U].temperature;
        }
        config_changed = true;
        LOG_CUSTOM("-> New temp : %hd °C\n", config.target_temperature);
    }

    // User pressed and release the - button.
    // Reduce temp set point by one degree
    if (app_mem.buttons.minus_event == BUTTON_STATE_RELEASED && app_mem.buttons.prev_minus_event != app_mem.buttons.minus_event
        && app_mem.buttons.prev_minus_event != BUTTON_STATE_HOLD)

    {
        LOG("Button - Clicked !\n");
        config.target_temperature--;

        // Clamp max temperature to min of NTC curve
        if (config.target_temperature < thermistor_ntc_100k_3950K_data.data[0U].temperature)
        {
            config.target_temperature = thermistor_ntc_100k_3950K_data.data[0U].temperature;
        }
        config_changed = true;
        LOG_CUSTOM("-> New temp : %hd °C\n", config.target_temperature);
    }

    // Only update persistent configuration if it has changed (reduces the amount of writes)
    if (config_changed)
    {
        LOG("Writing configuration to EEPROM\n");
        persistent_mem_write_config(&config);
    }

    switch (app_mem.app_state)
    {
        case APP_STATE_MOTOR_STALLED: {
            app_mem.app_state = handle_motor_stalled_loop(&app_mem.tracking.stalled_cond_time, time);
            break;
        }

        case APP_STATE_POST_BOOT_WAIT:
        case APP_STATE_NORMAL:
        default: {
            handle_normal_operation_loop(&app_mem, &current_rms, temperature, time);
            break;
        }
    }

    // Update previous buttons states
    app_mem.buttons.prev_minus_event = app_mem.buttons.minus_event;
    app_mem.buttons.prev_plus_event  = app_mem.buttons.plus_event;

#if DEBUG_REPORT_PERIODIC == 1
    if ((time->seconds - previous_time.seconds) > DEBUG_REPORT_PERIOD_SECONDS)
    {
        // Report few things about current states
        previous_time = *time;
        LOG_CUSTOM("temperature : %hd °C\n", temperature);
        LOG_CUSTOM("current : %hu mA\n", current_ma);
        LOG_CUSTOM("current RMS: %hu mA\n", current_rms);
        LOG_CUSTOM("config.target_temperature : %hd °C\n", config.target_temperature);
        LOG_CUSTOM("config.current_threshold : %hu mA\n\n", config.current_threshold);
    }
#endif

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

static void read_buttons_events(button_state_t* const plus_button_event, button_state_t* const minus_button_event, const mcu_time_t* time)
{
    static button_local_mem_t plus_button_mem = {
        .current  = LOW,
        .previous = LOW,
        .pressed  = 0,
        .event    = BUTTON_STATE_RELEASED,
    };
    static button_local_mem_t minus_button_mem = {
        .current  = LOW,
        .previous = LOW,
        .pressed  = 0,
        .event    = BUTTON_STATE_RELEASED,
    };

    plus_button_mem.current  = digitalRead(plus_button_pin);
    minus_button_mem.current = digitalRead(minus_button_pin);

    read_single_button_event(&plus_button_mem, &time->seconds);
    read_single_button_event(&minus_button_mem, &time->seconds);

    *plus_button_event  = plus_button_mem.event;
    *minus_button_event = minus_button_mem.event;
}

// Note : very naive implementation
static uint16_t read_current_fake_rms(uint16_t const* const current_ma)
{
    static uint16_t data[SAMPLES_PER_SINE];
    static uint8_t  index = 0;

    // Circular buffer
    data[index] = *current_ma;
    index       = (index + 1) % SAMPLES_PER_SINE;

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
    uint16_t magnitude    = peak_to_peak / 2U;

    // Removing alias again on sqrt(2) with small(er) error margin
    return (magnitude * 10U) / 14U;
}

static app_state_t handle_motor_stalled_loop(uint32_t const* const start_time, const mcu_time_t* time)
{
    // Wait for 5 minutes before exiting this state
    uint32_t elapsed_time = time->seconds - *start_time;
    if (elapsed_time >= 5U * 60U)
    {
        LOG("Motor stalled timeout condition reached -> Reverting to normal mode.\n");
        // Revert to normal operation mode
        return APP_STATE_NORMAL;
    }
    return APP_STATE_MOTOR_STALLED;
}

static void handle_normal_operation_loop(app_working_mem_t* const app_mem, uint16_t const* const current_rms, const int8_t temperature,
                                         const mcu_time_t* time)
{
    // Only trigger this event once, at first detection of the button
    // HOLD event, not the subsequent ones.

    // clang-format off
    if ((BUTTON_STATE_HOLD == app_mem->buttons.minus_event)
    && (app_mem->buttons.prev_minus_event != app_mem->buttons.minus_event))
    // clang-format on
    {
        LOG("Button - hold condition detected : Reverting current threshold to default.\n");
        // Reset memory back to default (starts a new "Learning" mode)
        config.current_threshold = 0;
        persistent_mem_write_config(&config);
        led_set_blink_pattern(led_driver_index, LED_BLINK_ACCEPT);

        led_next_event_t event = {
            .kind = LED_NEXT_EVENT_PATTERN,
            .data = {.pattern = LED_BLINK_BREATHING}
        };
        led_set_next_event(led_driver_index, &event);

        if (is_motor_started())
        {
            // Rest the tracker so that we start counting from now on
            app_mem->tracking.motor_start_time = time->seconds;
        }
    }

    // Just learnt new "normal" motor behavior ! Save it to persistent memory
    bool motor_run_long_enough = (time->seconds - app_mem->tracking.motor_start_time) > STEADY_MOTOR_RUNTIME;
    if (is_motor_started() && (config.current_threshold == 0) && (motor_run_long_enough))
    {
        config.current_threshold = *current_rms;
        led_set_blink_pattern(led_driver_index, LED_BLINK_ACCEPT);
        led_next_event_t event = {
            .kind = LED_NEXT_EVENT_IO_STATE,
            .data = {.io_state = (uint8_t) HIGH}
        };
        led_set_next_event(led_driver_index, &event);

        persistent_mem_write_config(&config);
        LOG("Learnt new basis current for normal operation ; Saving to EEPROM\n");
        LOG_CUSTOM("Current : %u, threshold : %u\n", *current_rms, config.current_threshold)
    }

    // Detected stalled motor, stop trying to trigger the compressor for now
    bool overcurrent_detected = *current_rms > (STALLED_CURRENT_MULTIPLIER * config.current_threshold);
    if ((config.current_threshold > 0) && (overcurrent_detected))
    {
        LOG("Overcurrent detected, motor is probably stalled. Waiting for pressure to equalize in heat pump circuit.\n");
        app_mem->app_state = APP_STATE_MOTOR_STALLED;
        set_motor_output(LOW);
        app_mem->tracking.stalled_cond_time = time->seconds;

        led_set_blink_pattern(led_driver_index, LED_BLINK_WARNING);
        return;
    }

    // Simple hysteresis to control the compressor based on a target temperature
    if (!is_motor_started() && (temperature > (int8_t)(config.target_temperature + TEMP_HYSTERESIS_HIGH)))
    {
        // Start the compressor
        LOG("Starting motor : temperature is high enough.\n");
        set_motor_output(HIGH);
    }
    else if (is_motor_started() && (temperature < (int8_t)(config.target_temperature - TEMP_HYSTERESIS_LOW)))
    {
        LOG("Stopping motor : temperature is low enough.\n");
        // Stop the compressor
        set_motor_output(LOW);
        app_mem->tracking.motor_start_time = time->seconds;
    }
}

static bool is_motor_started(void)
{
    return digitalRead(motor_control_pin) == HIGH;
}

void set_motor_output(const uint8_t value)
{
    digitalWrite(motor_control_pin, value);
    digitalWrite(status_led_pin, value);
}

static void read_temperature(const mcu_time_t* time, int8_t* temperature)
{
    static uint32_t last_check_s = 0;

    // Only trigger temperature reading if elapsed time is greater than 1 second.
    if (time->seconds - last_check_s >= 1U)
    {
        uint16_t temp_reading_raw = analogRead(temp_sensor_pin);
        last_check_s              = time->seconds;

        // Using the x10 to lower aliasing but still retain a more accurate
        // millivolt reading Also, this remains right under the overflow : (5000 x 10 < UINT16_MAX)
        uint16_t temp_reading_mv = (((vcc_mv * 10U) / 1024) * temp_reading_raw) / 10U;

        uint16_t ntc_resistance = 0;
        bridge_get_lower_resistance(&upper_resistance, &temp_reading_mv, &vcc_mv, &ntc_resistance);

        *temperature = thermistor_read_temperature(&thermistor_ntc_100k_3950K_data, &ntc_resistance);

#if DEBUG_TEMP == 1
        LOG_CUSTOM("Temp mv : %u mV\n", temp_reading_mv)
        LOG_CUSTOM("Vcc mv : %u mV\n", vcc_mv)
        LOG_CUSTOM("Upper resistance : %u k\n", upper_resistance)
        LOG_CUSTOM("Temperature : %d °C\n\n", (int)*temperature)
        LOG_CUSTOM("NTC res : %u k\n", ntc_resistance)
        LOG_CUSTOM("Temp raw : %u /1024\n", temp_reading_raw)
#endif
    }
}

static void read_current(const mcu_time_t* time, uint16_t* current_ma)
{
    static uint16_t last_check_ms = 0;

    // Only trigger temperature reading if elapsed time is greater than 1 second.
    if (time->milliseconds - last_check_ms >= 2U)
    {
        uint16_t current_raw = analogRead(current_sensor_pin);
        last_check_ms        = time->milliseconds;

        uint16_t currend_reading_mv = (((vcc_mv * 10U) / 1024) * current_raw) / 10U;
        current_from_voltage(&currend_reading_mv, current_ma);
    }
}
