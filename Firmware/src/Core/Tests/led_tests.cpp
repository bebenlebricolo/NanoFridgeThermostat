#include <cmath>

#include "led.h"
#include <gtest/gtest.h>

class LedFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        port = 0;
        leds[0] = led_io_t{
            .port = &port,
            .pin = 3,
        };
        leds[1] = led_io_t{
            .port = &port,
            .pin = 5,
        };

        time.milliseconds = 0;
        time.seconds = 0;
    }

    void TearDown() override
    {
        led_reset();
    }

    led_io_t leds[2U];
    mcu_time_t time;
    volatile uint8_t port;
};

TEST_F(LedFixture, led_process_uninitialized_test)
{
    led_process(&time);
    // Should return early : init has not been called and no led was configured

    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    // Initialize internal memory
    led_init(leds, 2U);
}

TEST_F(LedFixture, led_process_initialized_pattern_none_test)
{
    // Initialize internal memory
    led_init(leds, 2U);

    led_process(&time);

    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.milliseconds = 12;
    led_process(&time);

    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.milliseconds = 25;
    led_process(&time);

    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.seconds = LED_BLINK_WARNING_HALF_P;
    led_process(&time);

    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);
}

TEST_F(LedFixture, led_process_pattern_warning_test)
{
    // Initialize internal memory
    led_init(leds, 2U);

    led_set_blink_pattern(0U, LED_BLINK_WARNING);
    led_process(&time);

    // Led 1 should be HIGH now
    ASSERT_NE(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.milliseconds = 12;
    led_process(&time);

    ASSERT_NE(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.seconds = LED_BLINK_WARNING_HALF_P;
    led_process(&time);

    // Led should be off now
    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.seconds = LED_BLINK_WARNING_PERIOD_S;
    led_process(&time);

    // Led should be ON again now
    ASSERT_NE(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    led_set_blink_pattern(0U, LED_BLINK_NONE);
    led_process(&time);

    // This should revert the led back to its off state :
    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);
}

TEST_F(LedFixture, led_process_pattern_breathing_test)
{
    // Initialize internal memory
    led_init(leds, 2U);

    led_set_blink_pattern(0U, LED_BLINK_WARNING);
    led_process(&time);

    // Led 1 should be HIGH now
    ASSERT_NE(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.milliseconds = 12;
    led_process(&time);

    ASSERT_NE(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.seconds = LED_BLINK_WARNING_HALF_P;
    led_process(&time);

    // Led should be off now
    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    time.seconds = LED_BLINK_WARNING_PERIOD_S;
    led_process(&time);

    // Led should be ON again now
    ASSERT_NE(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);

    led_set_blink_pattern(0U, LED_BLINK_NONE);
    led_process(&time);

    // This should revert the led back to its off state :
    ASSERT_EQ(port & (1 << leds[0].pin), 0);
    ASSERT_EQ(port & (1 << leds[1].pin), 0);
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}