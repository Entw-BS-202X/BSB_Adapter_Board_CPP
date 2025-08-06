#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

#define MAX_TASKS     8
#define MAX_PRIORITY  10

class Scheduler {
	public:
	void init();
	void start();
	void begin();
	void tick();
	void run();

	void addTask(void (*taskFunc)(), uint8_t priority, uint16_t period_ms);
	void removeTask(void (*taskFunc)());
	void setTimeout(void (*taskFunc)(), uint16_t delay_ms); // One-shot

	void debugTaskMonitor();  // Print task states

	private:
	struct Task {
		void (*func)();       // Task function pointer
		uint8_t priority;     // Priority: 0 = highest
		uint16_t period;      // Period in ms (0 = one-shot)
		uint16_t counter;     // Counter for timing
		bool ready;           // Task ready to run
		bool active;          // If this slot is in use
		bool missedDeadline;  // True if task was skipped
		bool oneShot;         // True if setTimeout() task
	};

	Task tasks[MAX_TASKS];
	uint8_t taskCount = 0;

	void markMissedDeadlines();
};


/**
 * @brief Delay the task execution for N ticks (non-blocking).
 *
 * @param delayTicks Delay in scheduler ticks (1 tick = 1ms)
 * @return true if delay has passed
 */
bool vTaskDelay(uint16_t delayTicks);

/**
 * @brief Run the task periodically with fixed interval (like FreeRTOS).
 *
 * @param lastWakeTick Pointer to stored last tick
 * @param periodTicks Period in ticks (ms)
 * @return true if it's time to execute
 */
bool vTaskDelayUntil(uint32_t* lastWakeTick, uint16_t periodTicks);

extern Scheduler scheduler;

#endif /* SCHEDULER_H_ */