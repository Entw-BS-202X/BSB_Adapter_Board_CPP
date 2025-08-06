#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

/**
 * @brief Arduino-style LCD class using 8-bit mode.
 * Works like LiquidCrystal, but implemented using your own GPIO functions.
 */
class LCD {
public:
  /**
   * @brief Constructor with full 8-bit data pin and control pin support.
   * @param rs Register select pin
   * @param rw Read/write control pin (pass 255 if unused and tied to GND)
   * @param en Enable pin
   */
  LCD(uint8_t rs, uint8_t rw, uint8_t en,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
      uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

  /**
   * @brief Initializes the LCD in 8-bit mode.
   * @param cols Number of columns (e.g. 16 or 20)
   * @param rows Number of rows (e.g. 2 or 4)
   */
  void begin(uint8_t cols, uint8_t rows);

  void clear();
  void home();
  void setCursor(uint8_t col, uint8_t row);
  void print(const char* str);
  void print(char c);
  void command(uint8_t cmd);
  void write(uint8_t data);

private:
  void send(uint8_t value, bool mode);
  void pulseEnable();
  void setupPins();
  void digitalWriteFast(uint8_t pin, uint8_t val);
  void pinModeFast(uint8_t pin, uint8_t mode);

  uint8_t _rs, _rw, _en;
  uint8_t _data_pins[8];
  uint8_t _cols, _rows;
};

#endif
