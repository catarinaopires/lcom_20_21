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

int i8254_parse_irq(timer_nr timer){
  switch (timer){
  case timer0:
    return TIMER0_IRQ;

  default:
    return -1;    
  }
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

int i8042_get_conf(timer_nr timer, uint8_t *st) {
    uint8_t rbword = 0;
    uint8_t port = i8042_parse_port(timer);

    if(port == -1){
      printf("Invalid timer port!");
      return 1;
    }

    // Create Read-Back Word
    rbword = rbword | TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);


    // Write Read-Back word to control regiter (0x43)
    if (sys_outb(TIMER_CTRL, rbword) != OK) {
      printf("Error in sys_outb() in i8042_get_conf()");
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



int i8042_set_frequency(timer_nr timer, uint32_t freq) {
  
  // Invalid timer
  if(!(timer >= 0 && timer <= 2))
    return 1;

  // Change the operating frequency of a timer
  uint16_t freqValue = TIMER_FREQ / freq;

  // Get current configuration, and keep LSB
  uint8_t maskMSB = 0x0F;
  uint8_t controlWord = 0;
  i8042_get_conf(timer, &controlWord);
  controlWord = (controlWord & maskMSB);

  // Assemble new control word and write it to ctrl register
  int8_t timerCtrlSel = i8242_parse_selection(timer);
  int timerPort = i8042_parse_port(timer);
  if(timerPort == -1 || timerCtrlSel == -1){
    printf("Invalid port number in i8042_set_frequency");
    return 1;
  }

  controlWord = controlWord | timerCtrlSel | TIMER_LSB_MSB;

  if(sys_outb(TIMER_CTRL, controlWord) != OK) {
    printf("Error in sys_outb() timer_set_frequency()");
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

/*
int (timer_display_conf)(uint8_t timer, uint8_t st,
                         enum timer_status_field field) {

  union timer_status_field_val timer_conf;

  // Invalid timer
  if(!(timer >= 0 && timer <= 2))
    return 1;

  uint8_t maskInit = 0x30;
  uint8_t maskMode = 0x0E;

  switch(field){
    case(tsf_all):
      timer_conf.byte = st;
      break;

    case(tsf_initial):

      maskInit = st & maskInit;

      if(maskInit == TIMER_LSB_MSB){
          timer_conf.in_mode = MSB_after_LSB;
        }
      else if (maskInit == TIMER_MSB){
          timer_conf.in_mode = MSB_only;
        }
      else if (maskInit == TIMER_LSB){
          timer_conf.in_mode = LSB_only;
        }
      else
        timer_conf.in_mode = INVAL_val;
      break;

    case(tsf_mode):

      maskMode = st & maskMode;
      maskMode = maskMode >> 1;

      // Handle DC bit
      if(maskMode > 5){
          timer_conf.count_mode = maskMode - 4;
        }
      else {
          timer_conf.count_mode = maskMode;
        }
      break;

    case(tsf_base):
      timer_conf.bcd = (st & TIMER_BCD);
      break;
  };

  timer_print_config(timer, field, timer_conf);

  return 0;
}
*/
