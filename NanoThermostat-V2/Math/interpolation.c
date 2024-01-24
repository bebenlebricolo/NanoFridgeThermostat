#include "interpolation.h"
#include <stdbool.h>

range_check_t check_value_range_uint16(uint16_t const * const value, range_uint16_t const * const range)
{
    bool positive_range = range->start < range->end;
    if(positive_range)
    {
        if(*value <= range->start)
        {
            return RANGE_CHECK_LEFT;
        }

        if(*value >= range->end)
        {
            return RANGE_CHECK_RIGHT;
        }
    }
    else
    {
        if(*value >= range->start)
        {
            return RANGE_CHECK_LEFT;
        }

        if(*value <= range->end)
        {
            return RANGE_CHECK_RIGHT;
        }
    }

    return RANGE_CHECK_INCLUDED;
}

int8_t linear_interpolate_uint16_to_int8(uint16_t const * const value, range_uint16_t const * const in, range_int8_t const * const out )
{
    range_check_t checked_value = check_value_range_uint16(value, in);
    switch(checked_value)
    {
        case RANGE_CHECK_LEFT :
            // Clamp data to the start of output range
            return out->start;

        case RANGE_CHECK_RIGHT :
            // Clamp data to the end of output range
            return out->end;

        case RANGE_CHECK_INCLUDED:
        default:
            break;
    }




    return 0;
}
