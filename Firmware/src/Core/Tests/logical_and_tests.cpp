#include <cmath>

#include "Filters/logical_and.h"
#include <gtest/gtest.h>
#include <vector>

class LogicalAndTests : public ::testing::TestWithParam<uint8_t>
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(plop, LogicalAndTests, ::testing::Values(16, 12));

TEST_P(LogicalAndTests, test_flip_flop)
{
    logand_buffer16_t buffer = {
        .data     = 0,
        .index    = 0,
        .max_bits = GetParam(),
        .state    = false,
    };

    for (uint8_t i = 0; i < buffer.max_bits; i++)
    {
        auto result = logand_16_push(&buffer, true);
        if (i < buffer.max_bits - 1)
        {
            ASSERT_FALSE(result);
        }
    }
    ASSERT_TRUE(buffer.state);

    for (uint8_t i = 0; i < buffer.max_bits; i++)
    {
        auto result = logand_16_push(&buffer, false);
        if (i < buffer.max_bits - 1)
        {
            ASSERT_TRUE(result);
        }
    }
    ASSERT_FALSE(buffer.state);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}