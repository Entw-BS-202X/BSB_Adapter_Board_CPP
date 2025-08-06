#ifndef TIMER0_MILLIS_H_
#define TIMER0_MILLIS_H_

#include <stdint.h>

/**
 * @file timer0_millis.h
 * @brief Provides millisecond timing using Timer0 and a Blink class for non-blocking LED control.
 *
 * This module initializes AVR Timer0 in CTC mode to generate 1ms ticks,
 * which are used by the millis() function and Blink objects for time tracking.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes Timer0 to generate 1 millisecond interrupts.
 * This function must be called before using millis() or Blink objects.
 */
void Timer0_Init(void);

/**
 * @brief Returns the number of milliseconds since Timer0 was initialized.
 * The counter increments every 1ms using Timer0 Compare Match interrupts.
 *
 * @return Milliseconds since power-up (as a 32-bit unsigned integer)
 */
uint32_t millis(void);

/**
 * @brief Provides non-blocking LED blinking on a specific pin.
 *
 * @param pin      The pin number (e.g., APB7, APD2)
 * @param on_time  Duration in milliseconds the pin should stay HIGH
 * @param off_time Duration in milliseconds the pin should stay LOW
 *
 * This function must be called repeatedly in the main loop to operate correctly.
 * It is statically implemented and supports only one pin at a time.
 * Use the Blink class for multiple concurrent blinkers.
 */
void blink(uint8_t pin, uint16_t on_time, uint16_t off_time);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * @class Blink
 * @brief A non-blocking LED blinker using Timer0 and millis().
 *
 * Use this class to blink one or more pins with independent ON/OFF durations.
 * Example:
 *
 *     Blink led(APB7, 150, 850);
 *     while (1) {
 *         led.update();  // Call regularly
 *     }
 */
class Blink {
private:
    uint8_t _pin;
    uint16_t _on_time;
    uint16_t _off_time;
    uint32_t _last_toggle_time;
    bool _state;

public:
    /**
     * @brief Constructor to set up a blinker.
     * @param pin      Pin to toggle (e.g., APB7)
     * @param on_time  ON duration in ms
     * @param off_time OFF duration in ms
     */
    Blink(uint8_t pin, uint16_t on_time, uint16_t off_time);

    /**
     * @brief Call this function repeatedly in the main loop to toggle the pin.
     */
    void update();
};

#endif

#endif /* TIMER0_MILLIS_H_ */
