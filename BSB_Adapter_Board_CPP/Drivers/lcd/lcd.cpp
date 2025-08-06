#include "lcd.h"
#include <util/delay.h>
#include "gpio.h"  // Your GPIO library

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// HD44780 commands
#define LCD_CLEARDISPLAY  0x01
#define LCD_RETURNHOME    0x02
#define LCD_ENTRYMODESET  0x06
#define LCD_DISPLAYON     0x0C
#define LCD_FUNCTIONSET   0x30
#define LCD_2LINE         0x08
#define LCD_SETDDRAMADDR  0x80

// Updated constructor with RW
LCD::LCD(uint8_t rs, uint8_t rw, uint8_t en,
uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
: _rs(rs), _rw(rw), _en(en), _cols(0), _rows(0) {
	_data_pins[0] = d0;
	_data_pins[1] = d1;
	_data_pins[2] = d2;
	_data_pins[3] = d3;
	_data_pins[4] = d4;
	_data_pins[5] = d5;
	_data_pins[6] = d6;
	_data_pins[7] = d7;
}

void LCD::begin(uint8_t cols, uint8_t rows) {
	_cols = cols;
	_rows = rows;

	setupPins();

	_delay_ms(50);

	command(LCD_FUNCTIONSET | (_rows > 1 ? LCD_2LINE : 0x00));
	command(LCD_DISPLAYON);
	command(LCD_CLEARDISPLAY);
	command(LCD_ENTRYMODESET);
	_delay_ms(2);
}

void LCD::clear() {
	command(LCD_CLEARDISPLAY);
	_delay_ms(2);
}

void LCD::home() {
	command(LCD_RETURNHOME);
	_delay_ms(2);
}

void LCD::setCursor(uint8_t col, uint8_t row) {
	static const uint8_t row_offsets_2[] = {0x00, 0x40};
	static const uint8_t row_offsets_4[] = {0x00, 0x40, 0x14, 0x54};
	const uint8_t* offsets = (_rows > 2) ? row_offsets_4 : row_offsets_2;

	if (row >= _rows) row = _rows - 1;
	command(LCD_SETDDRAMADDR | (col + offsets[row]));
}

void LCD::print(const char* str) {
	while (*str) {
		write(*str++);
	}
}

void LCD::print(char c) {
	write(c);
}

void LCD::command(uint8_t cmd) {
	send(cmd, false);  // mode = 0 for command
}

void LCD::write(uint8_t data) {
	send(data, true);  // mode = 1 for data
}

void LCD::send(uint8_t value, bool mode) {
	// Set RS
	digitalWrite(_rs, mode);

	// Set RW = 0 for write if used
	if (_rw != 255) digitalWrite(_rw, 0);

	// Set data lines
	for (uint8_t i = 0; i < 8; i++) {
		digitalWrite(_data_pins[i], (value >> i) & 0x01);
	}

	pulseEnable();
	_delay_us(50);
}

void LCD::pulseEnable() {
	digitalWrite(_en, 1);
	_delay_us(1);
	digitalWrite(_en, 0);
	_delay_us(1);
}

void LCD::setupPins() {
	pinMode(_rs, 1);
	pinMode(_en, 1);
	if (_rw != 255) pinMode(_rw, 1);  // Output mode only if RW is used

	for (uint8_t i = 0; i < 8; i++) {
		pinMode(_data_pins[i], 1);
	}
}

// Optional wrappers
void LCD::digitalWriteFast(uint8_t pin, uint8_t val) {
	digitalWrite(pin, val);
}

void LCD::pinModeFast(uint8_t pin, uint8_t mode) {
	pinMode(pin, mode);
}
