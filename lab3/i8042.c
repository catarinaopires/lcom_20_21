#include "i8042.h"
#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <minix/sysutil.h>

void(kbc_ih)() {
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