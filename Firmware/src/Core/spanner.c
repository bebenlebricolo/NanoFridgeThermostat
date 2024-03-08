#include "spanner.h"
#include "stdbool.h"
#include "memory.h"

static inline void span_first(const span_data_t * data, uint8_t *sequence, const uint8_t length);
static inline void span_last(const span_data_t * data, uint8_t *sequence, const uint8_t length);
static inline void span_even(const span_data_t * data, uint8_t *sequence, const uint8_t length);


void span(const span_data_t * data, uint8_t *sequence, const uint8_t length)
{
    // Should not happen !
    // Reject input so that we don't run buffer overflows.
    // Works either for too many input data or too few.
    if(data->a_cnt + data->b_cnt != length)
    {
        return;
    }

    switch(data->kind)
    {
        case SPAN_FIRST:
            span_first(data, sequence, length);
            break;

        case SPAN_LAST:
            span_last(data, sequence, length);
            break;

        case SPAN_EVEN:
            span_even(data, sequence, length);
            break;
    }
}


static inline void span_first(const span_data_t * data, uint8_t *sequence, const uint8_t length)
{
    uint8_t index = 0;
    while(index < data->a_cnt)
    {
        sequence[index] = data->a_val;
        index++;
    }

    while(index < length)
    {
        sequence[index] = data->b_val;
        index++;
    }
}

static inline void span_last(const span_data_t * data, uint8_t *sequence, const uint8_t length)
{
    uint8_t index = 0;
    while(index < data->b_cnt)
    {
        sequence[index] = data->b_val;
        index++;
    }

    while(index < length)
    {
        sequence[index] = data->a_val;
        index++;
    }
}

typedef enum
{
    SEL_A,
    SEL_B
} selector_t;

static inline void span_even(const span_data_t * data, uint8_t *sequence, const uint8_t length)
{
    if(data->a_cnt == 0)
    {
        memset(sequence, data->b_val, length);
        return;
    }

    if(data->b_cnt == 0)
    {
        memset(sequence, data->a_val, length);
        return;
    }

    bool reversed = (data->b_cnt >= data->a_cnt);
    uint8_t ratio = true == reversed ?  data->b_cnt / data->a_cnt : data->a_cnt /  data->b_cnt;
    uint8_t index = 0;

    uint8_t const * val = true == reversed ? &data->b_val : &data->a_val;
    uint8_t counter = 0;
    uint8_t counter_a = 0;
    uint8_t counter_b = 0;
    while(index < length)
    {
        sequence[index] = *val;
        counter++;
        if(val == &data->a_val)
        {
            counter_a++;
        }
        else
        {
            counter_b++;
        }

        if(counter % ratio == 0)
        {
            if(val == &data->a_val && counter_b < data->b_cnt)
            {
                val = &data->b_val;
            }
            else if(val == &data->b_val && counter_a < data->a_cnt)
            {
                val = &data->a_val;
            }
            counter = 0;
        }

        index++;
    }
}