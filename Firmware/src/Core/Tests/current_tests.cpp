#include <cmath>

#include <gtest/gtest.h>
#include "current.h"

class CurrentFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(CurrentFixture, current_from_voltage_test)
{
    uint16_t reading_mv = 2000;
    uint16_t current_ma = 0;
    current_from_voltage(&reading_mv, &current_ma);

    // 1V/amp -> 2V = 2Amps
    // But Opamp gain is 10, so 2V -> 0,2 amps.
    ASSERT_EQ(current_ma, 200);

    reading_mv = 353;
    current_from_voltage(&reading_mv, &current_ma);
    ASSERT_EQ(current_ma, 35);

    reading_mv = 13;
    current_from_voltage(&reading_mv, &current_ma);
    ASSERT_EQ(current_ma, 1);
}

TEST_F(CurrentFixture, current_compute_rms_sine_test)
{
    constexpr double magnitude = 1000;

    // We are filling the internal buffer with values from a sinusoidal waveform
    for(unsigned int i = 0; i < CURRENT_MEASURE_SAMPLES_PER_SINE ; i++)
    {
        double theta = 2 * M_PI * i / (CURRENT_MEASURE_SAMPLES_PER_SINE - 1);
        uint16_t current_ma = (uint16_t) ((1 + sin(theta)) * magnitude);
        uint16_t out = 0;
        current_compute_rms_sine(&current_ma, &out);
        (void) out;
    }

    // Then we try to measure the actual value
    uint16_t current_ma = (uint16_t) (magnitude);
    uint16_t out = 0;
    current_compute_rms_sine(&current_ma, &out);

    uint16_t expected = (uint16_t) ( magnitude / sqrt(2));

    ASSERT_NEAR(out, expected , 10);

}

#ifdef CURRENT_RMS_ARBITRARY_FCT
TEST_F(CurrentFixture, current_compute_rms_arbitrary_test)
{
    constexpr double magnitude = 1000;
    constexpr uint16_t offset = (uint16_t)(magnitude);

    // We are filling the internal buffer with values from a sinusoidal waveform
    for(unsigned int i = 0; i < CURRENT_MEASURE_SAMPLES_PER_SINE ; i++)
    {
        double theta = 2 * M_PI * i / (CURRENT_MEASURE_SAMPLES_PER_SINE - 1);
        uint16_t current_ma = (uint16_t) ((1 + sin(theta)) * magnitude);
        uint16_t out = 0;
        current_compute_rms_arbitrary(&current_ma, &out, &offset);
        (void) out;
    }

    // Then we try to measure the actual value
    uint16_t current_ma = (uint16_t) (magnitude);
    uint16_t out = 0;
    current_compute_rms_arbitrary(&current_ma, &out, &offset);

    uint16_t expected = (uint16_t) ( magnitude / sqrt(2));

    ASSERT_NEAR(out, expected , 20);
}
#endif /* CURRENT_RMS_ARBITRARY_FCT */


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}