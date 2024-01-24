#include "thermistor.h"
#include "interpolation.h"

#include <stddef.h>

range_check_t frame_value(thermistor_data_t const * const thermistor, uint16_t const * const resistance, temp_res_t const ** low, temp_res_t const ** high)
{
    // Clamp resistance to the lowest resistance found in the curve (aka highest temperature)
    if(*resistance < thermistor->data[thermistor->sample_count - 1].resistance)
    {
        *low = &(thermistor->data[thermistor->sample_count - 1]);
        *high = *low;
        return RANGE_CHECK_RIGHT;
    }

    // Clamp resistance to the highest resistance found in the curve (aka lowest temperature)
    if(*resistance > thermistor->data[0].resistance)
    {
        *low = &(thermistor->data[0]);
        *high = *low;
        return RANGE_CHECK_LEFT;
    }

    // Linear search
    *low = &(thermistor->data[thermistor->sample_count - 1]);
    *high = &(thermistor->data[0]);
    for(uint8_t i = 0 ; i < thermistor->sample_count; i++)
    {
        if(*resistance >= thermistor->data[i].resistance)
        {
            *low = &(thermistor->data[i + 1]);
            *high = &(thermistor->data[i]);
            break;
        }
    }

    return RANGE_CHECK_INCLUDED;
}

int8_t read_temperature(thermistor_data_t const * const thermistor, uint16_t const * const resistance)
{
    int8_t result = 0;

    // Find boundaries of resistance within the lookup table
    temp_res_t const * low = NULL;
    temp_res_t const * high = NULL;

    range_check_t check = frame_value(thermistor, resistance, &low, &high);
    if(check != RANGE_CHECK_INCLUDED)
    {
        return low->temperature;
    }

    range_int8_t temp_range = {
        .start = low->temperature,
        .end = high->temperature
    };
    range_uint16_t res_range = {
        .start = low->resistance,
        .end = high->resistance
    };

    // Then linearly interpolate the value within the boundaries
    result = linear_interpolate_uint16_to_int8(resistance, &res_range, &temp_range);
    return result;
}