#include "i8042.h"

// Global variable to store output data of the kbc
extern uint8_t OUTPUT_BUFF_DATA;

void(kbc_ih)(void) {
  uint8_t st;
  uint8_t data;
  util_sys_inb(KBC_ST_REG, &st); /* assuming it returns OK */


  if(st & KBC_OUTPUT_BUFF_FULL) {
    util_sys_inb(KBC_OUT_BUF, &data);
  }
  if ( (st &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 ) {
    OUTPUT_BUFF_DATA = data;
  }
  else
    OUTPUT_BUFF_DATA = 0;
}
