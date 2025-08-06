
#include "board.h"
#include "scheduler.h"
#include "lcd.h"

int main(void) {
	
	Board_Init();       // Set up GPIOs, peripherals
	scheduler.begin();  // Add tasks and start scheduler

	while (1) {
		scheduler.run();  // Run ready tasks in priority order
	}
}
