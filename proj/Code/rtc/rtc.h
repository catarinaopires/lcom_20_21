#pragma once
#include <lcom/lcf.h>

// Define Real Time Clock Controller (RTC) IRQ
#define RTC_IRQ 8                   // RTC IRQ

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_SECONDS_REG 0
#define RTC_SECONDS_ALARM_REG 1
#define RTC_MINUTES_REG 2
#define RTC_MINUTES_ALARM_REG 3
#define RTC_HOURS_REG 4
#define RTC_HOURS_ALARM_REG 5

#define RTC_REGISTER_A 10
#define RTC_UIP BIT(7)

#define RTC_REGISTER_B 11
#define RTC_SET BIT(7)
#define RTC_AIE BIT(5)              // Bit to enable alarm interrupts

#define RTC_REGISTER_C 12
#define RTC_UF BIT(4)
#define RTC_AF BIT(5)
#define RTC_PF BIT(6)

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

void wait_valid_rtc(void);
int rtc_set_flag(uint8_t reg, uint8_t flag, uint8_t activate);
int rtc_ih(void);
uint8_t handle_alarm_int();
uint8_t rtc_config_alarm(uint8_t hours, uint8_t min, uint8_t sec);
