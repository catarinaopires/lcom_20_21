#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"



int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

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
    rbword | TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);


    // Write Read-Back word to control regiter (0x43)
    if (sys_outb(TIMER_CTRL, rbword) != 0)
        return 1;

    // Reads timer status
    // util_sys_inb calls sys_inb with st variable transformed into uint32_t
    if(util_sys_inb(port, st) != 0)
        return 1;

    return 0;
    printf("%s\n", _func_);
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                         enum timer_status_field field) {

    union timer_status_field_val timer_conf;

    // Invalid timer
    if(!(timer >= 0 && timer <= 2))
        return 1;

    switch(field){

      case(tsf_all):
        timer_conf.byte = st;
        break;

      case(tsf_initial):
        uint8_t mask = 0x30;
        mask = st & mask;

        if(mask == TIMER_LSB_MSB){
          timer_conf.in_mode = MSB_after_LSB;
        }
        else if (mask == TIMER_MSB){
          timer_conf.in_mode = MSB_only;
        }
        else if (mask == TIMER_LSB){
          timer_conf.in_mode = LSB_only;
        }
        else
          timer_conf.in_mode = INVAL_val;
        break;

      case(tsf_mode):
        uint8_t mask = 0x0D;
        mask = st & mask;
        mask >> 1;

        //Handle dc bit
        if(mask > 5){
          timer_conf.count_mode = mask - 4;
        }
        else {
          timer_conf.count_mode = mask;
        }
        break;

      case(tsf_base):
        timer_conf.bcd = (st & TIMER_BCD);
        break;
    };

    timer_print_config(timer, field, timer_conf);


    return 0;
}
