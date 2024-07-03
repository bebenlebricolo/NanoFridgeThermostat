#ifndef BITMAP_HEADER
#define BITMAP_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>


/**
 * @brief small bitmap structure. Data is layed out bit by bit, in uint8_t data chunks.
 * This means that in order to access a single pixel, we need to address the right byte, then bitmask the right bit inside the 8 bits.
 */
typedef struct
{
    uint8_t* data;      /**> image data organized as [row 0 pixels], [row 1 pixels] .... [row N pixels] */
    uint8_t width;      /**> width in pixel     */
    uint8_t height;     /**> height in pixel    */
} bitmap_t;


#ifdef __cplusplus
}
#endif

#endif /* BITMAP_HEADER */