#include "current.h"



void current_from_voltage(uint16_t const * const reading_mv, uint16_t * const out_current_ma)
{
    // Usually we have a 1V for 1A CT with burden resistor couple.
    // As we are reading millivolt -> current_reading_mv / 1000 gives us
    // current_reading_volts <=> current_reading_amps and as we'd like to read
    // milliamps we need to multiply current_reading_amps : both /1000 and *
    // 1000 cancel each other out. However, we are using a dual stage amplifier
    // to accommodate for lower voltages. So we need to take that gain into
    // account for the final calculation :
    *out_current_ma = *reading_mv / CURRENT_MEASURE_GAIN;
}



// Note : very naive implementation
void current_compute_rms(uint16_t const * const current_ma, uint16_t * const out_rms_ma)
{
    static uint16_t data[CURRENT_MEASURE_SAMPLES_PER_SINE];
    static uint8_t index = 0;

    // Circular buffer
    data[index] = *current_ma;
    index = (index + 1) % CURRENT_MEASURE_SAMPLES_PER_SINE;

    uint16_t max = 0;
    uint16_t min = 0;

    for (uint8_t i = 0; i < CURRENT_MEASURE_SAMPLES_PER_SINE; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
        if (data[i] < min)
        {
            min = data[i];
        }
    }

    uint16_t peak_to_peak = max - min;
    uint16_t magnitude = peak_to_peak / 2U;

    // Removing alias again on sqrt(2) with small(er) error margin
    *out_rms_ma = (magnitude * 10U) / 14U;
}