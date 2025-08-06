#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>

class SerialClass {
	public:
	void begin(uint32_t baudrate);
	void write(uint8_t data);
	int read();
	bool available();

	void print(const char* str);
	void println(const char* str);

	void print(int value);
	void println(int value);
};

extern SerialClass Serial;
extern SerialClass Serial1;
extern SerialClass Serial2;
extern SerialClass Serial3;

#endif /* SERIAL_H_ */
