#include "current.h"

#if CURRENT_RMS_ARBITRARY_FCT
static void int_sqrt(uint32_t const* const input, uint32_t* const out);
#endif

static uint16_t data[CURRENT_MEASURE_SAMPLES_PER_SINE] = {0};
static uint8_t  index                                  = 0;
static uint8_t  capacity                               = 0;

void current_from_voltage(uint16_t const* const reading_mv, uint16_t* const out_current_ma)
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
void current_compute_rms_sine(uint16_t const* const current_ma, uint16_t* const out_rms_ma)
{
    uint8_t max_idx = 0;
    uint8_t min_idx = 0;

    data[index] = *current_ma;
    index       = (index + 1) % CURRENT_MEASURE_SAMPLES_PER_SINE;

    if (capacity < CURRENT_MEASURE_SAMPLES_PER_SINE)
    {
        capacity++;
    }

    for (uint8_t i = 0; i < capacity; i++)
    {
        if (data[i] > data[max_idx])
        {
            max_idx = i;
        }
        if (data[i] < data[min_idx])
        {
            min_idx = i;
        }
    }

    uint16_t peak_to_peak = data[max_idx] - data[min_idx];
    uint16_t magnitude    = peak_to_peak / 2U;

    // Removing alias again on sqrt(2) with small(er) error margin
    *out_rms_ma = (magnitude * 100U) / 141U;
}

#if CURRENT_RMS_ARBITRARY_FCT
// Note : very naive implementation
void current_compute_rms_arbitrary(uint16_t const* const current_ma, uint16_t* const out_rms_ma, uint16_t const* const dc_offset_current)
{
    // Circular buffer
    data[index] = *current_ma;
    index       = (index + 1) % CURRENT_MEASURE_SAMPLES_PER_SINE;
    capacity    = capacity < CURRENT_MEASURE_SAMPLES_PER_SINE ? capacity + 1 : capacity;

    if (capacity == 0)
    {
        return;
    }

    uint32_t sum = 0;
    for (uint8_t i = 0; i < capacity; i++)
    {
        sum += data[i] * data[i];
    }
    uint32_t intermediate       = (sum / capacity);
    uint32_t global_rms_current = 0;
    int_sqrt(&intermediate, &global_rms_current);

    // RMS(AC + DC) = SQRT(DC_CURRENT² + RMS(AC)²)
    // RMS(AC)² = RMS(AC+DC)² - DC_CURRENT²
    // RMS(AC) = SQRT(RMS(AC+DC)² - DC_CURRENT²)

    uint32_t ac_rms = 0;
    intermediate    = (global_rms_current * global_rms_current) - (uint32_t)(*dc_offset_current * *dc_offset_current);
    int_sqrt(&intermediate, &ac_rms);
    *out_rms_ma = (uint16_t)ac_rms;
}

// Square root of integer
static void int_sqrt(uint32_t const* const input, uint32_t* const out)
{
    // Zero yields zero
    // One yields one
    if (*input <= 1)
    {
        *out = *input;
        return;
    }

    // Initial estimate (must be too high)
    uint32_t x0 = *input / 2;

    // Update
    uint32_t x1 = (x0 + *input / x0) / 2;

    while (x1 < x0) // Bound check
    {
        x0 = x1;
        x1 = (x0 + *input / x0) / 2;
    }
    *out = x0;
}
#endif