#pragma once
#include <stdint.h>
#include "../program_options.h"

/** @brief Data type to save occurrences */
typedef uint32_t counter_type;

/** @brief Struct to save several counters
 *  Simultaneous counters possible, edit the number of them in program options (COUNTERS_SIMULTANEOUSLY_ACTIVE)
*/
typedef struct counters{
    counter_type* active_counters[COUNTERS_SIMULTANEOUSLY_ACTIVE];
    counter_type* inactive_counters[COUNTERS_SIMULTANEOUSLY_ACTIVE];
    int8_t stored_act;
    int8_t stored_inact;
} counters;

/** @brief Function to create the counters struct
 * @return Pointer to the newly counters struct
*/
counters* counters_counters_initialize();

/** @brief Function to add counters
 * @return Pointer to the created counter
*/
counter_type* counters_counter_init();

/** @brief Function to increase the count
 * @param counter Counter manipulation
*/ 
void counters_counter_increase(counter_type* counter);

/** @brief Function to stop the count in a determined counter
 * @param counters Counters Struct
 * @param counter Counter to stop
*/ 
void counters_counter_stop(counters* timers, counter_type* counter);

/** @brief Function to get the time delta in seconds from interrupt counter
 * @param counter Counter to read from, needs to be stopped
 * @param frequency Frequency of the counter
*/
float counters_get_seconds(counter_type* counter, uint32_t frequency);

/** @brief Function to destroy one specific counter
 * @param counters Timers Struct
 * @param counter Counter to destroy
*/ 
void counters_counter_destructor(counters* counters, counter_type* counter);

/** @brief Function to destroy the complete struct array
 * @param counters Counters struct
*/
void counters_counters_destructor(counters* counters);
