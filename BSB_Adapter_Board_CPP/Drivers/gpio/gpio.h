/*
 * AVR GPIO Library for ATmega2560 with Arduino-style Interface
 * ------------------------------------------------------------
 *
 * This library provides an Arduino-style digital I/O interface for bare-metal
 * AVR programming, specifically tailored for the ATmega2560.
 *
 * It allows you to use familiar function names:
 *     - pinMode(pin, mode)
 *     - digitalWrite(pin, value)
 *     - digitalRead(pin)
 *
 * without relying on the Arduino core libraries.
 *
 * Naming Convention:
 * ------------------
 * To avoid conflicts with AVR's internal port macros (like PB0, PC3),
 * this library introduces a unique naming scheme using the prefix 'AP' for
 * "Arduino Port":
 *
 *     - APA0 to APA7  ? Port A pins 0 to 7  ? Values 0–7
 *     - APB0 to APB7  ? Port B pins 0 to 7  ? Values 16–23
 *     - APC0 to APC7  ? Port C pins 0 to 7  ? Values 32–39
 *     - ...
 *     - APL0 to APL7  ? Port L pins 0 to 7  ? Values 160–167
 *
 * This ensures clean and conflict-free usage in all environments,
 * including Atmel Studio, avr-gcc, or other toolchains.
 *
 * Internal Mechanism:
 * -------------------
 * Each pin is encoded as a unique integer based on its port and bit:
 *     pin_number = (port_base_offset) + bit_position
 *
 * For example:
 *     - APB7 (Port B, Pin 7) ? 16 + 7 ? 23
 *     - APD2 (Port D, Pin 2) ? 48 + 2 ? 50
 *
 * The implementation file (gpio.cpp) uses this encoded value to determine:
 *     - Which PORTx, DDRx, or PINx register to use
 *     - Which bit to set, clear, or read
 *
 * Usage Example:
 * --------------
 *     pinMode(APB7, OUTPUT);       // Set PB7 as output
 *     digitalWrite(APB7, HIGH);    // Drive PB7 high
 *     if (digitalRead(APD2)) {     // Read PD2 value
 *         // do something
 *     }
 *
 * Port Coverage:
 * --------------
 * This header defines all pins from Port A to Port L (ATmega2560 full support),
 * for a total of 86 usable digital pins.
 *
 * No hardware-specific or compiler-specific macros are used (e.g., _SFR_IO8_ADDR),
 * ensuring portability and compatibility.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>
#include <stdint.h>

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

// PORTA (0–7)
#define APA0 0
#define APA1 1
#define APA2 2
#define APA3 3
#define APA4 4
#define APA5 5
#define APA6 6
#define APA7 7

// PORTB (16–23)
#define APB0 16
#define APB1 17
#define APB2 18
#define APB3 19
#define APB4 20
#define APB5 21
#define APB6 22
#define APB7 23

// PORTC (32–39)
#define APC0 32
#define APC1 33
#define APC2 34
#define APC3 35
#define APC4 36
#define APC5 37
#define APC6 38
#define APC7 39

// PORTD (48–55)
#define APD0 48
#define APD1 49
#define APD2 50
#define APD3 51
#define APD4 52
#define APD5 53
#define APD6 54
#define APD7 55

// PORTE (64–71)
#define APE0 64
#define APE1 65
#define APE2 66
#define APE3 67
#define APE4 68
#define APE5 69
#define APE6 70
#define APE7 71

// PORTF (80–87)
#define APF0 80
#define APF1 81
#define APF2 82
#define APF3 83
#define APF4 84
#define APF5 85
#define APF6 86
#define APF7 87

// PORTG (96–101)
#define APG0 96
#define APG1 97
#define APG2 98
#define APG3 99
#define APG4 100
#define APG5 101

// PORTH (112–119)
#define APH0 112
#define APH1 113
#define APH2 114
#define APH3 115
#define APH4 116
#define APH5 117
#define APH6 118
#define APH7 119

// PORTJ (128–135)
#define APJ0 128
#define APJ1 129
#define APJ2 130
#define APJ3 131
#define APJ4 132
#define APJ5 133
#define APJ6 134
#define APJ7 135

// PORTK (144–151)
#define APK0 144
#define APK1 145
#define APK2 146
#define APK3 147
#define APK4 148
#define APK5 149
#define APK6 150
#define APK7 151

// PORTL (160–167)
#define APL0 160
#define APL1 161
#define APL2 162
#define APL3 163
#define APL4 164
#define APL5 165
#define APL6 166
#define APL7 167

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H_ */
