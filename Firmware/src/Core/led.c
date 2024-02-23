#include "led.h"
#include <stdbool.h>
#include <stddef.h>

#include "interpolation.h"

typedef struct
{
    bool configured;            /**> Used to know if this specific LED construct is used or not */
    led_static_config_t config; /**> Static led config                                          */
    mcu_time_t last_processed;  /**> Last time this LED was processed (used to perform soft PWM)*/
    union
    {
        struct
        {
            uint8_t step;    /**> Keeps track of the current duty cycle applied to the LED   */
        } breathing;

        struct
        {
            uint8_t count;  /**> Keeps track of the amount of blinks already executed       */
        } accept;
    } states;
} internal_config_t;

static internal_config_t internal_config[MAX_LED_COUNT];

static void reset_internals(void);

static void handle_led_accept(mcu_time_t const *const time, internal_config_t * const config);
static void handle_led_warning(mcu_time_t const *const time, internal_config_t * const config);
static void handle_led_breathing(mcu_time_t const *const time, internal_config_t * const config);

static void toggle(led_static_config_t * const config);

void led_static_config_default(led_static_config_t *config)
{
    config->pattern = LED_BLINK_NONE;
    config->pin_id = 0;
    config->port = NULL;
}

void led_init(const led_static_config_t *config, const uint8_t length)
{
    reset_internals();
    uint8_t max_length = length < MAX_LED_COUNT ? length : MAX_LED_COUNT;
    for (uint8_t i = 0; i < max_length; i++)
    {
        internal_config[i].config.pattern = config[i].pattern;
        internal_config[i].config.port = config[i].port;
        internal_config[i].config.pin_id = config[i].pin_id;
        internal_config[i].configured = true;
        internal_config[i].states.breathing.step = 0;
    }
}

void led_set_blink_pattern(const uint8_t led_id, const led_blink_pattern_t pattern)
{
    if (led_id >= MAX_LED_COUNT)
    {
        return;
    }

    internal_config[led_id].config.pattern = pattern;
}

void led_process(mcu_time_t const *const time)
{
    for (uint8_t i = 0; i < MAX_LED_COUNT; i++)
    {
        if (false == internal_config[i].configured)
        {
            // First non configured led implies there is no more configured ones after it
            // This is sufficient for us to jump out
            break;
        }

        switch (internal_config[i].config.pattern)
        {
            case LED_BLINK_BREATHING:
                handle_led_breathing(time, &internal_config[i]);
                break;

            case LED_BLINK_WARNING:
                handle_led_warning(time, &internal_config[i]);
                break;

            case LED_BLINK_ACCEPT :
                handle_led_accept(time, &internal_config[i]);
                break;

            case LED_BLINK_NONE:
            default:
                break;
        }
    }
}

static void reset_internals(void)
{
    for (uint8_t i = 0; i < MAX_LED_COUNT; i++)
    {
        led_static_config_default(&internal_config[i].config);
        internal_config[i].configured = false;
        time_default(&internal_config[i].last_processed);
    }
}


static void handle_led_accept(mcu_time_t const *const time, internal_config_t * const config)
{
    (void) time;
    (void) config;
}


static void handle_led_warning(mcu_time_t const *const time, internal_config_t * const internal)
{
    uint32_t elapsed = (time->seconds - internal->last_processed.seconds);
    if(elapsed > LED_BLINK_WARNING_HALF_P)
    {
        toggle(&internal->config);
        internal->last_processed.seconds = time->seconds;
    }
}

static void handle_led_breathing(mcu_time_t const *const time, internal_config_t *config)
{
    uint32_t elapsed = 0;
    if(time->milliseconds > config->last_processed.milliseconds)
    {
        elapsed = time->milliseconds - config->last_processed.milliseconds;
    }
    // Milliseconds was reset to 0 after reaching 1000
    else
    {
        elapsed = (time->milliseconds + 1000) - config->last_processed.milliseconds;
    }

    // Time to process the new duty-cycle
    // Event is generated at a target frequency of 25Hz
    if(elapsed <= LED_BLINK_BREATHING_UPDATE_MS)
    {
        uint8_t duty = 0;

        // NOTE : can be rewritten so that duty cycle is not recomputed multiple times for the same step
        // Sawtooth implementation, positive ramp (0 to 100)
        if(config->states.breathing.step <= LED_BLINK_BREATHING_HALF_CYCLE_STEPS)
        {
            duty = config->states.breathing.step * LED_BLINK_BREATHING_DUTY_CYCLE_INC;
        }
        // Sawtooth implementation, negative ramp (100 to 0)
        else
        {
            duty = 100 - ((config->states.breathing.step - (LED_BLINK_BREATHING_FREQ_H * LED_BLINK_BREATHING_HALF_P)) * LED_BLINK_BREATHING_DUTY_CYCLE_INC);
        }

        range_uint8_t input = {
            .start = 0,
            .end = 100
        };
        range_uint8_t output = {
            .start = 0,
            .end = LED_BLINK_BREATHING_UPDATE_MS - 1
        };

        uint8_t duty_ms = 0;
        duty_ms = interpolation_linear_uint8_to_uint8(duty, &input, &output );

        if(elapsed >= duty_ms)
        {
            // Switch off the LED
            *config->config.port &= (1 << config->config.pin_id);
        }

        config->states.breathing.step++;
        config->states.breathing.step %= LED_BLINK_BREATHING_FULL_CYCLE_STEPS;
    }
    // Reset the window
    else
    {
        config->last_processed = *time;
    }
}

static void toggle(led_static_config_t * const config)
{
    *config->port ^= (1 << config->pin_id);
}
