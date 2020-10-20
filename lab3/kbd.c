#include "kbd.h"
int HOOK_ID = 0;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  HOOK_ID = *bit_no;
  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &HOOK_ID) != OK){
    printf("%s !\n", __func__);
    return 1;
  }
  return 0;
}

int(kbd_unsubscribe_int)(){
  if(sys_irqrmpolicy(&HOOK_ID) != OK){
    printf("%s !\n", __func__);
      return -1;
  }
  return 0;
}

void (kbc_ih)() {
  uint16_t data;
  while( 1 ) {
    util_sys_inb8(KBC_ST_REG, &ST); /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if( ST & KBC_OBF ) {
      if(util_sys_inb16(KBC_OUT_BUF, &data) != OK){
        printf("%s sys_inb\n", __func__);
      }
    }
    if ((ST &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 )
      OUTPUT_BUFFER_DATA = data;
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
}
