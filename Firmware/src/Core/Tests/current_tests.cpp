#include <cmath>

#include <gtest/gtest.h>
#include "Sensors/current.h"

class CurrentFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

TEST_F(CurrentFixture, current_from_voltage_test)
{
    int16_t reading_mv = 2000;
    int16_t current_ma = 0;
    current_from_voltage(&reading_mv, &current_ma);

    // 0,1V/amp -> 2V = 20Amps
    // But Opamp gain is 22, so 2V -> 0,9 amps.
    ASSERT_EQ(current_ma, 909);

    reading_mv = 353;
    current_from_voltage(&reading_mv, &current_ma);
    ASSERT_EQ(current_ma, 160);

    reading_mv = 13;
    current_from_voltage(&reading_mv, &current_ma);
    ASSERT_EQ(current_ma, 5);
}

TEST_F(CurrentFixture, current_compute_rms_sine_test)
{
    constexpr double magnitude = 1000;

    // We are filling the internal buffer with values from a sinusoidal waveform
    for(unsigned int i = 0; i < CURRENT_MEASURE_SAMPLES_PER_SINE ; i++)
    {
        double theta = 2 * M_PI * i / (CURRENT_MEASURE_SAMPLES_PER_SINE - 1);
        int16_t current_ma = (int16_t) ((1 + sin(theta)) * magnitude);
        int16_t out = 0;
        current_compute_rms_sine(&current_ma, &out);
        (void) out;
    }

    // Then we try to measure the actual value
    int16_t current_ma = (int16_t) (magnitude);
    int16_t out = 0;
    current_compute_rms_sine(&current_ma, &out);

    int16_t expected = (int16_t) ( magnitude / sqrt(2));

    ASSERT_NEAR(out, expected , 10);

}

#if CURRENT_RMS_ARBITRARY_FCT
TEST_F(CurrentFixture, current_compute_rms_arbitrary_test)
{
    constexpr double magnitude = 1000;
    constexpr int16_t offset = (int16_t)(magnitude);

    // We are filling the internal buffer with values from a sinusoidal waveform
    for(unsigned int i = 0; i < CURRENT_MEASURE_SAMPLES_PER_SINE ; i++)
    {
        double theta = 2 * M_PI * i / (CURRENT_MEASURE_SAMPLES_PER_SINE - 1);
        int16_t current_ma = (int16_t) ((1 + sin(theta)) * magnitude);
        int16_t out = 0;
        current_compute_rms_arbitrary(&current_ma, &out, &offset);
        (void) out;
    }

    // Then we try to measure the actual value
    int16_t current_ma = (int16_t) (magnitude);
    int16_t out = 0;
    current_compute_rms_arbitrary(&current_ma, &out, &offset);

    int16_t expected = (int16_t) ( magnitude / sqrt(2));

    ASSERT_NEAR(out, expected , 20);
}
#endif /* CURRENT_RMS_ARBITRARY_FCT */


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}