#include <gtest/gtest.h>

#include "memory.h"
#include "spanner.h"

class SpannerFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(SpannerFixture, check_span_first_last)
{
    constexpr uint8_t SEQLEN = 10U;
    span_data_t data = {
        .a_val = 3,
        .b_val = 4,
        .a_cnt = 6,
        .b_cnt = 4,
        .kind = SPAN_FIRST
    };

    uint8_t sequence[SEQLEN] = {0};
    span(&data, sequence, SEQLEN);

    for(uint8_t i = 0 ; i < SEQLEN ; i++)
    {
        if(i < data.a_cnt)
        {
            EXPECT_EQ(sequence[i], data.a_val);
        }
        else
        {
            EXPECT_EQ(sequence[i], data.b_val);
        }
    }


    data.kind = SPAN_LAST;
    memset(sequence, 0, SEQLEN);
    span(&data, sequence, SEQLEN);

    for(uint8_t i = 0 ; i < SEQLEN ; i++)
    {
        if(i < data.b_cnt)
        {
            EXPECT_EQ(sequence[i], data.b_val);
        }
        else
        {
            EXPECT_EQ(sequence[i], data.a_val);
        }
    }
}

TEST_F(SpannerFixture, check_span_even)
{
    constexpr uint8_t SEQLEN = 10U;
    span_data_t data = {
        .a_val = 3,
        .b_val = 4,
        .a_cnt = 6,
        .b_cnt = 4,
        .kind = SPAN_EVEN
    };

    uint8_t sequence[SEQLEN] = {0};
    span(&data, sequence, SEQLEN);

    for(uint8_t i = 0 ; i < 8 ; i++)
    {
        if(i % 2 == 0)
        {
            EXPECT_EQ(sequence[i], data.a_val);
        }
        else
        {
            EXPECT_EQ(sequence[i], data.b_val);
        }
    }
    ASSERT_EQ(sequence[SEQLEN - 2], data.a_val);
    ASSERT_EQ(sequence[SEQLEN - 1], data.a_val);



    // Reversing the range, should be the exact opposite
    data.a_cnt = 4;
    data.b_cnt = 6;
    memset(sequence, 0, SEQLEN);
    span(&data, sequence, SEQLEN);
    for(uint8_t i = 0 ; i < 8 ; i++)
    {
        if(i % 2 == 0)
        {
            EXPECT_EQ(sequence[i], data.b_val);
        }
        else
        {
            EXPECT_EQ(sequence[i], data.a_val);
        }
    }
    ASSERT_EQ(sequence[SEQLEN - 2], data.b_val);
    ASSERT_EQ(sequence[SEQLEN - 1], data.b_val);
}

TEST_F(SpannerFixture, check_span_even_corner_cases)
{
    constexpr uint8_t SEQLEN = 10U;
    span_data_t data = {
        .a_val = 3,
        .b_val = 4,
        .a_cnt = SEQLEN,
        .b_cnt = 0,
        .kind = SPAN_EVEN
    };

    // Should fill the sequence with A values
    uint8_t sequence[SEQLEN] = {0};
    span(&data, sequence, SEQLEN);
    for(const auto& value : sequence)
    {
        EXPECT_EQ(value, data.a_val);
    }

    // Should fill the sequence with B values
    memset(sequence, 0, SEQLEN);
    data.b_cnt = 10;
    data.a_cnt = 0;
    span(&data, sequence, SEQLEN);
    for(const auto& value : sequence)
    {
        EXPECT_EQ(value, data.b_val);
    }

    // Should not modify anything in case the input value is crap
    memset(sequence, 0, SEQLEN);
    data.a_cnt = 1;
    span(&data, sequence, SEQLEN);
    for(const auto& value : sequence)
    {
        EXPECT_EQ(value, 0);
    }
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}