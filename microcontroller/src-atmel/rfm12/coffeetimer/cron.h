#include <stdint.h>
#pragma once

#define NUM_CRONJOBS 16

typedef struct
{
	uint16_t time;
	uint16_t func_param;
	void ((*func)());
} cronjob_t;

/* @description Add a job to cron's queue. 
 * @param The first param is the unit of ($time) after which the function should be executed.
 * @param The second param is the function to be executed.
 * @param The last param is the parameter copied to the function.
 * */
uint8_t cron_add (uint16_t, void ((*in_function)()), uint16_t);

/* @description initialize this cruft */
void cron_init();

/* @description This function handles the task queue. The task_queue function is used to detach
 * the function called by cron from the timer interrupt so it can be run in your main loop instead
 * of the timer context.
 *
 * @param The first parameter is the function pointer of the function that should be executed. If
 * that parameter is (numerical) zero, the function is this functions stack is executed.
 *
 * @param The second parameter is the parameter that is handed over to the called function.
 *
 * @return task_queue returns 1 on success, 0 on failure. When pushing a function pointer to task_queue()'s
 * function stack succeeds, it returns 1 - if the queue is full and the function was not added, it returns 0.
 * If you call this function with 0 as first param, the return value is 0 if the task queue was empty and
 * therefore no function has been executed. If the function was executed, it returns 1.
 */
uint8_t task_queue (void ((*in_func)(uint16_t)), uint16_t);

/* @description Periodic tick function. Execute this every second (or any other unit of $time you may
 * choose)
 */
void cron_tick();
