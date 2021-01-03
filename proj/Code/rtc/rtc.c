#include "../common/utils.h"
#include "rtc.h"

int rtc_write(uint8_t port, uint8_t address){
  wait_valid_rtc();

  if(sys_outb(RTC_ADDR_REG, port) != OK)
    return 1;

  if(sys_outb(RTC_DATA_REG, address) != OK)
    return 1;
  return 0;
}

int rtc_read(uint8_t port, uint8_t* value){
  wait_valid_rtc();

  if(sys_outb(RTC_ADDR_REG, port) != OK)
    return 1;

  if(util_sys_inb(RTC_DATA_REG, value) != OK)
    return 1;
  return 0;
}

void wait_valid_rtc(void) {
  uint8_t regA = 0;
  do {
    //disable();
    sys_outb(RTC_ADDR_REG, RTC_REGISTER_A);
    util_sys_inb(RTC_DATA_REG, &regA);
    //enable();
  } while (regA & RTC_UIP);
}

int rtc_get_value(uint8_t reg){
  uint8_t value;
  if(rtc_read(reg, &value) != 0)
    return -1;

  // Convert BCD to decimal
  bcd_to_decimal(&value);

  return value;
}

int rtc_set_flag(uint8_t reg, uint8_t flag, uint8_t activate){
  uint8_t value;

  if(rtc_read(reg, &value) != 0)
    return 1;

  if(!activate)
    value = value & ~flag;
  else
    value |= flag;

  if(rtc_write(reg, value) != 0)
    return 1;
  return 0;
}

uint8_t rtc_config_alarm(uint8_t hours, uint8_t min, uint8_t sec){

  // Config seconds for alarm
  decimal_to_bcd(&sec);
  if(rtc_write(RTC_SECONDS_ALARM_REG, sec) != 0)
    return 1;

  // Config minutes for alarm
  decimal_to_bcd(&min);
  if(rtc_write(RTC_MINUTES_ALARM_REG, min) != 0)
    return 1;

  // Config hours for alarm
  decimal_to_bcd(&hours);
  if(rtc_write(RTC_HOURS_ALARM_REG, hours) != 0)
    return 1;

  return 0;
}

void rtc_calculate_finish_alarm(int* hours, int* min, int* sec) {

  int h = rtc_get_value(RTC_HOURS_REG);
  int m = rtc_get_value(RTC_MINUTES_REG);
  int s = rtc_get_value(RTC_SECONDS_REG);

  int secs = s + *sec % 60;
  int mins = (m + *min) % 60 + (s + *sec) / 60;
  int hs = (h + *hours) % 24 + (m + *min) / 60;

  *hours = hs;
  *min = mins;
  *sec = secs;

}

int rtc_ih(void) {
  uint8_t regC;

  sys_outb(RTC_ADDR_REG, RTC_REGISTER_C);
  if (util_sys_inb(RTC_DATA_REG, &regC) != 0)
    return 1;

  if( regC & RTC_UF )
    return 1;
  if( regC & RTC_AF )
    return 0;
  if( regC & RTC_PF )
    return 1;
  return 1;
}

