#ifndef BRIDGE_HEADER
#define BRIDGE_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * @brief computes lower bridge resistance based on upper resistance value and voltage (milli volt)
 * @param[in] upper_resistance : upper bridge resistance value (don't need to specify unit here)
 * @param[in] voltage_mv       : voltage reading above the lower resistance of the bridge (in millivolt)
 * @param[in] vcc_mv           : input vcc voltage of the resistor bridge (in millivolt)
 * @return calculated resistance, using the same unit as input upper resistance parameter (Ω, kΩ, MΩ)
*/
uint16_t bridge_get_lower_resistance(uint16_t const * const upper_resistance, uint16_t const * const voltage_mv, uint16_t const * const vcc_mv);


#ifdef __cplusplus
}
#endif

#endif /* BRIDGE_HEADER */