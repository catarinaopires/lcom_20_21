#include "rtc.h"
#include "../common/utils.h"

/*To access to a a particular register in the array,
  must first write the address of that register to port 0x70.
  Then, depending on the type of access, is should either write to, or read from, port 0x71, the new contents,
  or the existing contents, respectively, of the register in the array.*/


int rtc_write(uint8_t port, uint8_t address){

  if(sys_outb(port, address) != OK)
    return 1;
  return 0;
}

int rtc_read(uint8_t port, uint8_t* value){
  if(util_sys_inb(port, value) != OK)
    return 1;
  return 0;
}

int rtc_get_value(uint8_t reg){
  if(rtc_write(RTC_ADDR_REG, reg) != 0)
    return -1;

  uint8_t value;
  if(rtc_read(RTC_DATA_REG, &value) != 0)
    return -1;

  // Convert BCD to decimal
  bcd_to_decimal(&value);

  return value;
}


// printf("TIME: %d:%d:%d", rtc_get_value(RTC_HOURS_REG), rtc_get_value(RTC_MINUTES_REG), rtc_get_value(RTC_SECONDS_REG));

/*
In order to enable interrupts, after an event has generated an interrupt,
  it is necessary to read Register C, i.e. the register in the array at address 12.*/

