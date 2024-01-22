// Thermistor header
#ifndef THERMISTOR_HEADER
#define THERMISTOR_HEADER

#include<stdint.h>

#define THERMISTOR_MAX_SAMPLES 50U

/**
 * @brief Encodes the actual scaler for the thermistor base resistance
*/
typedef enum
{
    RESUNIT_OHMS,       /**> Ohms               */
    RESUNIT_KILOOHMS,   /**> Kilo Ohms (10³)    */
    RESUNIT_MEGAOHMS    /**> Mega Ohms (10⁶)    */
} resistance_scale_t;

/**
 * @brief Thermistor data base construct (maps a temperature with a resistance)
*/
typedef struct
{
    int8_t            temperature;  /**> Temperature data (ranging from -24°C usually to +25°C, 1 degree increment)                 */
    uint16_t          resistance;   /**> Resistance of the thermistor at the given temperature (unit given below, default is KOhms) */
} temp_res_t;

/**
 * @brief Thermistor data for a given thermistor
*/
typedef struct {
    temp_res_t data[THERMISTOR_MAX_SAMPLES];    /**> (Ordered) array of thermistor data pairing */
    resistance_scale_t unit;                     /**> Resistance scale used*/
} thermistor_data_t;


/**
 * @brief Converts input resistance reading to an actual temperature.
 * Data is linearly extrapolated when resistance value is out of bounds
 * otherwise it's linearly interpolated in between thermistance data points.
*/
int8_t read_temperature(thermistor_data_t const * const data, const uint16_t resistance);

#endif /* THERMISTOR_HEADER */