#ifndef CURRENT_HEADER
#define CURRENT_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define CURRENT_MEASURE_SAMPLES_PER_SINE 20U
#define CURRENT_MEASURE_GAIN 22
#define CURRENT_TRANSFORMER_INV_RATIO 10  /**> Current Transformer has a 1000:1 turn ratio, with a 0.1V/1A spec, so invert that*/
#define CURRENT_RMS_ARBITRARY_FCT 0
/**
 * @brief Computes current RMS over a sliding window (N last samples, @see CURRENT_MEASURE_SAMPLES_PER_SINE)
 * @param[in]   current_ma  : current reading in milliamperes
 * @param[out]  out_rms_ma  : output RMS reading in milliamperes
*/
void current_compute_rms_sine(int16_t const * const current_ma, int16_t * const out_rms_ma);

#if CURRENT_RMS_ARBITRARY_FCT

/**
 * @brief computes the RMS current value for an arbitrary waveform.
 * That's useful for waveforms that are not exact sine waves
 * @param[in]  current_ma : input current (milliamps)
 * @param[out] out_rms_ma : output rms current (milliamps)
 * @param[in]  dc_offset  : DC offset to be removed from the actual RMS value (we don't care for DC if only AC is the target)
*/
void current_compute_rms_arbitrary(int16_t const * const current_ma, int16_t * const out_rms_ma, int16_t const * const dc_offset);

#endif

/**
 * @brief Converts a voltage (as per read by the microcontroller) into a current reading (milliamperes)
 * @param[in]   reading_mv      : input voltage (millivolts), read from current transformer + dual staged amplifier
 * @param[out]  out_current_ma  : converted result in milliamperes.
*/
void current_from_voltage(int16_t const * const reading_mv, int16_t * const out_current_ma);

// Out data size should be at least CURRENT_MEASURE_SAMPLES_PER_SINE.
void current_export_internal_data(int16_t (* out_data)[CURRENT_MEASURE_SAMPLES_PER_SINE]);


#ifdef __cplusplus
}
#endif

#endif /* CURRENT_HEADER */