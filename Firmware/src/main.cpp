#include <Arduino.h>
#include <EEPROM.h>
#include <stdint.h>
#include <stdbool.h>

#include "Thermistor/thermistor.h"
#include "Thermistor/thermistor_ntc_100k_3950K.h"
#include "timebase.h"
#include "persistent_memory.h"

// #define F_CPU 16000000

// Permanent storage header and footer are used to make sure EEPROM was already used and is valid.
// These are default constant values which is really unlikely we'll find in the EEPROM straight from factory.
// They will be used to invalidate cached values and trigger board auto-learning
#define PERMANENT_STORAGE_HEADER 0xDE
#define PERMANENT_STORAGE_FOOTER 0xAD

#define BUTTON_PRESS_TIME_THRESHOLD 3U

// Pin mapping
const uint8_t motor_control_pin = 2; // D2
const uint8_t status_led_pin = 3;    // D3
const uint8_t minus_button_pin = 4;  // D4
const uint8_t plus_button_pin = 5;   // D5

const uint8_t temp_sensor_pin = A0;
const uint8_t current_sensor_pin = A1;

const uint16_t time_on_seconds = 3600;
const uint16_t time_off_seconds = 7200;

typedef enum
{
    BUTTON_EVENT_HOLD,
    BUTTON_EVENT_PRESSED,
    BUTTON_EVENT_RELEASED,
    BUTTON_EVENT_DEFAULT
} button_event_t;

void set_motor_output(const uint8_t value)
{
    digitalWrite(motor_control_pin, LOW);
    digitalWrite(status_led_pin, LOW);
}

// Default configuration initialisation
static persistent_config_t config =
    {
        .header = PERMANENT_STORAGE_HEADER,
        .target_temperature = 4,
        .current_threshold = 0,
        .footer = PERMANENT_STORAGE_FOOTER};

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
        // Writes the default config on first boot so that it's a known starting point for
        // subsequent eeprom references.
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

typedef struct
{
    uint8_t current;      /**> Current button state (read from IO)          */
    uint8_t previous;     /**> Previous button state (used to raise events) */
    uint32_t pressed;     /**> When the button was pressed (using timebase) */
    button_event_t event; /**> Output button event                          */
} button_local_mem_t;

void read_single_button_event(button_local_mem_t *const button, uint32_t const *const time)
{
    if (button->current == HIGH)
    {
        // Start of long press, count press duration
        if (button->current == button->previous)
        {
            const uint32_t duration = *time - button->pressed;
            if (duration >= BUTTON_PRESS_TIME_THRESHOLD)
            {
                button->event = BUTTON_EVENT_HOLD;
            }
        }
        // New button pressed event (was not pressed before)
        else
        {
            button->event = BUTTON_EVENT_PRESSED;
            button->pressed = *time;
        }
    }
    // Button is released
    else
    {
        button->event = BUTTON_EVENT_RELEASED;
    }
    button->previous = button->current;
}

void read_buttons_events(button_event_t *const plus_button_event, button_event_t *const minus_button_event, uint32_t const *const seconds)
{
    static button_local_mem_t minus_button_mem =
        {
            .current = LOW,
            .previous = LOW,
            .pressed = 0,
            .event = BUTTON_EVENT_DEFAULT,
        };
    static button_local_mem_t plus_button_mem;

    static uint8_t previous_plus_button_state = LOW;
    static uint32_t plus_button_pressed_start = 0;

    static uint8_t previous_minus_button_state = LOW;
    static uint32_t minus_button_pressed_start = 0;

    uint8_t current_plus_button_state = digitalRead(plus_button_pin);
    uint8_t current_minus_button_state = digitalRead(minus_button_pin);

    if (current_plus_button_state == HIGH)
    {
        // Start of long press, count press duration
        if (current_plus_button_state == previous_plus_button_state)
        {
            const uint32_t duration = *seconds - plus_button_pressed_start;
            if (duration >= BUTTON_PRESS_TIME_THRESHOLD)
            {
                *plus_button_event = BUTTON_EVENT_HOLD;
            }
        }
        // New button pressed event (was not pressed before)
        else
        {
            *plus_button_event = BUTTON_EVENT_PRESSED;
            plus_button_pressed_start = *seconds;
        }
    }
    // Button is released
    else
    {
        *plus_button_event = BUTTON_EVENT_RELEASED;
    }
    previous_plus_button_state = current_plus_button_state;

    // Same for the minus button
    if (current_plus_button_state == HIGH)
    {
        *plus_button_event = BUTTON_EVENT_PRESSED;
        if (current_plus_button_state == previous_plus_button_state)
        {
            plus_button_pressed_duration = *seconds - plus_button_pressed_start;
            if (plus_button_pressed_duration >= BUTTON_PRESS_TIME_THRESHOLD)
            {
                *plus_button_event = BUTTON_EVENT_HOLD;
            }
        }
    }
    else
    {
        *plus_button_event = BUTTON_EVENT_RELEASED;
    }
}

void loop()
{
    // When the button is pushed, it means the refrigerator is in its "forced mode"
    static bool forced_run = false;
    static bool cooling_down = true;

    // Keeps track of the previous time the system was toggled
    static uint32_t previous_toggle = 0;
    static uint32_t seconds = 0;
    static uint16_t temp_reading = 0;
    static uint16_t current_reading = 0;
    static button_event_t plus_button_event = BUTTON_EVENT_DEFAULT;
    static button_event_t minus_button_event = BUTTON_EVENT_DEFAULT;

    timebase_get_time(&seconds);
    temp_reading = analogRead(temp_sensor_pin);
    current_reading = analogRead(current_sensor_pin);
    read_buttons_events(&plus_button_event, &minus_button_event);
    // Read temp
    // Process (filter) temp reading -> hysteresis
    // Read current
    // Process current reading

    // If motor current is too high, increment motor_current_high_seconds counter for it
    // Then if that counter goes above 3 seconds, we set the forced stalled flag and reset the counter back to 0
    // -> We enter a "safe" loop where compressor won't be triggered

    // if temp is not in appropriate range
    //      -> Cooling is required
    // else, do nothing

    // Then, if already cooling and motor current was too high for at least 5 seconds,

    if ((cooling_down) && (seconds - previous_toggle) >= time_on_seconds)
    {
        // Toggling cooling mode
        cooling_down = false;
        previous_toggle = seconds;
    }
    else if ((!cooling_down && (seconds - previous_toggle) >= time_off_seconds))
    {
        cooling_down = true;
        previous_toggle = seconds;
    }

    if (cooling_down)
    {
        digitalWrite(relay_pin_1, LOW);
        digitalWrite(relay_pin_2, LOW);
    }
    else
    {
        digitalWrite(relay_pin_1, HIGH);
        digitalWrite(relay_pin_2, HIGH);
    }
}