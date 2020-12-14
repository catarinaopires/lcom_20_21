#pragma once
#include <lcom/lcf.h>

/** @brief Reads 8 bit value from given port.
 * @param port Port to read from
 * @param value Value read in port
 * @return Returns 0 upon sucess, 1 with failiure
 */
int (util_sys_inb)(int port, uint8_t *value);

/** @brief Gets LSB (less significant bits) of value 'val'.
 * @param val Value to get LSB from
 * @return Returns 0 upon sucess, 1 with failiure
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/** @brief Gets MSB (most significant bits) of value 'val'.
 * @param val Value to get MSB from
 * @param msb MSB of value 'val'
 * @return Returns 0 upon sucess, 1 with failiure
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/** @brief Function to get the minimum value.
 * @param val1 1st value
 * @param val2 2nd value
 * @return The smallest of val1 and val2
 */
int32_t (utils_min)(int32_t val1, int32_t val2);

