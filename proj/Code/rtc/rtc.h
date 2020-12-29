#pragma once
#include <lcom/lcf.h>


#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_SECONDS_REG 0x00
#define RTC_MINUTES_REG 0x02
#define RTC_HOURS_REG 0x04



int rtc_write(uint8_t port, uint8_t address);
int rtc_read(uint8_t port, uint8_t* value);
int rtc_get_value(uint8_t reg);
