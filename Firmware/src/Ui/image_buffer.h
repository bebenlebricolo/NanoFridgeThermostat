#ifndef IMAGE_BUFFER_HEADER
#define IMAGE_BUFFER_HEADER

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    uint8_t* data;          /**> Image data (one bit per pixel, black n' white)
                                 Note :
                                    - if the image width is a multiple of 8 (aligned on uint8_t format) :
                                       -> array size = (width/8) * (height).
                                    - Otherwise, an extra byte needs to be reserved for each pixel line to contain the extra pixels :
                                       -> array size = (width/8 + 1) * (height)
                            */
    uint8_t width;          /**> Total available width (pixels) */
    uint8_t height;         /**> Total available height (pixels)*/

    uint8_t used_width;     /**> Actually used width (in pixel) */
    uint8_t used_height;    /**> Actually used height (in pixel)*/
} image_buffer_t;

#ifdef __cplusplus
}
#endif

#endif /* IMAGE_BUFFER_HEADER */
