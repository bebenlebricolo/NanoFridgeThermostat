#include "led.h"
#include <stdbool.h>
#include <stddef.h>

#include "interpolation.h"

typedef struct
{
    bool configured;           /**> Used to know if this specific LED construct is used or not */
    led_io_t io;               /**> Static led IO config                                       */
    mcu_time_t last_processed; /**> Last time this LED was processed (used to perform soft PWM)*/
    struct
    {
        led_blink_pattern_t current;  /**> Current LED Pattern (used to trigger state changed events) */
        led_blink_pattern_t previous; /**> Previous LED Pattern (used to trigger state changed events)*/
    } patterns;

    union
    {
        struct
        {
            uint8_t step; /**> Keeps track of the current duty cycle applied to the LED   */
        } breathing;

        struct
        {
            uint8_t count; /**> Keeps track of the amount of blinks already executed       */
        } accept;
    } states;
} internal_config_t;

static internal_config_t internal_config[MAX_LED_COUNT];

static void handle_led_accept(mcu_time_t const *const time, internal_config_t *const config);
static void handle_led_warning(mcu_time_t const *const time, internal_config_t *const config);
static void handle_led_breathing(mcu_time_t const *const time, internal_config_t *const config);
static void get_elapsed_milliseconds(mcu_time_t const* const time, internal_config_t * const config, uint32_t * const elapsed);


static void led_on(led_io_t *const config);
static void led_off(led_io_t *const config);

void led_static_config_default(led_io_t *io)
{
    io->pin = 0;
    io->port = NULL;
}

void led_reset(void)
{
    for (uint8_t i = 0; i < MAX_LED_COUNT; i++)
    {
        led_static_config_default(&internal_config[i].io);
        internal_config[i].configured = false;
        time_default(&internal_config[i].last_processed);
        internal_config[i].patterns.current = LED_BLINK_NONE;
        internal_config[i].patterns.previous = LED_BLINK_NONE;
    }
}

void led_init(const led_io_t *config, const uint8_t length)
{
    led_reset();
    uint8_t max_length = length < MAX_LED_COUNT ? length : MAX_LED_COUNT;
    for (uint8_t i = 0; i < max_length; i++)
    {
        internal_config[i].io.port = config[i].port;
        internal_config[i].io.pin = config[i].pin;
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

    internal_config_t * config = &internal_config[led_id];
    config->patterns.current = pattern;

    // Reinitializes the "states" trackers
    switch(pattern)
    {
        case LED_BLINK_ACCEPT:
            config->states.accept.count = 0;
            break;

        case LED_BLINK_BREATHING:
            config->states.breathing.step = 0;
            break;

        case LED_BLINK_WARNING:
        default:
            break;
    }
}

void led_process(mcu_time_t const *const time)
{
    for (uint8_t i = 0; i < MAX_LED_COUNT; i++)
    {
        internal_config_t * const config = &internal_config[i];
        if (false == internal_config[i].configured)
        {
            // First non configured led implies there is no more configured ones after it
            // This is sufficient for us to jump out
            break;
        }

        // Update time of last processed (useful to start new patterns)
        if(config->patterns.current != config->patterns.previous)
        {
            config->last_processed = *time;
        }

        switch (config->patterns.current)
        {
            case LED_BLINK_BREATHING:
                handle_led_breathing(time, &internal_config[i]);
                break;

            case LED_BLINK_WARNING:
                handle_led_warning(time, &internal_config[i]);
                break;

            case LED_BLINK_ACCEPT:
                handle_led_accept(time, &internal_config[i]);
                break;

            case LED_BLINK_NONE:
            default:
                // Only turn the led off in case of event generation
                if (config->patterns.previous != config->patterns.current)
                {
                    led_off(&config->io);
                }

                break;
        }

        config->patterns.previous = config->patterns.current;
    }
}

static void handle_led_accept(mcu_time_t const *const time, internal_config_t *const config)
{
    uint32_t elapsed = 0;
    get_elapsed_milliseconds(time, config, &elapsed);

    if(elapsed >= LED_BLINK_ACCEPT_CYCLE_PERIOD_MS)
    {
        config->last_processed.milliseconds = time->milliseconds;
        config->states.accept.count++;
    }

    // Reverts back to original state when pattern is fully executed
    if(config->states.accept.count >= LED_BLINK_ACCEPT_FLASHES)
    {
        config->states.accept.count = 0;
        config->patterns.current = LED_BLINK_NONE;
        return;
    }

    uint32_t elapsed_clamped = elapsed % LED_BLINK_ACCEPT_CYCLE_PERIOD_MS;

    // First half of the cycle period (ON time)
    if(elapsed_clamped < LED_BLINK_ACCEPT_ON_TIME_MS)
    {
        led_on(&config->io);
    }
    // Second half of the cycle period (OFF time)
    else if(elapsed_clamped < LED_BLINK_ACCEPT_CYCLE_PERIOD_MS)
    {
        led_off(&config->io);
    }
}

static void handle_led_warning(mcu_time_t const *const time, internal_config_t *const config)
{
    uint32_t elapsed = (time->seconds - config->last_processed.seconds);
    if ((elapsed % LED_BLINK_WARNING_PERIOD_S ) < LED_BLINK_WARNING_HALF_P)
    {
        led_on(&config->io);
    }
    else if((elapsed % LED_BLINK_WARNING_PERIOD_S ) < LED_BLINK_WARNING_PERIOD_S)
    {
        led_off(&config->io);
    }

    if(elapsed >= LED_BLINK_WARNING_PERIOD_S)
    {
        config->last_processed.seconds = time->seconds;
    }
}

static void handle_led_breathing(mcu_time_t const *const time, internal_config_t *config)
{
    uint32_t elapsed = 0;
    get_elapsed_milliseconds(time, config, &elapsed);

    // Time to process the new duty-cycle
    // Event is generated at a target frequency of 25Hz
    if (elapsed <= LED_BLINK_BREATHING_UPDATE_MS)
    {
        uint8_t duty = led_breathing_get_duty_sawtooth(config->states.breathing.step);

        range_uint8_t input = {.start = 0, .end = 100};
        range_uint8_t output = {.start = 0, .end = LED_BLINK_BREATHING_UPDATE_MS - 1};

        uint8_t duty_ms = 0;
        duty_ms = interpolation_linear_uint8_to_uint8(duty, &input, &output);

        if (elapsed >= duty_ms)
        {
            // Switch off the LED
            led_off(&config->io);
        }
    }
    // Reset the window
    else
    {
        config->last_processed = *time;
        // Switch on the LED
        led_on(&config->io);

        config->states.breathing.step++;
        config->states.breathing.step %= LED_BLINK_BREATHING_FULL_CYCLE_STEPS;
    }
}

static void led_on(led_io_t *const config)
{
    *config->port |= (1 << config->pin);
}

static void led_off(led_io_t *const config)
{
    *config->port &= ~(1 << config->pin);
}

static void get_elapsed_milliseconds(mcu_time_t const* const time, internal_config_t * const config, uint32_t * const elapsed)
{
    if (time->milliseconds >= config->last_processed.milliseconds)
    {
        *elapsed = time->milliseconds - config->last_processed.milliseconds;
    }
    // Milliseconds was reset to 0 after reaching 1000
    else
    {
        *elapsed = (time->milliseconds + 1000) - config->last_processed.milliseconds;
    }
}

uint8_t led_breathing_get_duty_sawtooth(const uint8_t step)
{
    uint8_t duty = 0;
    // Sawtooth implementation, positive ramp (0 to 100)
    if (step <= LED_BLINK_BREATHING_HALF_CYCLE_STEPS)
    {
        duty = step * LED_BLINK_BREATHING_DUTY_CYCLE_INC;
    }
    // Sawtooth implementation, negative ramp (100 to 0)
    else
    {
        duty = 100 - ((step - (LED_BLINK_BREATHING_HALF_CYCLE_STEPS)) * LED_BLINK_BREATHING_DUTY_CYCLE_INC);
    }

    return duty;
}