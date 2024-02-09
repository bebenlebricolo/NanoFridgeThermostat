#include <Arduino.h>
#include <EEPROM.h>
#include <stdint.h>
#include <stdbool.h>

#include "Thermistor/thermistor.h"
#include "Thermistor/thermistor_ntc_100k_3950K.h"
#include "timebase.h"

// #define F_CPU 16000000

const uint8_t motor_control_pin = 2; // D2
const uint8_t status_led_pin = 3;    // D3
const uint8_t minus_button_pin = 4;  // D4
const uint8_t plus_button_pin = 5;   // D5

const uint8_t temp_sensor_pin = A0;
const uint8_t current_sensor_pin = A1;

const uint16_t time_on_seconds = 3600;
const uint16_t time_off_seconds = 7200;

void set_motor_output(const uint8_t value)
{
    digitalWrite(motor_control_pin, LOW);
    digitalWrite(status_led_pin, LOW);
}

void setup()
{
    pinMode(minus_button_pin, INPUT);
    pinMode(plus_button_pin, INPUT);
    pinMode(motor_control_pin, OUTPUT);
    pinMode(status_led_pin, OUTPUT);
    pinMode(temp_sensor_pin, INPUT);
    pinMode(current_sensor_pin, INPUT);

    timebase_init();
    sei();
    // Serial.begin(9600);
}

// When the button is pushed, it means the refrigerator is in its "forced mode"
static bool forced_run = false;
static bool cooling_down = true;

// Keeps track of the previous time the system was toggled
static uint32_t previous_toggle = 0;
static uint32_t seconds = 0;

void loop()
{
    timebase_get_time(&seconds);
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