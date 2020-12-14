#pragma once
#include <stdint.h>
#include "../program_options.h"

/** @brief Data type to save intervals */
typedef uint32_t time_delta;

/** @brief Struct to save several timers
 *  Simultaneous timers possible, edit the number of them in program options (TIMERS_SIMULTANEOUSLY_ACTIVE)
*/
typedef struct timers{
    time_delta* active_timers[TIMERS_SIMULTANEOUSLY_ACTIVE];
    time_delta* inactive_timers[TIMERS_SIMULTANEOUSLY_ACTIVE];
    int8_t stored_act;
    int8_t stored_inact;
} timers;

/** @brief Function to create the timers struct
 * @return Pointer to the newly timers struct
*/
timers* timer_timers_initialize();

/** @brief Function to add timers
 * @return Pointer to the created counter
*/
time_delta* timer_counter_init();

/** @brief Function to increase the time count
 * @param timers Timers Struct
 * @param counter Counter manipulation
*/ 
void timer_counter_increase(time_delta* counter);

/** @brief Function to stop the count in a determined counter
 * @param timers Timers Struct
 * @param counter Counter to stop
*/ 
void timer_counter_stop(timers* timers, time_delta* counter);

/** @brief Function to get the time delta in seconds
 * @param counter Counter to read from, needs to be stopped
 * @param frequency Frequency of the counter
*/
float timer_counter_seconds(time_delta* counter, uint32_t frequency);

/** @brief Function to destroy one specific counter
 * @param timers Timers Struct
 * @param counter Counter to destroy
*/ 
void timer_counter_destructor(timers* timers, time_delta* counter);

/** @brief Funtion to destroy the complete struct array
 * @param The timers struct
*/
void timer_timers_destructor(timers* timers);
