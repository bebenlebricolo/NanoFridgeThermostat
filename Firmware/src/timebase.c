#include "timebase.h"
#include "Arduino.h"

// Used to count ticks 100 times a second
volatile static uint8_t ticks = 0;
static uint32_t seconds = 0;

// Using TIMER2 CompA vector to increment the time variable
ISR(TIMER2_COMPA_vect)
{
    ticks++;
}

/**
 * @brief Setups timer 2 as a slow timer (with the biggest prescaler of 1024) and sets the compare value so that the interrupt frequency is around 100Hz.
 */
void timebase_init(void)
{
    TCCR2A = (1 << WGM21); // CTC mode

    // Prescaler of 1024
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);

    // 16MHz / 1024 -> 15625 HZ
    // 15625 / 156 ~= 100,1 HZ -> closest to 100 Hz so we can just "count" up to 100 with a software counter and derive seconds out of this
    // 15625 / 157 ~= 99,5 HZ

    // Will raise an interrupt around 100 times per second
    OCR2A = 155;

    // Enable interrupts for this counter
    TIMSK2 |= (1 << OCIE2A);
    TCNT2 = 0;
}

void timebase_reset(void)
{
    ticks = 0;
    TCNT2 = 0;
    TIMSK2 &= ~(1 << OCIE2A);
    TCCR2A &= ~(1 << WGM21);
    TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    OCR2A = 0;
}


void timebase_get_time(uint32_t * const seconds)
{
    if (ticks >= 100)
    {
        (*seconds)++; // Will overflow in UINT32_MAX seconds, around 136 years.
        ticks = 0;
    }
}