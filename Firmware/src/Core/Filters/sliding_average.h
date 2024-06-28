#ifndef SLIDING_AVG_HEADER
#define SLIDING_AVG_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct
{
    struct
    {
        int8_t* data;
        uint8_t tot_capacity;
        uint8_t elem_count;
        uint8_t index;
    } buffer;
    int16_t sum;
} savgi8_t;


void savgi8_t_init(savgi8_t * const avg_data);

void sliding_avg_i8_push(savgi8_t * const avg_data, const int8_t new_data, int8_t * const out);

#ifdef __cplusplus
}
#endif

#endif /* SLIDING_AVG_HEADER */