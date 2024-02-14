#include "timebase.h"
#include "Arduino.h"

#define TCCR2B_PRESCALER_VALUE (1 << CS22) | (1 << CS20)

// Used to count ticks 100 times a second
volatile static uint16_t milliseconds = 0;

static timebase_time_t internal_time = {
    .seconds = 0,
    .milliseconds = 0,
};

// Using TIMER2 CompA vector to increment the time variable
ISR(TIMER2_COMPA_vect)
{
    milliseconds++;
}

/**
 * @brief Setups timer 2 as a slow timer (with the biggest prescaler of 1024) and sets the compare value so that the interrupt frequency is around 100Hz.
 */
void timebase_init(void)
{
    TCCR2A = (1 << WGM21); // CTC mode

    // Prescaler of 128
    //TCCR2B = (1 << CS22) | (1 << CS21); //| (1 << CS20);
    TCCR2B |= TCCR2B_PRESCALER_VALUE;

    // 16MHz / 128 -> 125kHz HZ
    // 125kHz/ 25 = 5 kHz

    // Will raise an interrupt around 100 times per second
    OCR2A = 124;

    // Enable interrupts for this counter
    TIMSK2 |= (1 << OCIE2A);
    TCNT2 = 0;
}

void timebase_reset(void)
{
    milliseconds = 0;
    TCNT2 = 0;
    TIMSK2 &= ~(1 << OCIE2A);
    TCCR2A &= ~(1 << WGM21);
    TCCR2B &= ~(TCCR2B_PRESCALER_VALUE);
    OCR2A = 0;
}


void timebase_process(void)
{
    if (milliseconds >= 1000U)
    {
        internal_time.seconds++;
        milliseconds = 0;
    }
    internal_time.milliseconds = milliseconds;
}

const timebase_time_t * timebase_get_time(void)
{
    return &internal_time;
}