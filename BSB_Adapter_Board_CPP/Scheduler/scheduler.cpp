#include "scheduler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "board.h"
#include "tasks.h"
#include "Serial.h"

#include <string.h> // Optional for memset()

Scheduler scheduler;

void blinkTask(void);
void uart3Task(void);

void Scheduler::init() {
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS01) | (1 << CS00);
	OCR0A = (F_CPU / 64 / 1000) - 1;
	TIMSK0 |= (1 << OCIE0A);
}

void Scheduler::start() {
	sei();
}

void Scheduler::addTask(void (*taskFunc)(), uint8_t priority, uint16_t period_ms) {
	if (!taskFunc || priority >= MAX_PRIORITY)
	return;

	for (uint8_t i = 0; i < MAX_TASKS; ++i) {
		if (tasks[i].active && tasks[i].func == taskFunc) return;
	}

	for (uint8_t i = 0; i < MAX_TASKS; ++i) {
		if (!tasks[i].active) {
			tasks[i] = {
				.func = taskFunc,
				.priority = priority,
				.period = period_ms,
				.counter = 0,
				.ready = false,
				.active = true,
				.missedDeadline = false,
				.oneShot = false
			};
			return;
		}
	}
}

void Scheduler::removeTask(void (*taskFunc)()) {
	for (uint8_t i = 0; i < MAX_TASKS; ++i) {
		if (tasks[i].active && tasks[i].func == taskFunc) {
			tasks[i].active = false;
			return;
		}
	}
}

void Scheduler::setTimeout(void (*taskFunc)(), uint16_t delay_ms) {
	if (!taskFunc || taskCount >= MAX_TASKS)
	return;

	for (uint8_t i = 0; i < MAX_TASKS; ++i) {
		if (!tasks[i].active) {
			tasks[i] = {
				.func = taskFunc,
				.priority = 0,        // Highest for one-shot
				.period = delay_ms,
				.counter = 0,
				.ready = false,
				.active = true,
				.missedDeadline = false,
				.oneShot = true
			};
			taskCount++;
			return;
		}
	}
}

volatile uint32_t schedulerTickCount = 0;  // Add this at global level


void Scheduler::tick() {
	
	    schedulerTickCount++;  // <-- Add this line to maintain global tick count

	for (uint8_t i = 0; i < MAX_TASKS; ++i) {
		if (tasks[i].active) {
			tasks[i].counter++;
			if (tasks[i].counter >= tasks[i].period) {
				// Missed deadline?
				if (tasks[i].ready)
				tasks[i].missedDeadline = true;

				tasks[i].counter = 0;
				tasks[i].ready = true;
			}
		}
	}
}

void Scheduler::run() {
	for (uint8_t p = 0; p < MAX_PRIORITY; ++p) {
		for (uint8_t i = 0; i < MAX_TASKS; ++i) {
			if (tasks[i].active && tasks[i].ready && tasks[i].priority == p) {
				tasks[i].ready = false;
				tasks[i].missedDeadline = false;
				tasks[i].func();

				if (tasks[i].oneShot) {
					tasks[i].active = false;
				}
			}
		}
	}
}

void Scheduler::debugTaskMonitor() {
	Serial3.println("=== Scheduler Task Monitor ===");
	for (uint8_t i = 0; i < MAX_TASKS; ++i) {
		if (tasks[i].active) {
			Serial3.print("Task["); Serial3.print(i); Serial3.print("]: ");
			Serial3.print("Prio="); Serial3.print(tasks[i].priority);
			Serial3.print(" | Period="); Serial3.print(tasks[i].period);
			Serial3.print(" | Cnt="); Serial3.print(tasks[i].counter);
			Serial3.print(" | Ready="); Serial3.print(tasks[i].ready);
			Serial3.print(" | Missed="); Serial3.print(tasks[i].missedDeadline);
			Serial3.print(" | OneShot="); Serial3.println(tasks[i].oneShot);
		}
	}
	Serial3.println("================================");
}

ISR(TIMER0_COMPA_vect) {
	scheduler.tick();
}


extern volatile uint32_t schedulerTickCount;

bool vTaskDelay(uint16_t delayTicks) {
	static uint32_t lastTick = 0;

	if ((uint32_t)(schedulerTickCount - lastTick) >= delayTicks) {
		lastTick = schedulerTickCount;
		return true;
	}
	return false;
}

bool vTaskDelayUntil(uint32_t* lastWakeTick, uint16_t periodTicks) {
	if ((uint32_t)(schedulerTickCount - *lastWakeTick) >= periodTicks) {
		*lastWakeTick += periodTicks;
		return true;
	}
	return false;
}

void Scheduler::begin() {
	init();
	addTask(blinkTask, 2, 150); 
	addTask(uart3Task, 1, 1000);
	addTask(lcdTask,3, 1000);
	addTask(ADCTask,1,1000);
	start();
}
