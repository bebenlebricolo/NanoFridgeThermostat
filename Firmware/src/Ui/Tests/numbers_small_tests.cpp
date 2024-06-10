#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include "Ui/numbers_small.h"
#include "Ui/image_buffer.h"

class NumbersSmallFIxture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(NumbersSmallFIxture, temp_rendering_test)
{
    int8_t temp = 23;
    const uint8_t WIDTH = 16U/8U*3U;
    const uint8_t HEIGHT = 18U;

    uint8_t data[WIDTH][HEIGHT] = {0};
    image_buffer_t buffer;
    buffer.data = (uint8_t *) data;
    buffer.width = WIDTH;
    buffer.height = HEIGHT;

    draw_temperature(temp, &buffer, true);

    std::cout << "col(px)\t";
    for(uint8_t i = 0 ; i < WIDTH * 8U ; i++)
    {
        std::cout << std::to_string(i) << " ";
        if((i<10))
        {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    for(uint8_t i = 0; i < HEIGHT ; i++)
    {
        std::cout << "line" << std::to_string(i) << "\t";
        for (uint8_t j=0; j < WIDTH ; j++)
        {
            uint8_t tmp = data[j][i];
            for(uint8_t k = 0 ; k < 8; k++)
            {
                uint8_t bit = tmp & 1 << (7 - k);
                if(bit)
                {
                    std::cout << "#  ";
                }
                else
                {
                    std::cout << "•  ";
                }
                tmp >>= 1;
            }
        }
        std::cout << std::endl;
    }
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}