#ifndef LED_HEADER
#define LED_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_time.h"
#include <stdint.h>

#define MAX_LED_COUNT 1

#define LED_BLINK_BREATHING_PERIOD_S 4U

#define LED_BLINK_WARNING_PERIOD_S 2U

typedef enum
{
    LED_BLINK_ACCEPT,    /**> Shows the "accept/ok" pattern : 3 short blinks then back to previous state                            */
    LED_BLINK_WARNING,   /**> Shows the "warning" pattern : continuous slow blink 1s ON 1s OFF                                      */
    LED_BLINK_BREATHING, /**> Shows the "breathing" pattern : soft dimming from 0 to 100 and 100 to 0 (sawtooth pattern, 4s period) */
    LED_BLINK_NONE       /**> No particular pattern is applied to the LED*/
} led_blink_pattern_t;

/**
 * @brief encodes the static configuration we need for led driver
 */
typedef struct
{
    volatile uint8_t *port;      /**> Specifies the LED IO port (needs to be a pointer to the PORT[ABCD]) */
    uint8_t pin_id;              /**> Specifies the LED IO id (0 to 7)                                    */
    led_blink_pattern_t pattern; /**> Blink pattern */
} led_static_config_t;

/**
 * @brief initializes the config parameter to default values.
 */
void led_static_config_default(led_static_config_t *config);

/**
 * @brief configures the LED driver with static configuration.
 */
void led_init(const led_static_config_t *config, const uint8_t length);

/**
 * @brief processes events on all registered LEDs.
 */
void led_process(mcu_time_t const *const time);

/**
 * @brief changes the blink pattern for a single led
 */
void led_set_blink_pattern(const uint8_t led_id, const led_blink_pattern_t pattern);

#ifdef __cplusplus
}
#endif

#endif /* LED_HEADER */