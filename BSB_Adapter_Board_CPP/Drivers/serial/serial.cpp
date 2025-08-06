#include "serial.h"
#include <avr/io.h>
#include <stdlib.h>  // for itoa

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD_PRESCALE(baud) ((F_CPU / (16UL * baud)) - 1)

void SerialClass::begin(uint32_t baudrate) {
	uint16_t ubrr = BAUD_PRESCALE(baudrate);

	if (this == &Serial) {
		UBRR0H = (ubrr >> 8);
		UBRR0L = ubrr;
		UCSR0B = (1 << RXEN0) | (1 << TXEN0);
		UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
		} else if (this == &Serial1) {
		UBRR1H = (ubrr >> 8);
		UBRR1L = ubrr;
		UCSR1B = (1 << RXEN1) | (1 << TXEN1);
		UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
		} else if (this == &Serial2) {
		UBRR2H = (ubrr >> 8);
		UBRR2L = ubrr;
		UCSR2B = (1 << RXEN2) | (1 << TXEN2);
		UCSR2C = (1 << UCSZ21) | (1 << UCSZ20);
		} else if (this == &Serial3) {
		UBRR3H = (ubrr >> 8);
		UBRR3L = ubrr;
		UCSR3B = (1 << RXEN3) | (1 << TXEN3);
		UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
	}
}

void SerialClass::write(uint8_t data) {
	if (this == &Serial) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = data;
		} else if (this == &Serial1) {
		while (!(UCSR1A & (1 << UDRE1)));
		UDR1 = data;
		} else if (this == &Serial2) {
		while (!(UCSR2A & (1 << UDRE2)));
		UDR2 = data;
		} else if (this == &Serial3) {
		while (!(UCSR3A & (1 << UDRE3)));
		UDR3 = data;
	}
}

int SerialClass::read() {
	if (this == &Serial && (UCSR0A & (1 << RXC0))) return UDR0;
	if (this == &Serial1 && (UCSR1A & (1 << RXC1))) return UDR1;
	if (this == &Serial2 && (UCSR2A & (1 << RXC2))) return UDR2;
	if (this == &Serial3 && (UCSR3A & (1 << RXC3))) return UDR3;
	return -1;
}

bool SerialClass::available() {
	if (this == &Serial) return (UCSR0A & (1 << RXC0));
	if (this == &Serial1) return (UCSR1A & (1 << RXC1));
	if (this == &Serial2) return (UCSR2A & (1 << RXC2));
	if (this == &Serial3) return (UCSR3A & (1 << RXC3));
	return false;
}

void SerialClass::print(const char* str) {
	while (*str) {
		write(*str++);
	}
}

void SerialClass::println(const char* str) {
	print(str);
	write('\r');
	write('\n');
}

void SerialClass::print(int value) {
	char buf[16];
	itoa(value, buf, 10);
	print(buf);
}

void SerialClass::println(int value) {
	print(value);
	write('\r');
	write('\n');
}

// Global instances
SerialClass Serial;
SerialClass Serial1;
SerialClass Serial2;
SerialClass Serial3;
