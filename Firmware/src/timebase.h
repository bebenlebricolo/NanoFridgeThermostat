#ifndef TIMEBASE_HEADER
#define TIMEBASE_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>


/**
 * @brief initialises timebase and starts timer 2
*/
void timebase_init(void);

/**
 * @brief Retrieves current time
*/
void timebase_get_time(uint32_t * const seconds);

/**
 * @brief resets internal time back to 0 and reverts timer 2 to its original condition.
*/
void timebase_reset(void);


#ifdef __cplusplus
}
#endif

#endif /* TIMEBASE_HEADER */