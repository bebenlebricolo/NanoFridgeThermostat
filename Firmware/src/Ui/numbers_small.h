#ifndef NUMBERS_SMALL_HEADER
#define NUMBERS_SMALL_HEADER

#include "image_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

// All images are aligned on bytes (width pixels is a multiple of 8)
#define NUMBERS_WIDTH_PX    16U
#define NUMBERS_HEIGHT_PX   18U
#define NUMBERS_ARRAY_SIZE  ((NUMBERS_WIDTH_PX / 2U) * NUMBERS_WIDTH_PX)

// All those small pictures are aligned on 8 bits for the width
// Hence the array size matches the number of pixels for the height of the image
// (size is width / 8 * height, which turns to be 1 * height here)
extern const uint8_t sm_3x8_degFsmall   [8U];
extern const uint8_t sm_3x8_degCsmall   [8U];
extern const uint8_t sm_3x3_minus       [1U];
extern const uint8_t sm_3x3_plus        [4U];
extern const uint8_t sm_7x18_degFbig    [18U];
extern const uint8_t sm_7x18_degCbig    [18U];
extern const uint8_t sm_5x15_degF       [16U];
extern const uint8_t sm_5x15_degC       [16U];
extern const uint8_t sm_6x6_minus       [3U];
extern const uint8_t sm_6x6_plus        [7U];

// Those images however are bigger and require more space
extern const uint8_t sm_12x18_nine    [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_eight   [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_seven   [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_six     [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_five    [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_four    [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_three   [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_two     [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_one     [NUMBERS_ARRAY_SIZE];
extern const uint8_t sm_12x18_zero    [NUMBERS_ARRAY_SIZE];


/**
 * @brief draws temperature numbers on buffer.
 * This function does not check for input buffer validity (no NULL comparison).
 * Note that the 'overwrite' parameter allows you to perform a logical OR or to force rewrite every bit.
 * Non overwrite mode : data |= <byte>.
 * Overwrite mode : data = <byte>
 * @param[in]  temp: input temperature (supported theoretical temps range spans from -99 to +100)
 * @param[out] buffer : input buffer where data will be drawn.
*/
void draw_temperature(const int8_t temp, image_buffer_t * const buffer, const bool overwrite);

#ifdef __cplusplus
}
#endif

#endif /* NUMBERS_SMALL_HEADER */