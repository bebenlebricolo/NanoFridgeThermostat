#include <stdint.h>
#include <stdbool.h>


typedef struct
{
    uint8_t** data;         /**> Image data (one bit per pixel, black n' white)
                                 Note :
                                    - if the image width is a multiple of 8 (aligned on uint8_t format) :
                                       -> array size = (width/8) * (height).
                                    - Otherwise, an extra byte needs to be reserved for each pixel line to contain the extra pixels :
                                       -> array size = (width/8 + 1) * (height)
                            */
    const uint8_t width;    /**> Total available width (pixels) */
    const uint8_t height;   /**> Total available height (pixels)*/

    uint8_t used_width;     /**> Actually used width (in pixel) */
    uint8_t used_height;    /**> Actually used height (in pixel)*/
} image_buffer_t;
