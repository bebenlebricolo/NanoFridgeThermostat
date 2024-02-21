#ifndef CURRENT_HEADER
#define CURRENT_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define CURRENT_MEASURE_SAMPLES_PER_SINE 20U
#define CURRENT_MEASURE_GAIN 10U

/**
 * @brief Computes current RMS over a sliding window (N last samples, @see CURRENT_MEASURE_SAMPLES_PER_SINE)
 * @param[in]   current_ma  : current reading in milliamperes
 * @param[out]  out_rms_ma  : output RMS reading in milliamperes
*/
void current_compute_rms(uint16_t const * const current_ma, uint16_t * const out_rms_ma);


/**
 * @brief Converts a voltage (as per read by the microcontroller) into a current reading (milliamperes)
 * @param[in]   reading_mv      : input voltage (millivolts), read from current transformer + dual staged amplifier
 * @param[out]  out_current_ma  : converted result in milliamperes.
*/
void current_from_voltage(uint16_t const * const reading_mv, uint16_t * const out_current_ma);


#ifdef __cplusplus
}
#endif

#endif /* CURRENT_HEADER */