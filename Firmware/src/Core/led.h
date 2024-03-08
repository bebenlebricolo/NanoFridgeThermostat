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
#define LED_BLINK_BREATHING_PERIOD_S 4U                                                                     /**> LED breathing cycle period                             */
#define LED_BLINK_BREATHING_HALF_P (LED_BLINK_BREATHING_PERIOD_S / 2U)                                      /**> LED breathing half period                              */
#define LED_BLINK_BREATHING_FREQ_H 70                                                                      /**> LED breathing base update frequency (Hertz)            */
#define LED_BLINK_BREATHING_RESOLUTION_MS (1000 / LED_BLINK_BREATHING_FREQ_H)                               /**> LED PWM Resolution (how many millisecond per "step")   */
#define LED_BLINK_BREATHING_HALF_CYCLE_STEPS (LED_BLINK_BREATHING_FREQ_H * LED_BLINK_BREATHING_HALF_P)      /**> Number of events (steps) for half a period             */
#define LED_BLINK_BREATHING_FULL_CYCLE_STEPS (LED_BLINK_BREATHING_FREQ_H * LED_BLINK_BREATHING_PERIOD_S)    /**> Number of events (steps) for the full period           */
#define LED_BLINK_BREATHING_DUTY_CYCLE_INC_ALIASING_FACTOR 20
#define LED_BLINK_BREATHING_DUTY_CYCLE_INC  \
    ((LED_BLINK_BREATHING_DUTY_CYCLE_INC_ALIASING_FACTOR * 100) / (LED_BLINK_BREATHING_HALF_P * LED_BLINK_BREATHING_FREQ_H))   /**> LED breathing duty cycle increment             */

/**
 * @brief Macro used to enable pulse stuffing in software PWM code.
 * Pulse stuffing is used to represent duty cycles which are not achievable with the physical resolution alone.
 * For instance, when requiring a 100Hz pulse with a millisecond tick capability, we'll end with 1000ms/100Hz steps => 10 ms time period (window) per PWM pulse @100Hz.
 * It means we only achieve a 10 grades resolution @100Hz max. And applied duty cycle (e.g. 50 duty -> 0-4 (ON), then 5-9 (OFF), same for 51, 53, 59.) will be aligned with
 * the pulses.
 * However we can use the remaining part of the division to stuff several consecutive time window (10) with a mix and match of pulses to virtually achieve intermediate levels of resolution.
 * E.g. :
 * 100 Hz, duty cycle of 55% :
 * [5,6,5,6,5,6,5,6,5,6] -> The 1:1 alternating 5 ms and 6 ms pulse simulates the "effective" pulse in the exact middle
 *
 * Duty cycle : 33%
 * [3,3,4,3,3,4,3,3,4,3]
*/
// #ifndef LED_PWM_NO_PULSE_STUFFING
// #define LED_PWM_PULSE_STUFFING
// #endif

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

uint8_t led_breathing_get_duty_sawtooth(const uint16_t step);

#ifdef __cplusplus
}
#endif

#endif /* LED_HEADER */