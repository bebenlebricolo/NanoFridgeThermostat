#include "logical_and.h"

void sanitize_16(logand_buffer16_t * const buffer)
{
    // Clamp max bits to avoid overflows
    if(buffer->max_bits >= LOGAND_MAX_BITS)
    {
        buffer->max_bits = LOGAND_MAX_BITS;
    }

    // Loop back index
    if(buffer->index >= buffer->max_bits)
    {
        buffer->index %= buffer->max_bits;
    }
}

bool logand_16_push(logand_buffer16_t * const buffer, bool value)
{
    uint8_t bit = value != 0 ? 1 : 0;
    buffer->data = (buffer->data & ~(1 << buffer->index)) | (bit << buffer->index);
    buffer->index = (buffer->index + 1) % buffer->max_bits;

    if(buffer->state == true && buffer->data == 0)
    {
        buffer->state = false;
    }

    const uint16_t mask = (uint16_t)((uint32_t)(1 << (buffer->max_bits)) - 1);
    if(buffer->state == false && buffer->data == mask)
    {
        buffer->state = true;
    }

    return buffer->state;
}
