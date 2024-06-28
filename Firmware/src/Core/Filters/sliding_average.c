#include "sliding_average.h"
#include <stddef.h>

void savgi8_t_init(savgi8_t * const avg_data)
{
    avg_data->buffer.data = NULL;
    avg_data->buffer.elem_count = 0;
    avg_data->buffer.index = 0;
    avg_data->buffer.tot_capacity = 0;
    avg_data->sum = 0;
}

void sliding_avg_i8_push(savgi8_t * const avg_data, const int8_t new_data, int8_t * const out)
{
    if(avg_data->buffer.elem_count > 0)
    {
        avg_data->sum -= avg_data->buffer.data[avg_data->buffer.index];
    }

    avg_data->buffer.data[avg_data->buffer.index] = new_data;
    avg_data->sum += new_data;
    avg_data->buffer.index++;
    avg_data->buffer.index %= avg_data->buffer.tot_capacity;

    if(avg_data->buffer.elem_count < avg_data->buffer.tot_capacity)
    {
        avg_data->buffer.elem_count++;
    }

    *out = avg_data->sum / avg_data->buffer.elem_count;
}
