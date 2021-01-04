#pragma once

#include <lcom/utils.h>
#include "video/video.h"
#include "video/extImages.h"


// Define Real Time Clock Controller (RTC) IRQ
#define RTC_IRQ 8                   // RTC IRQ

#define RTC_ADDR_REG 0x70           // Register that receives addresses
#define RTC_DATA_REG 0x71           // Register used to transfer data

#define RTC_SECONDS_REG 0           // Address to access seconds
#define RTC_SECONDS_ALARM_REG 1     // Address to access seconds for alarm
#define RTC_MINUTES_REG 2           // Address to access minutes
#define RTC_MINUTES_ALARM_REG 3     // Address to access minutes for alarm
#define RTC_HOURS_REG 4             // Address to access hours
#define RTC_HOURS_ALARM_REG 5       // Address to access hours for alarm

#define RTC_REGISTER_A 10           // Control register to control RTC: Register A
#define RTC_UIP BIT(7)              // Flag of Register A (if set to 1, do not access time/date)

#define RTC_REGISTER_B 11           // Control register to control RTC: Register B
#define RTC_SET BIT(7)              // Flag of Register B (if set to inhibits updates of time/date registers)
#define RTC_AIE BIT(5)              // Flag of Register B (Bit to enable alarm interrupts)

#define RTC_REGISTER_C 12           // Control register to control RTC: Register C
#define RTC_UF BIT(4)               // Flag of Register C (if set to 1, update interrupt pending)
#define RTC_AF BIT(5)               // Flag of Register C (if set to 1, alarm interrupt pending)
#define RTC_PF BIT(6)               // Flag of Register C (if set to 1, periodic interrupt pending)

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

/** @brief Waits fir valid Time/Date.
 */
void wait_valid_rtc(void);

/** @brief Reads 8 bit value from given reg.
 * @param reg Register to read from
 * @return Value read from register
 */
int rtc_get_value(uint8_t reg);

/** @brief Sets flag in certain register depending if is to enable or disable.
 * @param reg Register to alter flag
 * @param flag Flag (bit) to be changed in register
 * @param activate 1 if is it to enable flag, 0 to disable
 * @return Returns 0 upon success, 1 with failure
 */
int rtc_set_flag(uint8_t reg, uint8_t flag, uint8_t activate);

/** @brief Configures alarm. Alarm set to hours:min:sec.
 * @param hours Hours of alarm
 * @param min Minutes of alarm
 * @param sec Seconds of alarm
 * @return Returns 0 upon success, 1 with failure
 */
uint8_t rtc_config_alarm(uint8_t hours, uint8_t min, uint8_t sec);

/** @brief Calculates time of the alarm according to wanted time interval.
 * @param hours Hours to add to current time. Modified in the function to hours for alarm to set
 * @param min Minutes to add to current time. Modified in the function to minutes for alarm to set
 * @param sec Seconds to add to current time. Modified in the function to seconds for alarm to set
 */
void rtc_calculate_finish_alarm(int* hours, int* min, int* sec);

/** @brief RTC interruption handler. Checks the origin of the interrupts and acts according to it.
 * @return Returns 0 if interruption was of an alarm, 1 otherwise
 */
int rtc_ih(void);
