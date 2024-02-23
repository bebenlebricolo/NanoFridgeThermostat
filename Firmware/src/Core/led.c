#include "led.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    bool configured;                /**> Used to know if this specific LED construct is used or not */
    led_static_config_t config;     /**> Static led config                                          */
    mcu_time_t last_processed;          /**> Last time this LED was processed (used to perform soft PWM)*/
} internal_config_t;

static internal_config_t internal_config[MAX_LED_COUNT];

static void reset_internals(void);

static void handle_led_warning(mcu_time_t const * const time, internal_config_t * config);
static void handle_led_breathing(mcu_time_t const * const time, internal_config_t * config);

void led_static_config_default(led_static_config_t * config)
{
    config->pattern = LED_BLINK_NONE;
    config->pin_id = 0;
    config->port = NULL;
}

void led_init(const led_static_config_t * config, const uint8_t length)
{
    reset_internals();
    uint8_t max_length = length < MAX_LED_COUNT ? length : MAX_LED_COUNT;
    for(uint8_t i = 0 ; i < max_length ; i++)
    {
        internal_config[i].config.pattern = config[i].pattern;
        internal_config[i].config.port = config[i].port;
        internal_config[i].config.pin_id = config[i].pin_id;
        internal_config[i].configured = true;
    }
}

void led_set_blink_pattern(const uint8_t led_id, const led_blink_pattern_t pattern)
{
    if(led_id >= MAX_LED_COUNT)
    {
        return ;
    }

    internal_config[led_id].config.pattern = pattern;
}

void led_process(mcu_time_t const * const time)
{
    for(uint8_t i = 0 ; i < MAX_LED_COUNT ; i++)
    {
        if(false == internal_config[i].configured)
        {
            // First non configured led implies there is no more configured ones after it
            // This is sufficient for us to jump out
            break;
        }

        switch(internal_config[i].config.pattern)
        {
            case LED_BLINK_BREATHING :
                handle_led_breathing(time, &internal_config[i]);
                break;

            case LED_BLINK_WARNING :
                handle_led_warning(time, &internal_config[i]);
                break;

            case LED_BLINK_NONE:
            default:
                break;
        }

    }
}

static void reset_internals(void)
{
    for(uint8_t i = 0 ; i < MAX_LED_COUNT ; i++)
    {
        led_static_config_default(&internal_config[i].config);
        internal_config[i].configured = false;
        time_default(&internal_config[i].last_processed);
    }
}

static void handle_led_warning(mcu_time_t const * const time, internal_config_t * config)
{

}

static void handle_led_breathing(mcu_time_t const * const time, internal_config_t * config)
{

}
