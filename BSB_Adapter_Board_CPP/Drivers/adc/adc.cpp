#include "adc.h"

/*
 * ============================================================
 * ATmega2560 ADC Channel-to-Pin Mapping (TQFP-100)
 * ============================================================
 *
 * The ATmega2560 provides 16 analog input channels (ADC0–ADC15)
 * spread across PORTF and PORTK. These correspond directly to
 * Arduino Mega2560 analog pins A0–A15 and follow this mapping:
 *
 * ------------- PORTF (ADC0–ADC7) -------------
 * | ADC | Port | Arduino Pin | TQFP Pin |
 * |-----|------|--------------|-----------|
 * |  0  | PF0  | A0           | 97        |
 * |  1  | PF1  | A1           | 96        |
 * |  2  | PF2  | A2           | 95        |
 * |  3  | PF3  | A3           | 94        |
 * |  4  | PF4  | A4           | 93        |
 * |  5  | PF5  | A5           | 92        |
 * |  6  | PF6  | A6           | 91        |
 * |  7  | PF7  | A7           | 90        |
 *
 * ------------- PORTK (ADC8–ADC15) -------------
 * | ADC | Port | Arduino Pin | TQFP Pin |
 * |-----|------|--------------|-----------|
 * |  8  | PK0  | A8           | 89        |
 * |  9  | PK1  | A9           | 88        |
 * | 10  | PK2  | A10          | 87        |
 * | 11  | PK3  | A11          | 86        |
 * | 12  | PK4  | A12          | 85        |
 * | 13  | PK5  | A13          | 84        |
 * | 14  | PK6  | A14          | 83        |
 * | 15  | PK7  | A15          | 82        |
 *
 * Notes:
 * - All channels support single-ended analog input.
 * - analogRead(Ax) and adc.analogRead(x) map directly.
 * - Differential inputs exist for ADC0–7 but are not supported here.
 */

/*
 * =========================
 * Prescaler Selection Table
 * =========================
 * For F_CPU = 16 MHz
 *
 * | Prescaler | ADC Clock | Conversion Time | Sampling Rate (ADC_CLK / 13) | Resolution Notes       |
 * |-----------|------------|------------------|-------------------------------|-------------------------|
 * |     2     | 8.0 MHz    | 1.63 µs          | ~615,385 samples/sec          | ? Very poor (~6-bit)   |
 * |     4     | 4.0 MHz    | 3.25 µs          | ~307,692 samples/sec          | ? Inaccurate           |
 * |     8     | 2.0 MHz    | 6.5 µs           | ~153,846 samples/sec          | ? Limited (~8-bit)     |
 * |    16     | 1.0 MHz    | 13.0 µs          | ~76,923 samples/sec           | ? Acceptable (~8–9 bit)|
 * |    32     | 500 kHz    | 26.0 µs          | ~38,461 samples/sec           | ? Slight loss (~9-bit) |
 * |    64     | 250 kHz    | 52.0 µs          | ~19,230 samples/sec           | ? Close to 10-bit limit|
 * |   128     | 125 kHz    | 104.0 µs         | ~9,615 samples/sec            | ? Full 10-bit accuracy |
 *
 * Recommended for 10-bit mode: 50 – 200 kHz ADC Clock ? Prescaler = 128, 64, or 32
 */

/*
 * =========================
 * ADC Clock Prescaler Guide
 * =========================
 * ADC requires a clock between 50–200 kHz for 10-bit accuracy.
 * This is configured via a prescaler, dividing the CPU frequency.
 *
 * Recommended: Prescaler = 128 (gives 125 kHz from 16 MHz)
 */

// ========================
// ADC Initialization
// ========================
/**
 * Initializes the ADC with selected mode (single or free-running)
 * and desired ADC clock frequency. Ensures valid resolution and
 * selects a suitable prescaler automatically.
 */
void AVR_ADC::init(uint8_t mode, uint32_t adcClockHz) {
    _mode = mode;

    // Clamp requested ADC clock frequency for 10-bit accuracy
    if (_resolution == 10) {
        if (adcClockHz < 50000)  adcClockHz = 50000;
        if (adcClockHz > 200000) adcClockHz = 200000;
    }

    const uint32_t F_CPU_HZ = 16000000UL; // Main clock
    uint8_t prescalerBits = 0b111;        // Default: 128

    // Mapping between ADC prescaler values and register bits
    struct PrescalerEntry {
        uint8_t bits;   // Bit pattern for ADPS2:0
        uint8_t value;  // Actual prescaler divisor
    };

    const PrescalerEntry table[] = {
        {0b001, 2},
        {0b010, 4},
        {0b011, 8},
        {0b100, 16},
        {0b101, 32},
        {0b110, 64},
        {0b111, 128}
    };

    // Select the smallest prescaler that produces ADC_CLK ? adcClockHz
    for (uint8_t i = 0; i < sizeof(table)/sizeof(table[0]); ++i) {
        if ((F_CPU_HZ / table[i].value) <= adcClockHz) {
            prescalerBits = table[i].bits;
            break;
        }
    }

    // Enable ADC, set calculated prescaler
    ADCSRA = (1 << ADEN); // Enable ADC
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); // Clear prescaler
    ADCSRA |= prescalerBits; // Set prescaler bits

    // Set reference voltage to AVcc (default)
    ADMUX = (1 << REFS0);

    // Free-running mode: auto-trigger enabled, start conversion
    if (_mode == ADC_MODE_FREE) {
        ADCSRA |= (1 << ADATE);  // Auto Trigger Enable
        ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));  // Trigger Source: Free Running
        ADCSRA |= (1 << ADSC);   // Start first conversion
    } else {
        // Single conversion mode
        ADCSRA &= ~(1 << ADATE); // Manual trigger
    }
}

// ========================
// Set Reference Voltage
// ========================
/**
 * Sets the ADC reference voltage source.
 * Options: External, AVcc, Internal 1.1V, Internal 2.56V (fallback).
 */
void AVR_ADC::setReference(uint8_t mode) {
    // Clear previous reference bits
    ADMUX &= ~((1 << REFS1) | (1 << REFS0));

    // Set reference based on mode
    switch (mode) {
        case MODE_EXVREF:  break; // External reference (AREF pin)
        case MODE_AVCC:    ADMUX |= (1 << REFS0); break; // AVcc with capacitor
        case MODE_INT_1V1: ADMUX |= (1 << REFS1) | (1 << REFS0); break; // Internal 1.1V
        case MODE_INT_2V56: ADMUX |= (1 << REFS0); break; // (Fallback, only REFS0)
        default:           ADMUX |= (1 << REFS0); break; // Default: AVcc
    }
}

// ========================
// Set and Get Resolution
// ========================
/**
 * Sets resolution to 8–10 bits (10-bit is native; 8/9-bit via shift).
 */
void AVR_ADC::setResolution(uint8_t bits) {
    if (bits < 8) bits = 8;
    if (bits > 10) bits = 10;
    _resolution = bits;
}

/**
 * Returns currently configured resolution (in bits).
 */
uint8_t AVR_ADC::getResolution() {
    return _resolution;
}

// ========================
// Single ADC Read
// ========================
/**
 * Reads an analog value from the specified channel (0–15).
 * Valid only in single conversion mode.
 */
uint16_t AVR_ADC::analogRead(uint8_t channel) {
    if (_mode != ADC_MODE_SINGLE || channel > 15) return 0;

    // Configure channel: handle MUX5 for channels > 7
    ADCSRB &= ~(1 << MUX5);                        // Clear MUX5
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);     // Set MUX0–MUX3
    if (channel > 7) ADCSRB |= (1 << MUX5);        // Set MUX5 if needed

    // Start conversion and wait for completion
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)); // Wait until ADSC clears

    uint16_t raw = ADCW;
    return (_resolution == 10) ? raw : (raw >> (10 - _resolution));
}

// ========================
// Latest ADC Value (Free Mode)
// ========================
/**
 * Returns the most recent ADC conversion result.
 * Only valid if ADC is running in free mode.
 */
uint16_t AVR_ADC::readLatest() {
    uint16_t raw = ADCW;
    return (_resolution == 10) ? raw : (raw >> (10 - _resolution));
}

// ========================
// Oversampled ADC Read
// ========================
/**
 * Performs multiple analog reads and returns the averaged result.
 * Oversampling helps reduce noise.
 */
uint16_t AVR_ADC::analogReadOversampled(uint8_t channel, uint8_t oversample) {
	if (channel > 15 || oversample == 0 || oversample > 8) return 0;

	uint32_t sum = 0;
	for (uint8_t i = 0; i < oversample; ++i) {
		sum += analogRead(channel);
	}

	// Return average
	return sum / oversample;
}

// ========================
// Read All Channels Oversampled
// ========================
/**
 * Reads all 16 ADC channels and stores averaged results in buffer.
 * Useful for scanning multiple analog inputs at once.
 */
void AVR_ADC::analogReadAllOversampled(uint8_t oversample, uint16_t* buffer) {
	if (oversample == 0 || oversample > 8 || buffer == nullptr) return;

	for (uint8_t ch = 0; ch < 16; ++ch) {
		uint32_t sum = 0;
		for (uint8_t i = 0; i < oversample; ++i) {
			sum += analogRead(ch);
		}
		buffer[ch] = sum / oversample;
	}
}

// ========================
// Global ADC Object
// ========================
// Provides a ready-to-use ADC instance (singleton-style)
AVR_ADC adc;

