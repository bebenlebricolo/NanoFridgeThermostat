#include <gtest/gtest.h>

#include "bridge.h"

class BridgeFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(BridgeFixture, bridge_calculate_lower_resistance)
{
    uint16_t upper_resistance = 330;
    uint16_t voltage_milli = 1500;
    const uint16_t vcc = 5000;

    uint16_t result = bridge_get_lower_resistance(&upper_resistance, &voltage_milli, &vcc);
    ASSERT_EQ(result, 141);
    uint16_t expected = (vcc * result)/(upper_resistance + result);
    ASSERT_NEAR(voltage_milli, expected, 10);

    voltage_milli = 0;
    result = bridge_get_lower_resistance(&upper_resistance, &voltage_milli, &vcc);
    ASSERT_EQ(result, 0);

    voltage_milli = vcc;
    result = bridge_get_lower_resistance(&upper_resistance, &voltage_milli, &vcc);
    ASSERT_EQ(result, UINT16_MAX);

}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}