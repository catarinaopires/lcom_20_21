#pragma once
#include <lcom/lcf.h>

int (utils_sys_inb8)(int port, uint8_t *value);
int (utils_sys_inb16)(int port, uint16_t *value);
uint8_t (utils_get_LSB)(uint16_t val);

/** @brief Function to get the minimum value
 * @param val1 1st value
 * @param val2 2nd value
 * @return The smallest of val1 and val2
 * */
int32_t (utils_min)(int32_t val1, int32_t val2);

