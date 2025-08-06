#include "timer0_millis.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"      // Required for pinMode() and digitalWrite()
#include "serial.h"    // Serialx functions

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// -----------------------------------------------------------------------------
// Millis() Implementation Using Timer0
// -----------------------------------------------------------------------------

volatile uint32_t _ms_counter = 0;

/**
 * @brief Timer0 Compare Match interrupt service routine.
 * Increments the millisecond counter every 1ms.
 */
ISR(TIMER0_COMPA_vect)
{
    _ms_counter++;
}

/**
 * @brief Initializes Timer0 in CTC mode to generate a 1ms interrupt.
 */
void Timer0_Init(void)
{
    TCCR0A = (1 << WGM01);                    // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00);       // Prescaler 64
    OCR0A = (F_CPU / 64UL / 1000UL) - 1;      // Compare match for 1ms
    TIMSK0 |= (1 << OCIE0A);                  // Enable Timer0 Compare A interrupt
    sei();                                    // Enable global interrupts
}

/**
 * @brief Returns the number of milliseconds since Timer0 was initialized.
 */
uint32_t millis(void)
{
    uint32_t ms;
    cli();
    ms = _ms_counter;
    sei();
    return ms;
}

// -----------------------------------------------------------------------------
// C-style Blink Function (Single-Instance Only)
// -----------------------------------------------------------------------------

/**
 * @brief Simple non-blocking blink function for toggling a pin.
 */
void blink(uint8_t pin, uint16_t on_time, uint16_t off_time)
{
    static bool state = false;
    static uint32_t last_toggle_time = 0;

    uint32_t now = millis();

    if (state && (now - last_toggle_time >= on_time)) {
        digitalWrite(pin, LOW);          // Turn OFF
        state = false;
        last_toggle_time = now;
    }
    else if (!state && (now - last_toggle_time >= off_time)) {
        digitalWrite(pin, HIGH);         // Turn ON
        state = true;
        last_toggle_time = now;
        Serial3.println("BSB_Adapter_Platine MCU läuft...!");
    }
}

// -----------------------------------------------------------------------------
// Blink Class Implementation (Multiple Concurrent Pins)
// -----------------------------------------------------------------------------

Blink::Blink(uint8_t pin, uint16_t on_time, uint16_t off_time)
    : _pin(pin), _on_time(on_time), _off_time(off_time),
      _last_toggle_time(0), _state(false)
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);  // Initially OFF
}

void Blink::update()
{
    uint32_t now = millis();

    if (_state && (now - _last_toggle_time >= _on_time)) {
        digitalWrite(_pin, HIGH);     // Turn OFF
        _state = false;
        _last_toggle_time = now;
    }
    else if (!_state && (now - _last_toggle_time >= _off_time)) {
        digitalWrite(_pin, LOW);    // Turn ON
        _state = true;
        _last_toggle_time = now;
        Serial3.println("BSB_Adapter_Platine MCU sich bewegt...!");
    }
}
