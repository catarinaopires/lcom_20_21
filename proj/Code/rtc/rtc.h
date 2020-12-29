#pragma once
#include <lcom/lcf.h>


#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_SECONDS_REG 0x00
#define RTC_MINUTES_REG 0x02
#define RTC_HOURS_REG 0x04


/** @brief Writes 8 bit value (address) to given port.
 * @param port Port to write to
 * @param address Value to be written in port
 * @return Returns 0 upon success, 1 with failure
 */
int rtc_write(uint8_t port, uint8_t address);

/** @brief Reads 8 bit value from given port.
 * @param port Port to read from
 * @param value Value to be read from port (Modified in function)
 * @return Returns 0 upon success, 1 with failure
 */
int rtc_read(uint8_t port, uint8_t* value);

/** @brief Reads 8 bit value from given reg.
 * @param reg Register to read from
 * @return Value read from register
 */
int rtc_get_value(uint8_t reg);
