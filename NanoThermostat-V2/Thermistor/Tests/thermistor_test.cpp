#include <gtest/gtest.h>

#include "thermistor.h"
#include "thermistor_ntc_100k_3950K.h"

class ThermistorFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(ThermistorFixture, test)
{
    ASSERT_TRUE(true);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}