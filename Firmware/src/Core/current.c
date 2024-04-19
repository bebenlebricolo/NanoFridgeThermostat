#include "current.h"

#if CURRENT_RMS_ARBITRARY_FCT
static void int_sqrt(int32_t const* const input, int32_t* const out);
#endif

static int16_t data[CURRENT_MEASURE_SAMPLES_PER_SINE] = {0};
static uint8_t  index                                  = 0;
static uint8_t  capacity                               = 0;

#define SQRT_2X100 141

void current_from_voltage(int16_t const* const reading_mv, int16_t* const out_current_ma)
{
    // Usually we have a 0.1V for 1A CT with burden resistor couple.
    // Total gain of the 2 stages amp is around 30.
    // Multiplying by 10

    *out_current_ma = (CURRENT_TRANSFORMER_INV_RATIO * *reading_mv) / (CURRENT_MEASURE_GAIN);
}

void current_export_internal_data(int16_t (* out_data)[CURRENT_MEASURE_SAMPLES_PER_SINE])
{
    for(uint8_t i = 0 ; i < CURRENT_MEASURE_SAMPLES_PER_SINE ; i++)
    {
        (*out_data)[i] = data[i];
    }
}

// Note : very naive implementation
void current_compute_rms_sine(int16_t const* const current_ma, int16_t* const out_rms_ma)
{
    uint8_t max_idx = 0;
    uint8_t min_idx = 0;

    // Store new input data in RMS buffer
    data[index] = *current_ma;
    index       = (index + 1) % CURRENT_MEASURE_SAMPLES_PER_SINE;

    if (capacity < CURRENT_MEASURE_SAMPLES_PER_SINE)
    {
        capacity++;
    }

    // Find min and max values in stored buffer
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

    int16_t peak_to_peak = data[max_idx] - data[min_idx];
    int16_t magnitude    = peak_to_peak / 2;

    // Removing alias again on sqrt(2) with small(er) error margin
    *out_rms_ma = (magnitude * 100) / SQRT_2X100;
}

#if CURRENT_RMS_ARBITRARY_FCT
// Note : very naive implementation
void current_compute_rms_arbitrary(int16_t const* const current_ma, int16_t* const out_rms_ma, int16_t const* const dc_offset_current)
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
    *out_rms_ma = (int16_t)ac_rms;
}

// Square root of integer
static void int_sqrt(int32_t const* const input, int32_t* const out)
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