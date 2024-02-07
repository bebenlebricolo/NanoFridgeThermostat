#ifndef INTERPOLATION_HEADER
#define INTERPOLATION_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdint.h>

/**
 * @brief Enum used to check if a value is contained within a range or not.
 * Note that we use the "Right" or "Left" convention, as this method can adapt to ranges which are reversed (start value is greater than end value).
 * This is done this way in order to support decreasing curves as well as increasing curves.
 *
*/
typedef enum
{
    RANGE_CHECK_INCLUDED,   /**> Value is included within the range             */
    RANGE_CHECK_LEFT,       /**> Value is outside the range, before its start   */
    RANGE_CHECK_RIGHT       /**> Value is outside the range, past the end       */
} range_check_t;


/**
 * @brief Numerical range with uint16_t arithmetic
 * Note that ranges can be reversed (start is bigger than end, decreasing range)
*/
typedef struct
{
    uint16_t start;
    uint16_t end;
} range_uint16_t;

/**
 * @brief Numerical range with uint16_t arithmetic
 * Note that ranges can be reversed (start is bigger than end, decreasing range)
*/
typedef struct
{
    int8_t start;
    int8_t end;
} range_int8_t;

/**
 * @brief interpolates the value within the input range into the output range.
 * @param[in] value         : the value that needs to be converted in the output range
 * @param[in] input_range   : input value range
 * @param[in] output_range  : output value range
 * @return int8_t : input value mapped in the output range
*/
int8_t linear_interpolate_uint16_to_int8(uint16_t const * const value,
                                         range_uint16_t const * const input_range,
                                         range_int8_t const * const out_range );

/**
 * @brief Checks whether an input value is contained within the input range (value pair) or not.
 * This function uses the included version of the check : [val1, val2] instead of the excluding check ]val1,val2[.
 * In case the input value is outside of the given range, returned enum indicates on which side the value is located.
 * @return
 *      RANGE_CHECK_INCLUDED : input value is included within the two values range.
 *      RANGE_CHECK_LEFT     : input value is NOT included within the two values range, and is on the left side of the range.
 *      RANGE_CHECK_RIGHT    : input value is NOT included within the two values range, and is on the right side of the range.
*/
range_check_t check_value_range_uint16(uint16_t const * const value, range_uint16_t const * const range);


#ifdef __cplusplus
}
#endif

#endif /* INTERPOLATION_HEADER */