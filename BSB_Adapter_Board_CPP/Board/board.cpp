
#include "board.h"
#include "gpio.h"
#include "serial.h"
#include "scheduler.h"
#include "lcd.h"
#include "adc.h"
#include <util/delay.h>

/**
 * @brief Initializes all essential hardware components of the board.
 *
 * This function should be called once at the beginning of main().
 * It initializes:
 *   - Timer0 for millisecond timing
 *   - GPIO configuration for default LEDs or I/Os
 *
 * You can extend this function to initialize other subsystems like UART, I2C, etc.
 */

LCD lcd(APG1, APB4, APE6, APH0, APH1, APH2, APH3, APH4 , APH5, APH6, APH7);

void Board_Init(void)
{

    // Set up GPIOs used by the board
    pinMode(LED_P2, OUTPUT);
    digitalWrite(LED_P2, LOW);  // Ensure LED starts in OFF state

    // === Optional Future Initializations ===
    // init_uart();
	
	Serial3.begin(9600);
	_delay_ms(500);
	Serial3.println("BSB_Adapter_Paltine sagt Hallo...!");
	
	scheduler.begin(); // Tasks are initialized inside function

		
    // init_i2c();
	
    // init_adc();
	
    
    adc.init(ADC_MODE_SINGLE);		  // Use default: 125 kHz ADC clock, single conversion mode
    adc.setReference(MODE_AVCC);      
    adc.setResolution(10);
	  
	
    // init_lcd();
	
	lcd.begin(16,2);
	_delay_ms(100);
	
	
    // init_buttons();
}
