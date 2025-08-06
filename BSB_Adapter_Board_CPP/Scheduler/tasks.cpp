#include <avr/io.h>
#include "tasks.h"
#include "gpio.h"
#include "serial.h"
#include "board.h"
#include "lcd.h"
#include <stdlib.h>

// Global variables for internal task state (if needed)

extern LCD lcd;

/**
 * @brief Toggles LED connected to PB7.
 * This is the highest priority task (0).
 */
void blinkTask(void) {
	static uint8_t state = 0;

	switch (state) {
		case 0:
		digitalWrite(LED_P2, LOW); // Turn ON LED Activ LOW
		break;
		case 1: case 2: case 3: case 4:
		digitalWrite(LED_P2, HIGH);  // Turn OFF
		break;
	}

	state = (state + 1) % 5;  // Cycle from 0 to 4
}

/**
 * @brief Simulates UART output (e.g., heartbeat or status).
 * Replace this with real UART print if needed.
 */
void uart3Task(void) {
	static uint8_t uartCounter = 0;
	uartCounter++;
	Serial3.print(" | uartCounter = "); Serial3.println(uartCounter);
}

/**
 * @brief Simulates LCD 

 */
void lcdTask()
{
	static uint32_t time = 0;
	time++;

	// Move cursor to beginning of line 1
	lcd.setCursor(0, 0);
	lcd.print("LCD Test - ");

	// Convert number to string
	char buffer[11];  // Enough for max 32-bit unsigned
	itoa(time, buffer, 10);
	lcd.print(buffer);
}



