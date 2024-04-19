#include "buffers.h"

void circular_buffer_init(circular_buffer_t* const buffer, const int16_t val)
{
    for (uint8_t i = 0; i < CIRCULAR_BUFFER_SIZE; i++)
    {
        buffer->data[i] = val;
    }
    buffer->index = 0;
}

void circular_buffer_push_back(circular_buffer_t* const buffer, const int16_t val)
{
    buffer->data[buffer->index] = val;
    buffer->index               = (buffer->index + 1) % CIRCULAR_BUFFER_SIZE;
}

void circular_buffer_get_last(circular_buffer_t const * const buffer, int16_t * const val)
{
    uint8_t prev_index = buffer->index == 0 ? (CIRCULAR_BUFFER_SIZE - 1) : buffer->index - 1;
    *val = buffer->data[prev_index];
}
