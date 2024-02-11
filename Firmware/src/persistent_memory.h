#ifndef PERSISTENT_MEMORY_HEADER
#define PERSISTENT_MEMORY_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief this little structure embeds necessay data for
 * the board to permanently store settings and configurations.
 * This is meant to be written in EEPROM, hence it shall be preserved in between power cycles
*/
typedef struct
{
    uint8_t header;             /**> Constant header value. Used with Footer to know if EEPROM has already been written to or is blank (first boot)*/
    int8_t target_temperature;  /**> Target temperature set point. Regular values range from -20 to 25 °Celsius                                    */
    uint16_t current_threshold; /**> Fridge compressor current threshold (milliAmps). Used to discriminate stalled compressor conditions           */
    uint8_t footer;             /**> Constant footer value. Used with Header to know if EEPROM has already been written to or is blank (first boot)*/
} persistent_config_t;

// Individual offsets are computed in order to access single values from the EEPROM if need be
#define PERM_STORE_TGT_TEMP_IDX             offsetof(persistent_config_t, target_temperature)
#define PERM_STORE_CURRENT_THRESHOLD_IDX    offsetof(persistent_config_t, current_threshold)
#define PERM_STORE_HEADER_IDX               offsetof(persistent_config_t, header)
#define PERM_STORE_FOOTER_IDX               offsetof(persistent_config_t, footer)

// Define the EEPROM_START_OFFSET to move the datastructure further in the EEPROM chip
#ifndef EEPROM_START_OFFSET
#define EEPROM_START_OFFSET 0U
#endif

/**
 * @brief Reads the whole configuration structure from EEPROM
*/
void persistent_mem_read_config(persistent_config_t * config);

/**
 * @brief writes new configuration to EEPROM
*/
void persistent_mem_write_config(persistent_config_t const * const config);

/**
 * @brief Checks whether the persistent configuration has already been written to EEPROM or not.
*/
bool persistent_mem_is_first_boot(const uint8_t header_cst, const uint8_t footer_cst);


#ifdef __cplusplus
}
#endif

#endif /* PERSISTENT_MEMORY_HEADER */