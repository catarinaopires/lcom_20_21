#pragma once
#include <stdint.h>
#include "i8254.h"

/** @brief Starts counter from 0 with the desired frequency
 * @param timer Timer to configure (0-2)
 * @param frequency Counter frequency 
 * @return 0 in case of success, 1 otherwise
*/
int timer_start_counter(timer_nr timer, uint8_t frequency);
