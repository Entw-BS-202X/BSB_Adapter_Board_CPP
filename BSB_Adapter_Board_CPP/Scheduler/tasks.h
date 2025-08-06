#ifndef TASKS_H_
#define TASKS_H_

#ifdef __cplusplus
extern "C" {
	#endif

	// Function declarations for scheduled tasks
	void blinkTask(void);
	void uart3Task(void);
	void lcdTask(void);

	#ifdef __cplusplus
}
#endif

#endif /* TASKS_H_ */
