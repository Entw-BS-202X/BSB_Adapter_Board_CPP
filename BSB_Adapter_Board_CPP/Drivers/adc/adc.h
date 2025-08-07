#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <stdint.h>

/*
 * ================================
 * ADC Voltage Reference Options
 * ================================
 * MODE_EXVREF   ? External AREF pin
 * MODE_AVCC     ? AVcc (5V/3.3V) with external capacitor on AREF pin
 * MODE_INT_1V1  ? Internal 1.1V reference
 * MODE_INT_2V56 ? Not supported on ATmega2560 (fallback to AVCC)
 */
#define MODE_EXVREF    0
#define MODE_AVCC      1
#define MODE_INT_1V1   2
#define MODE_INT_2V56  3

/*
 * ================================
 * ADC Operating Modes
 * ================================
 * ADC_MODE_SINGLE ? One conversion per call (analogRead)
 * ADC_MODE_FREE   ? Continuous conversion (readLatest)
 */
#define ADC_MODE_SINGLE 0
#define ADC_MODE_FREE   1

class AVR_ADC {
public:
    /*
     * Initialize ADC
     *
     * @param mode        ADC_MODE_SINGLE or ADC_MODE_FREE
     * @param adcClockHz  Desired ADC clock frequency in Hz (default = 125000 Hz)
     *
     * Note:
     *   - For 10-bit resolution, ADC clock should be 50–200 kHz
     *   - 125 kHz (prescaler = 128) is ideal for 10-bit accuracy
     *   - Library automatically chooses the closest supported prescaler
     */
    void init(uint8_t mode, uint32_t adcClockHz = 125000);

    void setReference(uint8_t mode);
    void setResolution(uint8_t bits);    // 8, 9, or 10 bits (scales output)
    uint8_t getResolution();

    uint16_t analogRead(uint8_t channel);  // Single-shot blocking read
    uint16_t readLatest();                 // Free-running read (non-blocking)
	
	    /**
     * Read a single channel with oversampling
     * @param channel     ADC channel (0–15)
     * @param oversample  Number of samples to average (1–8)
     * @return Averaged ADC value (10-bit max)
     */
    uint16_t analogReadOversampled(uint8_t channel, uint8_t oversample);

    /**
     * Read all 16 channels with oversampling
     * @param oversample  Number of samples to average (1–8)
     * @param buffer      Pointer to 16-element uint16_t array to store results
     */
    void analogReadAllOversampled(uint8_t oversample, uint16_t* buffer);

private:
    uint8_t _resolution = 10;
    uint8_t _mode = ADC_MODE_SINGLE;
};



extern AVR_ADC adc;

#endif // ADC_H
