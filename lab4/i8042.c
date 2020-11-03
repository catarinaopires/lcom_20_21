#include "i8042.h"
#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <minix/sysutil.h>


void(kbc_ih)() {
  uint8_t st;
  uint8_t data;
  if(util_sys_inb(KBC_ST_REG, &st) != OK)
    return;

  if(st & KBC_OUTPUT_BUFF_FULL) {
    if(util_sys_inb(KBC_OUT_BUF, &data) != OK)
      return;
  }
  if ( (st &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 ) {
    OUTPUT_BUFF_DATA = data;
  }
  else
    OUTPUT_BUFF_DATA = 0;
}

int is_make_code(){
  if(OUTPUT_BUFF_DATA & KBC_BREAKCODE_FLAG){
    return 0;
  }
  return 1;
}

int (kbc_read_poll)(){

  uint8_t st;
  uint8_t data = 0;

  while( 1 ) {
    if(util_sys_inb(KBC_ST_REG, &st) != OK){
      return 1;
    } /* loop while 8042 output buffer is empty */

   // Check if OBF is full & if it came from kbd
    if(st & (KBC_OUTPUT_BUFF_FULL | !KBC_AUX)) {
      if(util_sys_inb(KBC_OUT_BUF, &data) != OK)
        return 1;
      if ( (st &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 ){
        OUTPUT_BUFF_DATA = data;
        break;
      }
    }
    else
      tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 0;

}

int (mouse_enable_data_reporting_ours)(){
  uint8_t stat = 0;
  uint8_t ack = 0;

  while(1){
    if(util_sys_inb(KBC_ST_REG, &stat) != OK)
      return 1;

    if((stat & KBC_INPUT_BUFF_FULL) == 0){
      if(sys_outb(KBC_CMD_REG, KBC_CMD_WRITE) != OK)
        return 1;

      while(1){
        if(util_sys_inb(KBC_ST_REG, &stat) != OK)
          return 1;

        if((stat & KBC_INPUT_BUFF_FULL) == 0){
          if(sys_outb(KBC_CMD_REG, KBC_WRITE_MOUSE) != OK)
            return 1;

          if(sys_outb(KBC_CMD_ARGS, KBC_ENABLE_DATA_REP_STR) != OK)
            return 1;
          while(1){
            if(util_sys_inb(KBC_OUT_BUF, &ack) != OK)
              return 1;

            if(ack == KBC_MOUSE_ACK)
              return 0;
            else
              break;
          }
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
      }
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
}

void (mouse_ih)(void){
  kbc_ih();
}

uint16_t (to16Cpl2)(uint8_t value){
  uint16_t rtv = 0;
  if(BIT(7) & value){
    rtv = 0xFF00;
  }
  return (rtv | value);
}

struct packet (mouse_process_packet)(uint8_t bytes[]){
  struct packet processedPacket;
  processedPacket.bytes[0] = bytes[0];
  processedPacket.bytes[1] = bytes[1];
  processedPacket.bytes[2] = bytes[2];

  processedPacket.rb = (bytes[0] & MOUSE_RB);
  processedPacket.lb = (bytes[0] & MOUSE_LB);
  processedPacket.mb = (bytes[0] & MOUSE_MB);

  processedPacket.x_ov = (bytes[0] & MOUSE_X_OVF);
  processedPacket.y_ov = (bytes[0] & MOUSE_Y_OVF);

  processedPacket.delta_x = to16Cpl2(bytes[1]);
  processedPacket.delta_y = to16Cpl2(bytes[2]);

  return processedPacket;
}
