#include "numbers_small.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define IBASE 10
#define TEMP_ITOA_MAX_DIGIT 2U

// 'degFsmall', 8x8px
const uint8_t sm_3x8_degFsmall [] PROGMEM = {
    0x40, 0xa0, 0x40, 0x00, 0xe0, 0x80, 0xc0, 0x80
};
// 'degCsmall', 8x8px
const uint8_t sm_3x8_degCsmall [] PROGMEM = {
    0x40, 0xa0, 0x40, 0x00, 0x60, 0x80, 0x80, 0x60
};
// '-_2small', 8x1px
const uint8_t sm_3x3_minus [] PROGMEM = {
    0xe0
};
// '+_2small', 8x3px
const uint8_t sm_3x3_plus [] PROGMEM = {
    0x40, 0xe0, 0x40
};
// 'degFbig', 8x18px
const uint8_t sm_7x18_degFbig [] PROGMEM = {
    0x7c, 0xfe, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c, 0x00, 0xfe, 0xfe, 0xc0, 0xc0, 0xf8, 0xf8, 0xc0, 0xc0,
    0xc0, 0xc0
};
// 'degCbig', 8x18px
const uint8_t sm_7x18_degCbig [] PROGMEM = {
    0x7c, 0xfe, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c, 0x00, 0x3e, 0x7e, 0x60, 0xe0, 0xc0, 0xc0, 0xe0, 0x60,
    0x7e, 0x3e
};
// 'degF', 8x15px
const uint8_t sm_5x15_degF [] PROGMEM = {
    0x70, 0xd8, 0x88, 0xd8, 0x70, 0x00, 0xf8, 0xf8, 0xc0, 0xf0, 0xf0, 0xc0, 0xc0, 0xc0, 0xc0
};
// 'degC', 8x15px
const uint8_t sm_5x15_degC [] PROGMEM = {
    0x70, 0xd8, 0x88, 0xd8, 0x70, 0x00, 0x78, 0xf8, 0xe0, 0xc0, 0xc0, 0xc0, 0xe0, 0xf8, 0x78
};

#define SM_6X6_MINUS_WIDTH_BYTES 1U
#define SM_6X6_MINUS_HEIGH_BYTES 2U

// '-_1', 8x2px
const uint8_t sm_6x6_minus [] PROGMEM = {
    0xfc, 0xfc
};

#define SM_6X6_PLUS_WIDTH_BYTES 1U
#define SM_6X6_PLUS_HEIGH_BYTES 6U

// '+_1', 8x6px
const uint8_t sm_6x6_plus [] PROGMEM = {
    0x30, 0x30, 0xfc, 0xfc, 0x30, 0x30
};

// '9', 16x18px
const uint8_t sm_12x18_nine [] PROGMEM = {
    0x1f, 0xc0, 0x7f, 0xe0, 0x70, 0xe0, 0xe0, 0x60, 0xe0, 0x70, 0xe0, 0x70, 0xe0, 0x70, 0xf0, 0xf0,
    0xff, 0xf0, 0x7f, 0xf0, 0x1f, 0x30, 0x00, 0x30, 0x00, 0x30, 0xc0, 0x30, 0xe0, 0x70, 0xf0, 0xe0,
    0x7f, 0xe0, 0x3f, 0xc0
};
// '8', 16x18px
const uint8_t sm_12x18_eight [] PROGMEM = {
    0x3f, 0xc0, 0x7f, 0xe0, 0xf0, 0xf0, 0xe0, 0x70, 0xe0, 0x70, 0xf0, 0xf0, 0xff, 0xf0, 0x7f, 0xe0,
    0x3f, 0xc0, 0x7f, 0xe0, 0xf0, 0xf0, 0xe0, 0x70, 0xe0, 0x70, 0xe0, 0x70, 0xf0, 0xf0, 0xff, 0xf0,
    0x7f, 0xe0, 0x1f, 0x80
};
// '7', 16x18px
const uint8_t sm_12x18_seven [] PROGMEM = {
    0xff, 0xf0, 0xff, 0xf0, 0x00, 0x30, 0x00, 0x70, 0x00, 0xf0, 0x01, 0xe0, 0x03, 0xc0, 0x07, 0x80,
    0x07, 0x00, 0x0f, 0x00, 0x0e, 0x00, 0x1e, 0x00, 0x1c, 0x00, 0x3c, 0x00, 0x38, 0x00, 0x78, 0x00,
    0x70, 0x00, 0x70, 0x00
};
// '6', 16x18px
const uint8_t sm_12x18_six [] PROGMEM = {
    0x07, 0x80, 0x0f, 0x80, 0x1f, 0x00, 0x3c, 0x00, 0x38, 0x00, 0x78, 0x00, 0x7f, 0x00, 0xff, 0xc0,
    0xff, 0xe0, 0xf0, 0xe0, 0xe0, 0x70, 0xe0, 0x70, 0xe0, 0x70, 0xe0, 0x70, 0xf0, 0xf0, 0x7f, 0xe0,
    0x3f, 0xc0, 0x1f, 0x80
};
// '5', 16x18px
const uint8_t sm_12x18_five [] PROGMEM = {
    0xff, 0xf0, 0xff, 0xf0, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0xff, 0x00, 0xff, 0xc0, 0xff, 0xe0,
    0x01, 0xe0, 0x00, 0xf0, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0xf0, 0xe1, 0xe0, 0xff, 0xe0,
    0x7f, 0xc0, 0x3f, 0x80
};
// '4', 16x18px
const uint8_t sm_12x18_four [] PROGMEM = {
    0x07, 0xc0, 0x0f, 0xc0, 0x0f, 0xc0, 0x1f, 0xc0, 0x1d, 0xc0, 0x3d, 0xc0, 0x39, 0xc0, 0x79, 0xc0,
    0x71, 0xc0, 0xf1, 0xc0, 0xe1, 0xc0, 0xff, 0xf0, 0xff, 0xf0, 0xff, 0xf0, 0x01, 0xc0, 0x01, 0xc0,
    0x01, 0xc0, 0x01, 0xc0
};
// '3', 16x18px
const uint8_t sm_12x18_three [] PROGMEM = {
    0x3f, 0xc0, 0x7f, 0xe0, 0xff, 0xf0, 0xf0, 0xf0, 0xe0, 0xf0, 0x00, 0xf0, 0x03, 0xe0, 0x0f, 0xe0,
    0x0f, 0xc0, 0x0f, 0xe0, 0x03, 0xe0, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 0xe1, 0xf0, 0xff, 0xf0,
    0xff, 0xe0, 0x7f, 0xc0
};
// '2', 16x18px
const uint8_t sm_12x18_two [] PROGMEM = {
    0x3f, 0xc0, 0x7f, 0xe0, 0xf0, 0xf0, 0xe0, 0x70, 0xc0, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0xf0,
    0x01, 0xe0, 0x03, 0xe0, 0x07, 0xc0, 0x0f, 0x80, 0x1f, 0x00, 0x3e, 0x00, 0x7c, 0x00, 0xfc, 0x00,
    0xff, 0xf0, 0xff, 0xf0
};
// '1', 16x18px
const uint8_t sm_12x18_one [] PROGMEM = {
    0x07, 0x00, 0x0f, 0x00, 0x1f, 0x00, 0x3f, 0x00, 0x7f, 0x00, 0xff, 0x00, 0xff, 0x00, 0xcf, 0x00,
    0x0f, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x0f, 0x00,
    0xff, 0xf0, 0xff, 0xf0
};

// '0', 12x18px
const uint8_t sm_12x18_zero [] PROGMEM = {
    0x1f, 0x80, 0x7f, 0xe0, 0x70, 0xe0, 0xe0, 0x70, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30,
    0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xc0, 0x30, 0xe0, 0x70, 0x70, 0xe0,
    0x7f, 0xe0, 0x1f, 0x80
};


void draw_sign_sm6x6(const bool is_positive, image_buffer_t * const buffer, const bool overwrite)
{
    // Buffer can be 16x18 px per character to display
    uint8_t* img = is_positive ? sm_6x6_plus :sm_6x6_minus;

    // Pixel offset within the Buffer data
    uint8_t x_offset = 0;
    uint8_t y_offset = 0;
    uint8_t length = is_positive ? SM_6X6_PLUS_HEIGH_BYTES : SM_6X6_MINUS_HEIGH_BYTES ;

    if(is_positive)
    {
        x_offset = 4;
        y_offset = 4;
    }
    else
    {
        x_offset = 4;
        y_offset = 4;
    }

    // Enable sub-byte offset (only needed for the x axis, y axis is pixel per pixel)
    uint8_t x_offset_inbyte = x_offset % 8U;

    // Copy image with pixel-per-pixel offset
    for(uint8_t i = 0U ; i < length ; i++ )
    {
        // No need to realign data left and right
        if(x_offset_inbyte == 0)
        {
            uint8_t* buffer_data = &buffer->data[i + y_offset][x_offset/8];
            if(overwrite)
            {
                *buffer_data = img[i];
            }
            else
            {
                *buffer_data |= img[i];
            }
        }
        else
        {
            // We need to split our img_data in 2 parts, because now it overlaps 2 bytes of buffer->data
            // Note that for now only positive offsets are supported
            // buffer->data[n]| buffer->data[n+1]
            // 000 img        | img 0000
            //  └ left offset         └  right offset
            uint8_t left_img = img[i] >> x_offset_inbyte;
            uint8_t right_img = img[i] << x_offset_inbyte;
            uint8_t* buffer_data_left = &buffer->data[i + y_offset][x_offset/8];
            uint8_t* buffer_data_right = &buffer->data[i + y_offset][(x_offset/8) + 1];
            if(overwrite)
            {
                *buffer_data_left = left_img;
                *buffer_data_right = right_img;
            }
            else
            {
                *buffer_data_left |= left_img;
                *buffer_data_right |= right_img;
            }
        }
    }
}


void draw_temperature(const int8_t temperature, image_buffer_t * const buffer, const bool overwrite)
{
    // 2's complement MSB is the sign bit.
    bool is_positive = (bool) temperature >> 8;

    // Buffer that'll hold the single digits of the input temperature
    // -> temperature = -35
    //    numbers = {3,5} ; is_positive = false;
    uint8_t numbers[TEMP_ITOA_MAX_DIGIT] = {0};
    int8_t tmp = temperature;
    uint8_t idx = 0;
    while(tmp != 0 && idx < TEMP_ITOA_MAX_DIGIT)
    {
        numbers[idx] = temperature % IBASE;
        idx++;
        tmp /= IBASE;
    }

    draw_sign_sm6x6(is_positive, buffer, overwrite);

}

