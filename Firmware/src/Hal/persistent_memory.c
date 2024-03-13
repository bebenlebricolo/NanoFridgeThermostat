#include "persistent_memory.h"
#include <stdlib.h>

#include <avr/eeprom.h>

void persistent_mem_read_config(persistent_config_t * config)
{
    eeprom_read_block((void *)config, (const void*) EEPROM_START_OFFSET, sizeof(persistent_config_t));
}

void persistent_mem_write_config(persistent_config_t const * const config)
{
    eeprom_write_block((void *) config, (const void*) EEPROM_START_OFFSET, sizeof(persistent_config_t));
}

bool persistent_mem_is_first_boot(const uint8_t header_cst, const uint8_t footer_cst)
{
    const uint8_t eep_header = eeprom_read_byte((const uint8_t*) (EEPROM_START_OFFSET + PERM_STORE_HEADER_IDX));
    const uint8_t eep_footer = eeprom_read_byte((const uint8_t*) (EEPROM_START_OFFSET + PERM_STORE_FOOTER_IDX));

    return (header_cst == eep_header) && (footer_cst == eep_footer);
}