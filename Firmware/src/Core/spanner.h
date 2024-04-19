#ifndef SPANNER_HEADER
#define SPANNER_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
 * @brief Describes how A and B values are arranged in the output sequence
 */
typedef enum
{
    SPAN_FIRST, /**> Packs all A values together at the beginning of the sequence   */
    SPAN_LAST,  /**> Packs all A values together at the end of the sequence         */
    SPAN_EVEN   /**> Tries to evenly distribute A and B values along the sequence   */
} span_type_t;

typedef struct
{
    uint8_t a_val;    /**> A value                                            */
    uint8_t b_val;    /**> B value                                            */
    uint8_t a_cnt;    /**> Desired count of A values in output sequence       */
    uint8_t b_cnt;    /**> Desired count of B values in output sequence       */
    span_type_t kind; /**> Desired distribution kind applied to the sequence  */
} span_data_t;

/**
 * @brief distributes 2 values (A and B) across the whole length of the given sequence.
 * @param[in] data      : constant input data for spanner algorithm
 * @param[out] sequence : output sequence (with the resulting data copied in it)
 * @param[in] length    : total length of sequence array (should match allocated memory length)
 */
void span(const span_data_t * data, uint8_t *sequence, const uint8_t length);

#ifdef __cplusplus
}
#endif

#endif /* SPANNER_HEADER */