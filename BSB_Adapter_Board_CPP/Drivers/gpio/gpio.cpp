#include "gpio.h"

// -----------------------------------------------------------------------------
// Internal Helpers
// -----------------------------------------------------------------------------

/**
 * @brief Returns the data direction register (DDRx) pointer for a given pin.
 *
 * @param pin Encoded pin value (e.g., APB3 = 19)
 * @return Pointer to DDRx register or nullptr if unsupported
 */
static volatile uint8_t* get_ddr(uint8_t pin) {
    if (pin >= 0 && pin <= 7)    return &DDRD;  // APAx ? PORTD for testing (simulated APA0–APA7)
    if (pin >= 8 && pin <= 15)   return &DDRC;  // APCx
    if (pin >= 16 && pin <= 23)  return &DDRB;  // APBx
    if (pin >= 32 && pin <= 39)  return &DDRC;
    if (pin >= 48 && pin <= 55)  return &DDRD;
    if (pin >= 64 && pin <= 71)  return &DDRE;
    if (pin >= 80 && pin <= 87)  return &DDRF;
    if (pin >= 96 && pin <= 101) return &DDRG;
    if (pin >= 112 && pin <= 119) return &DDRH;
    if (pin >= 128 && pin <= 135) return &DDRJ;
    if (pin >= 144 && pin <= 151) return &DDRK;
    if (pin >= 160 && pin <= 167) return &DDRL;

    return 0;  // Invalid or unsupported
}

/**
 * @brief Returns the PORTx register pointer for a given pin.
 *
 * @param pin Encoded pin value
 * @return Pointer to PORTx register or nullptr
 */
static volatile uint8_t* get_port(uint8_t pin) {
    if (pin >= 0 && pin <= 7)    return &PORTD;
    if (pin >= 8 && pin <= 15)   return &PORTC;
    if (pin >= 16 && pin <= 23)  return &PORTB;
    if (pin >= 32 && pin <= 39)  return &PORTC;
    if (pin >= 48 && pin <= 55)  return &PORTD;
    if (pin >= 64 && pin <= 71)  return &PORTE;
    if (pin >= 80 && pin <= 87)  return &PORTF;
    if (pin >= 96 && pin <= 101) return &PORTG;
    if (pin >= 112 && pin <= 119) return &PORTH;
    if (pin >= 128 && pin <= 135) return &PORTJ;
    if (pin >= 144 && pin <= 151) return &PORTK;
    if (pin >= 160 && pin <= 167) return &PORTL;

    return 0;
}

/**
 * @brief Returns the PINx input register pointer for a given pin.
 *
 * @param pin Encoded pin value
 * @return Pointer to PINx register or nullptr
 */
static volatile uint8_t* get_pin_reg(uint8_t pin) {
    if (pin >= 0 && pin <= 7)    return &PIND;
    if (pin >= 8 && pin <= 15)   return &PINC;
    if (pin >= 16 && pin <= 23)  return &PINB;
    if (pin >= 32 && pin <= 39)  return &PINC;
    if (pin >= 48 && pin <= 55)  return &PIND;
    if (pin >= 64 && pin <= 71)  return &PINE;
    if (pin >= 80 && pin <= 87)  return &PINF;
    if (pin >= 96 && pin <= 101) return &PING;
    if (pin >= 112 && pin <= 119) return &PINH;
    if (pin >= 128 && pin <= 135) return &PINJ;
    if (pin >= 144 && pin <= 151) return &PINK;
    if (pin >= 160 && pin <= 167) return &PINL;

    return 0;
}

/**
 * @brief Extracts the bit index within the port for a given pin.
 *
 * @param pin Encoded pin number
 * @return Bit index 0–7
 */
static uint8_t get_bit(uint8_t pin) {
    return pin % 8;
}

// -----------------------------------------------------------------------------
// Public GPIO API
// -----------------------------------------------------------------------------

/**
 * @brief Configures a pin as input or output.
 *
 * @param pin  Encoded pin macro (e.g., APB3)
 * @param mode INPUT or OUTPUT
 */
void pinMode(uint8_t pin, uint8_t mode) {
    volatile uint8_t* ddr = get_ddr(pin);
    if (!ddr) return;
    uint8_t bit = get_bit(pin);

    if (mode == OUTPUT)
        *ddr |= (1 << bit);
    else
        *ddr &= ~(1 << bit);
}

/**
 * @brief Writes a digital value (HIGH or LOW) to the specified pin.
 *
 * @param pin  Encoded pin macro
 * @param val  HIGH or LOW
 */
void digitalWrite(uint8_t pin, uint8_t val) {
    volatile uint8_t* port = get_port(pin);
    if (!port) return;
    uint8_t bit = get_bit(pin);

    if (val == HIGH)
        *port |= (1 << bit);
    else
        *port &= ~(1 << bit);
}

/**
 * @brief Reads the current logical state of a digital input pin.
 *
 * @param pin  Encoded pin macro
 * @return     HIGH if logic level is 1, otherwise LOW
 */
int digitalRead(uint8_t pin) {
    volatile uint8_t* pin_reg = get_pin_reg(pin);
    if (!pin_reg) return 0;
    uint8_t bit = get_bit(pin);

    return (*pin_reg & (1 << bit)) ? HIGH : LOW;
}
