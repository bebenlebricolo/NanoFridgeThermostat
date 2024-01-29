#include "bridge.h"

uint16_t bridge_get_lower_resistance(uint16_t const * const upper_resistance, uint16_t const * const voltage_mv, uint16_t const * const vcc_mv)
{

    uint16_t result = (*upper_resistance * *voltage_mv) / (*vcc_mv - *voltage_mv);
    return result;
}