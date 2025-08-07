#include <avr/io.h>
#include "tasks.h"
#include "gpio.h"
#include "serial.h"
#include "board.h"
#include "lcd.h"
#include <stdlib.h>
#include "adc.h"

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

void ADCTask()
{
	uint16_t adcBuffer[16]; // Buffer for all 16 channels
	
 adc.analogReadAllOversampled(8, adcBuffer);  // Oversample all 16 channels

// Print header
Serial3.println("ADC Channel Readings (Oversampled):");
Serial3.println("-----------------------------");

// First line: A0 to A7
for (uint8_t i = 0; i < 8; ++i) {
	Serial3.print("A");
	Serial3.print(i);
	Serial3.print("=");
	Serial3.print(adcBuffer[i]);
	Serial3.print("\t");
}
Serial3.println(""); // New line

// Second line: A8 to A15
for (uint8_t i = 8; i < 16; ++i) {
	Serial3.print("A");
	Serial3.print(i);
	Serial3.print("=");
	Serial3.print(adcBuffer[i]);
	Serial3.print("\t");
}
Serial3.println(""); // End

Serial3.println("-----------------------------");

}



