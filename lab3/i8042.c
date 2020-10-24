#include "i8042.h"
#include <lcf/lcf.h>

void kbc_ih() {
  uint8_t st;
  uint8_t data;
  while( 1 ) {
    util_sys_inb(KBC_ST_REG, &st); /* assuming it returns OK */
  /* loop while 8042 output buffer is empty */
    if(stat & KBC_OUTPUT_BUFF_FULL) {
      util_sys_inb(KBC_OUT_BUF, &data);
    }
    if ( (stat &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 )
      OUTPUT_BUFF_DATA = data;

  }
  tickdelay(micros_to_ticks(WAIT_KBC));                           // Wait for KBC
}

int is_make_code(){
  if(OUTPUT_BUFF_DATA & KBC_BREAKCODE_FLAG){
    return 0;
  }
  return 1;
}