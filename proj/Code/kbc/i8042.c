#include "i8042.h"

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

int is_make_code(){
  if(OUTPUT_BUFF_DATA & KBC_BREAKCODE_FLAG){
    return 0;
  }
  return 1;
}

/*
int (kbc_read_poll)(){

  uint8_t st;
  uint8_t data = 0;

  while( 1 ) {
    if(util_sys_inb(KBC_ST_REG, &st) != OK){
      return 1;
    } // loop while 8042 output buffer is empty

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
*/

void check_movement(uint8_t bytes[], direction* dir){
    // TODO: NONE (SAME DIRECTION EX UP+DOWN) - THEN RELEASE - ERROR

    switch (*dir) {
        case right:
            if(bytes[1] == KBC_RIGHT_BREAKCODE || bytes[1] == KBC_LEFT_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_UP_MAKECODE)
                *dir = right_up;
            else if (bytes[1] == KBC_DOWN_MAKECODE)
                *dir = right_down;
            break;
        case right_down:
            if(bytes[1] == KBC_RIGHT_BREAKCODE)
                *dir = down;
            else if(bytes[1] == KBC_DOWN_BREAKCODE)
                *dir = right;
            break;
        case right_up:
            if(bytes[1] == KBC_RIGHT_BREAKCODE)
                *dir = up;
            else if(bytes[1] == KBC_UP_BREAKCODE)
                *dir = right;
            break;
        case left:
            if(bytes[1] == KBC_LEFT_BREAKCODE || bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_UP_MAKECODE)
                *dir = left_up;
            else if (bytes[1] == KBC_DOWN_MAKECODE)
                *dir = left_down;
            break;
        case left_down:
            if(bytes[1] == KBC_LEFT_BREAKCODE)
                *dir = down;
            else if(bytes[1] == KBC_DOWN_BREAKCODE)
                *dir = left;
            break;
        case left_up:
            if(bytes[1] == KBC_LEFT_BREAKCODE)
                *dir = up;
            else if(bytes[1] == KBC_UP_BREAKCODE)
                *dir = left;
            break;
        case up:
            if(bytes[1] == KBC_UP_BREAKCODE || bytes[1] == KBC_DOWN_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = right_up;
            else if (bytes[1] == KBC_LEFT_MAKECODE)
                *dir = left_up;
            break;
        case down:
            if(bytes[1] == KBC_DOWN_BREAKCODE || bytes[1] == KBC_UP_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = right_down;
            else if (bytes[1] == KBC_LEFT_MAKECODE)
                *dir = left_down;
            break;
        default:
            if(bytes[1] == KBC_UP_MAKECODE)
                *dir = up;
            else if(bytes[1] == KBC_DOWN_MAKECODE)
                *dir = down;
            else if(bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = right;
            else if(bytes[1] == KBC_LEFT_MAKECODE)
                *dir = left;
            else
                *dir = none;
            break;

    }
}

