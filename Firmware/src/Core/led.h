#ifndef LED_HEADER
#define LED_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_time.h"
#include <stdint.h>

#define MAX_LED_COUNT 1

// clang-format off

// ################################### LED BLINK BREATHING PATTERN Defines ##############################################
#define LED_BLINK_BREATHING_PERIOD_S 4U                                                                     /**> LED breathing cycle period                     */
#define LED_BLINK_BREATHING_HALF_P (LED_BLINK_BREATHING_PERIOD_S / 2U)                                      /**> LED breathing half period                      */
#define LED_BLINK_BREATHING_FREQ_H 25                                                                       /**> LED breathing base update frequency (Hertz)    */
#define LED_BLINK_BREATHING_UPDATE_MS (1000 / LED_BLINK_BREATHING_FREQ_H)                                   /**> LED breathing base update period (millisecond) */
#define LED_BLINK_BREATHING_HALF_CYCLE_STEPS (LED_BLINK_BREATHING_FREQ_H * LED_BLINK_BREATHING_HALF_P)      /**> Number of events (steps) for half a period     */
#define LED_BLINK_BREATHING_FULL_CYCLE_STEPS (LED_BLINK_BREATHING_FREQ_H * LED_BLINK_BREATHING_PERIOD_S)    /**> Number of events (steps) for the full period   */
#define LED_BLINK_BREATHING_DUTY_CYCLE_INC  \
    (100 / (LED_BLINK_BREATHING_HALF_P * LED_BLINK_BREATHING_FREQ_H))                                       /**> LED breathing duty cycle increment             */


// ################################### LED BLINK WARNING PATTERN Defines ##############################################
#define LED_BLINK_WARNING_PERIOD_S 2U                                                                       /**> LED warning cycle period                       */
#define LED_BLINK_WARNING_HALF_P (LED_BLINK_WARNING_PERIOD_S / 2U)                                          /**> LED warning half period                        */


// ################################### LED BLINK ACCEPT PATTERN Defines ##############################################
#define LED_BLINK_ACCEPT_PERIOD_S  1U                               /**> LED warning cycle period (seconds)                                         */
#define LED_BLINK_ACCEPT_PERIOD_MS \
    (1000U  * LED_BLINK_ACCEPT_PERIOD_S)                            /**> LED accept pattern cycle period (milliseconds)                             */

#define LED_BLINK_ACCEPT_FLASHES  3U                                /**> LED accept pattern num of flashes                                          */
#define LED_BLINK_ACCEPT_CYCLE_PERIOD_MS \
    (LED_BLINK_ACCEPT_PERIOD_MS / LED_BLINK_ACCEPT_FLASHES)         /**> LED accept pattern individual cycle period (milliseconds)                  */
#define LED_BLINK_ACCEPT_ON_TIME_MS \
    (LED_BLINK_ACCEPT_CYCLE_PERIOD_MS / 2U)                         /**> LED accept pattern ON time (milliseconds) - half period (on/off is 333 ms) */
// clang-format on

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
    volatile uint8_t *port; /**> Specifies the LED IO port (needs to be a pointer to the PORT[ABCD]) */
    uint8_t pin;            /**> Specifies the LED IO id (0 to 7)                                    */
} led_io_t;

/**
 * @brief initializes the config parameter to default values.
 */
void led_static_config_default(led_io_t *config);

/**
 * @brief configures the LED driver with static configuration.
 */
void led_init(const led_io_t *config, const uint8_t length);

/**
 * @brief clears out internal memory and reverts LED states back to defaults.
*/
void led_reset(void);

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