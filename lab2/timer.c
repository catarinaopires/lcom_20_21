#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"



int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  // Invalid timer
  if(!(timer >= 0 && timer <= 2))
    return 1;

  // Change the operating frequency of a timer
  uint16_t  freqValue = TIMER_FREQ / freq;

  // Get current configuration, and keep LSB
  uint8_t maskMSB = 0x0F;
  uint8_t controlWord = 0;
  timer_get_conf(timer, &controlWord);
  controlWord = (controlWord & maskMSB);

  // Assemble new control word and write it to ctrl register
  uint8_t timerCtrlSel;
  uint8_t timerPort;
  switch(timer){
    case 0:
      timerCtrlSel = TIMER_SEL0;
      timerPort = TIMER_0;
      break;

    case 1:
      timerCtrlSel = TIMER_SEL1;
      timerPort = TIMER_1;
      break;

    case 2:
      timerCtrlSel = TIMER_SEL2;
      timerPort = TIMER_2;
      break;
  };

  controlWord = controlWord | timerCtrlSel | TIMER_LSB_MSB;

  if(sys_outb(TIMER_CTRL, controlWord))
    return 1;

  // Write freq value in the selected timer

  uint8_t lsb;
  uint8_t msb;
  util_get_LSB(freqValue, &lsb);
  util_get_MSB(freqValue, &msb);

  if(!sys_outb(timerPort,lsb)){
    if(!sys_outb(timerPort, msb))
      return 0;
  }

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    uint8_t rbword = 0;
    uint8_t port;

    // Check if given timer is valid
    if(timer == 0){
    port = TIMER_0;
    }
    else if(timer == 1){
    port = TIMER_1;
  }
    else if(timer == 2){
    port = TIMER_2;
  }
    else{
    return 1;
    }

    // Create Read-Back Word
    rbword = rbword | TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);


    // Write Read-Back word to control regiter (0x43)
    if (sys_outb(TIMER_CTRL, rbword))
        return 1;

    // Reads timer status
    // util_sys_inb calls sys_inb with st variable transformed into uint32_t
    if(util_sys_inb(port, st))
        return 1;

    return 0;
    printf("%s\n",  __func__);
}

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
