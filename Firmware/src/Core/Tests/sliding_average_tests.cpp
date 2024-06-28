#include <cmath>

#include "Filters/sliding_average.h"
#include <gtest/gtest.h>
#include <vector>

class SlidingAvgTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(SlidingAvgTests, small_check)
{
    const uint8_t MAX_CAPACITY = 20U;
    int8_t data[MAX_CAPACITY] = {0};
    savgi8_t avg;

    savgi8_t_init(&avg);
    avg.buffer.data = data;
    avg.buffer.tot_capacity = MAX_CAPACITY;

    int8_t result = 0;

    for(uint8_t i = 0 ; i < MAX_CAPACITY ; i++)
    {
        sliding_avg_i8_push(&avg, (int8_t)i, &result);
    }
    ASSERT_EQ(result, 9);

    // Do it once more to fill all the array with new data
    const int8_t expected_output = 5;
    for(uint8_t i = 0 ; i < MAX_CAPACITY ; i++)
    {
        sliding_avg_i8_push(&avg, expected_output, &result);
    }
    ASSERT_EQ(result, expected_output);

}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}