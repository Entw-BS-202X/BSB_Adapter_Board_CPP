#ifndef BOARD_H_
#define BOARD_H_

// Define the CPU clock frequency before including delay/timer headers
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "scheduler.h"
#include "lcd.h"

// Define the LED pin using a custom macro (e.g., APB7 style)
#define LED_P2 APB7

// LCD pin configuration:
// RS -> D12, RW -> D10, EN -> D11
// D4~D7 -> D5, D4, D3, D2 


#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Initializes all board-level drivers and GPIOs.
 *
 * This includes:
 * - Timer0 for millis()
 * - GPIO setup for default board pins
 *
 * Call this once in main().
 */
void Board_Init(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#endif

#endif /* BOARD_H_ */
