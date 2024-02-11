#include "buttons.h"

#define LOW 0U
#define HIGH 0xFF

void button_local_mem_default(button_local_mem_t *button)
{
    button->current = LOW;
    button->previous = LOW;
    button->pressed = 0;
    button->event = BUTTON_EVENT_DEFAULT;
}

void read_single_button_event(button_local_mem_t *const button, uint32_t const *const time)
{
    if (button->current == HIGH)
    {
        // Start of long press, count press duration
        if (button->current == button->previous)
        {
            const uint32_t duration = *time - button->pressed;
            if (duration >= BUTTON_PRESS_TIME_THRESHOLD)
            {
                button->event = BUTTON_EVENT_HOLD;
            }
        }
        // New button pressed event (was not pressed before)
        else
        {
            button->event = BUTTON_EVENT_PRESSED;
            button->pressed = *time;
        }
    }
    // Button is released
    else
    {
        button->event = BUTTON_EVENT_RELEASED;
    }
    button->previous = button->current;
}

