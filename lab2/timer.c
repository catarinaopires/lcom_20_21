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
    u8_t rbword = 0;
    uint8_t port;

    // Create Read-Back Word
    rbword | TIMER_RB_CMD | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);

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
        return 0;
    }

    // Write Read-Back word to control regiter (0x43)
    if (sys_outb(TIMER_CTRL, rbword))
        return 1;

    // Reads timer status
    // util_sys_inb calls sys_inb with st variable transformed into uint32_t
    if(util_sys_inb(port, st))
        return 1;

    return 0;
    printf("%s\n", _func_);
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                         enum timer_status_field field) {

    union timer_status_field_val conf;

    // BCD
    conf.bcd = st && BIT(0);

    // Counting Mode
    conf.count_mode = st && (BIT(3)| BIT(2) | BIT(1));

    // Type of access
    // ADD TO UNION: CONF.TIMER_INIT ???
    if(st && (BIT(4)| BIT(5)) == TIMER_LSB)
        // TODO: Change info in union
    if(st && (BIT(4)| BIT(5)) == TIMER_MSB)
        // TODO: Change info in union
    if(st && (BIT(4)| BIT(5)) == TIMER_LSB_MSB)
        // TODO: Change info in union

    // Byte ????


    timer_print_config(timer, field, conf);

    printf("%s is not yet implemented!\n", _func_);
    return 1;
}
