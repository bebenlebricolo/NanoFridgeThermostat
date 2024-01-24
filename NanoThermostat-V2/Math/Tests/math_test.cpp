#include <gtest/gtest.h>

#include "interpolation.h"

class MathFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(MathFixture, test)
{
    ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}