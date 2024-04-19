#ifndef BUFFERS_HEADER
#define BUFFERS_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CIRCULAR_BUFFER_SIZE 20U

typedef struct
{
    int16_t data[CIRCULAR_BUFFER_SIZE];
    uint8_t index;
} circular_buffer_t;

/**
 * @brief initializes members of the targeted circular buffer
 * @param[in/out] buffer : targeted buffer
 * @param[in]     val    : initialization value (commonly 0)
*/
void circular_buffer_init(circular_buffer_t* const buffer, const int16_t val);

/**
 * @brief adds a new value at the end of the data buffer (overwrites the previous value that was there).
 * When end of the buffer is reached, loops back to the beginning of the buffer
 * @param[in/ou]    buffer : targeted buffer
 * @param[in]       val    : new value
*/
void circular_buffer_push_back(circular_buffer_t* const buffer, const int16_t val);

/**
 * @brief retrieves the last value that was stored in the buffer
 * @note this won't work if the buffer is empty or just been created (will return the n-1 value, which is nonsense in that context)
 * @param[in]   buffer : targeted buffer
 * @param[out]  val    : output value
*/
void circular_buffer_get_last(circular_buffer_t const * const buffer, int16_t * const val);

#ifdef __cplusplus
}
#endif

#endif /* BUFFERS_HEADER */