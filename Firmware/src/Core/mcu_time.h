#ifndef TIME_HEADER
#define TIME_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief helps keeping track of time
 */
typedef struct
{
    uint32_t seconds;      /**> Counts seconds         */
    uint16_t milliseconds; /**> Counts milliseconds    */
} mcu_time_t;

/**
 * @brief resets time construct to default. (All zeros)
 * @param[in/out] time : time construct to be reset
 */
void time_default(mcu_time_t *time);

#ifdef __cplusplus
}
#endif

#endif /* TIME_HEADER */