#ifndef LOGICAL_AND_HEADER
#define LOGICAL_AND_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define LOGAND_MAX_BITS ((uint8_t)16U)

/**
 * @brief holds a compact buffer for a very simple AND filter
 */
typedef struct
{
    uint16_t data;     /**> Internal buffer, uses bit stuffing to optimize memory footprint                      */
    uint8_t  index;    /**> Internal index of the current bit in the data field. Used internally                 */
    uint8_t  max_bits; /**> Specifies how many bits (max is 16) that will be used in this filter                 */
    bool     state;    /**> Records the previous state of the filter, used to implement total hysteresis
                            (you need all bits to 1 or 0 to flip the state) */
} logand_buffer16_t;

/**
 * @brief performs a logical "and" over boolean values.
 * This function works on a buffer which has N bits length (from 1 to 16 bits)
 * and only turns on or off when reaching one of the extremes (all bits set to 1 or 0).
 * The previous state is persisted along the whole transition from one end to the other (total hysteresis)
 */
bool logand_16_push(logand_buffer16_t* const buffer, bool value);

#ifdef __cplusplus
}
#endif

#endif /* LOGICAL_AND_HEADER */