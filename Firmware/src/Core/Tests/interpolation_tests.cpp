#include <gtest/gtest.h>

#include "interpolation.h"

class InterpolationFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(InterpolationFixture, check_value_range_positive_range)
{
    uint16_t value = 20;
    range_uint16_t range;
    range.start = 19;
    range.end = 25;

    interpolation_range_check_t result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_INCLUDED);

    value = range.start;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_INCLUDED);

    value = range.end;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_INCLUDED);

    value = range.start - 1;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_LEFT);

    value = range.end + 1;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_RIGHT);
}

TEST_F(InterpolationFixture, check_value_range_negative_range)
{
    uint16_t value = 20;
    range_uint16_t range;
    range.start = 25;
    range.end = 19;

    interpolation_range_check_t result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_INCLUDED);

    value = range.start;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_INCLUDED);

    value = range.end;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_INCLUDED);

    value = range.start + 1;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_LEFT);

    value = range.end - 1;
    result = interpolation_check_value_range_uint16(&value, &range);
    ASSERT_EQ(result, RANGE_CHECK_RIGHT);
}

TEST_F(InterpolationFixture, linear_interpolate_uint16_to_int8_reversed_ranges)
{
    uint16_t value = 10;

    // Emulates resistance of an NTC thermistor
    range_uint16_t input = {
        .start = 1200,
        .end = 1000
    };

    // Emulates temperatures (rising temps with decreasing resistance)
    range_int8_t output = {
        .start = -23,
        .end = -20
    };

    int8_t result = interpolation_linear_uint16_to_int8(&value, &input, &output);
    ASSERT_EQ(result, output.end);

    value = 1300;
    result = interpolation_linear_uint16_to_int8(&value, &input, &output);
    ASSERT_EQ(result, output.start);

    value = 1100;
    result = interpolation_linear_uint16_to_int8(&value, &input, &output);
    ASSERT_EQ(result, -22);
}

TEST_F(InterpolationFixture, linear_interpolate_uint16_to_int8_same_signs_ranges)
{
    uint16_t value = 1050;

    // Emulates resistance of an NTC thermistor
    range_uint16_t input = {
        .start = 1000,
        .end = 1200
    };

    // Emulates temperatures (rising temps with decreasing resistance)
    range_int8_t output = {
        .start = 5,
        .end = 10
    };

    int8_t result = interpolation_linear_uint16_to_int8(&value, &input, &output);
    ASSERT_EQ(result, 6);

    value = 1100;
    result = interpolation_linear_uint16_to_int8(&value, &input, &output);
    ASSERT_EQ(result, 7);

    value = 1150;
    result = interpolation_linear_uint16_to_int8(&value, &input, &output);
    ASSERT_EQ(result, 8);

    value = 1180;
    result = interpolation_linear_uint16_to_int8(&value, &input, &output);
    ASSERT_EQ(result, 9);
}

TEST_F(InterpolationFixture, linear_interpolate_uint8_to_uint8_same_signs_ranges)
{
    uint8_t value = 34;

    // Emulates resistance of an NTC thermistor
    range_uint8_t input = {
        .start = 0,
        .end = 100
    };

    // Emulates temperatures (rising temps with decreasing resistance)
    range_uint8_t output = {
        .start = 0,
        .end = 39
    };

    uint8_t result = interpolation_linear_uint8_to_uint8(value, &input, &output);
    ASSERT_EQ(result, 13);

    value = 3;
    result = interpolation_linear_uint8_to_uint8(value, &input, &output);
    ASSERT_EQ(result, 1);

    value = 7;
    result = interpolation_linear_uint8_to_uint8(value, &input, &output);
    ASSERT_EQ(result, 2);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}