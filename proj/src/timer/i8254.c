#include "i8254.h"
#include <lcom/lcf.h>

int i8254_parse_port(timer_nr timer){
  switch(timer){
    case timer0:
      return TIMER_0;
    
    case timer1:
      return TIMER_1;

    case timer2:
      return TIMER_2;

    case timerCtrl:
      return TIMER_CTRL;

    default:
      return -1;      
  };
}

int8_t i8254_parse_selection(timer_nr timer){
  switch(timer){
    case timer0:
      return TIMER_SEL0;
    
    case timer1:
      return TIMER_SEL1;

    case timer2:
      return TIMER_SEL2;

    default:
      return -1;      
  };
}

uint8_t i8254_get_control_word(timer_nr timer, timer_rw_ops rw, timer_mode mode, timer_format format){
  uint8_t ctrlWord = 0;

  int8_t timerCtrlSel = i8254_parse_selection(timer);
  int timerPort = i8254_parse_port(timer);
  if(timerPort == -1 || timerCtrlSel == -1){
    printf("Invalid port number in i8042_set_frequency");
    return 1;
  }

  ctrlWord = ctrlWord | timerCtrlSel | rw | mode | format;
  return ctrlWord;
}

int i8254_get_conf(timer_nr timer, uint8_t *st) {
    uint8_t rbword = 0;
    int8_t port = i8254_parse_port(timer);

    if(port == -1){
      printf("Invalid timer port!");
      return 1;
    }

    // Create Read-Back Word
    rbword = rbword | TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);


    // Write Read-Back word to control regiter (0x43)
    if(i8254_write_command(rbword)){
      return 1;
    }

    // Reads timer status
    // util_sys_inb calls sys_inb with st variable transformed into uint32_t
    if(util_sys_inb(port, st)) {
      printf("Error in utils_sys_inb() in i8042_get_conf()");
      return 1;
    }

    return 0;
}

int i8254_write_command(uint8_t ctrlwd){
  if(sys_outb(TIMER_CTRL, ctrlwd) != OK) {
    printf("Error in sys_outb() for timer");
    return 1;
  }
  return 0;
}

int i8254_set_frequency(timer_nr timer, uint32_t freq) {
  // Change the operating frequency of a timer
  uint16_t freqValue = TIMER_FREQ / freq;

  // Get current configuration, and keep LSB
  uint8_t maskMSB = 0x0F;
  uint8_t controlWord = 0;
  i8254_get_conf(timer, &controlWord);
  controlWord = (controlWord & maskMSB);

  // Assemble new control word and write it to ctrl register
  int8_t timerCtrlSel = i8254_parse_selection(timer);
  int timerPort = i8254_parse_port(timer);
  if(timerPort == -1 || timerCtrlSel == -1){
    printf("Invalid port number in i8042_set_frequency");
    return 1;
  }

  controlWord = controlWord | timerCtrlSel | TIMER_LSB_MSB;

  if(i8254_write_command(controlWord)){
    return 1;
  }

  // Write freq value in the selected timer

  uint8_t lsb;
  uint8_t msb;
  util_get_LSB(freqValue, &lsb);
  util_get_MSB(freqValue, &msb);

  if(sys_outb(timerPort,lsb) == OK){
    if(sys_outb(timerPort, msb) == OK)
      return 0;
    else
      printf("Error in sys_outb(msb) timer_set_frequency()");
  }
  else
    printf("Error in sys_outb(lsb) timer_set_frequency()");

  return 1;
}
