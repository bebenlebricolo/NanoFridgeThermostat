#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

#include "Thermistor/thermistor.h"
#include "Thermistor/thermistor_ntc_100k_3950K.h"

//#define F_CPU 16000000

const uint8_t  button_pin = 4; // D4
const uint8_t  relay_pin_1 = A4;
const uint8_t  relay_pin_2 = A5;

// Used to count ticks 100 times a second
volatile uint8_t timer_ticker = 0;


const uint16_t time_on_seconds = 3600;
const uint16_t time_off_seconds = 7200;

// Using TIMER2 CompA vector to increment the time variable
ISR(TIMER2_COMPA_vect)
{
  timer_ticker++;
}

/**
  * @brief Setups timer 2 as a slow timer (with the biggest prescaler of 1024) and sets the compare value so that the interrupt frequency is around 100Hz.
*/
void setup_timer(void)
{
  TCCR2A = (1 << WGM21); // CTC mode

  // Prescaler of 1024
  TCCR2B = (1 << CS22) | (1 << CS21) | (1<<CS20);

  // 16MHz / 1024 -> 15625 HZ
  // 15625 / 156 ~= 100,1 HZ -> closest to 100 Hz so we can just "count" up to 100 with a software counter and derive seconds out of this
  // 15625 / 157 ~= 99,5 HZ

  // Will raise an interrupt around 100 times per second
  OCR2A = 155;

  // Enable interrupts for this counter
  TIMSK2 |= (1<<OCIE2A);
  TCNT2 = 0;
}

void setup()
{
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(relay_pin_1, OUTPUT);
  pinMode(relay_pin_2, OUTPUT);

  digitalWrite(relay_pin_1, LOW);
  digitalWrite(relay_pin_2, LOW);

  setup_timer();
  sei();
  //Serial.begin(9600);
}



// When the button is pushed, it means the refrigerator is in its "forced mode"
static bool forced_run = false;
static bool cooling_down = true;

// Keeps track of the previous time the system was toggled
static uint32_t previous_toggle = 0;

// Keeps track of current time
static uint32_t current_seconds = 0;

void aggregate_seconds(volatile uint8_t* ticker, uint32_t* seconds)
{
  if(*ticker >= 100)
  {
    *ticker = 0;
    (*seconds)++;
  }
}


void loop() {
  // read the state of the pushbutton value:
  forced_run = digitalRead(button_pin) == HIGH;

  aggregate_seconds(&timer_ticker, &current_seconds);

  //Serial.println(forced_run);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (forced_run == true)
  {
    // turn LED on:
    digitalWrite(relay_pin_1, LOW);
    digitalWrite(relay_pin_2, LOW);
  }
  else
  {
    if((cooling_down)
    && (current_seconds - previous_toggle) >= time_on_seconds)
    {
      // Toggling cooling mode
      cooling_down = false;
      previous_toggle = current_seconds;
    }
    else if((!cooling_down
          && (current_seconds - previous_toggle) >= time_off_seconds))
    {
      cooling_down = true;
      previous_toggle = current_seconds;
    }

    if(cooling_down)
    {
      digitalWrite(relay_pin_1, LOW);
      digitalWrite(relay_pin_2, LOW);
    }
    else
    {
      digitalWrite(relay_pin_1, HIGH);
      digitalWrite(relay_pin_2, HIGH);
    }
  }
}