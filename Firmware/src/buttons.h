#ifndef BUTTONS_HEADER
#define BUTTONS_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#ifndef BUTTON_PRESS_TIME_THRESHOLD
#define BUTTON_PRESS_TIME_THRESHOLD 3U
#endif

typedef enum
{
    BUTTON_STATE_HOLD,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_DEFAULT
} button_state_t;

typedef struct
{
    uint8_t current;      /**> Current button state (read from IO)          */
    uint8_t previous;     /**> Previous button state (used to raise events) */
    uint32_t pressed;     /**> When the button was pressed (using timebase) */
    button_state_t event; /**> Output button state                          */
} button_local_mem_t;

void button_local_mem_default(button_local_mem_t *button);

void read_single_button_event(button_local_mem_t *const button, uint32_t const *const time);

#ifdef __cplusplus
}
#endif

#endif /* BUTTONS_HEADER */