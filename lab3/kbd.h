#ifndef LAB3_KBD_H
#define LAB3_KBD_H
#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include"kbd_macros.h"
#include"utils.h"

int (kbd_subscribe_int)(uint8_t *bit_no);

int(kbd_unsubscribe_int)();


/* Declare global variables to use with kbd interrupts
 * OUTPUT_BUFFER_DATA - 8bits
 * ST - (Status register) 8bits
*/
extern uint16_t OUTPUT_BUFFER_DATA;
extern uint8_t ST;


void (kbc_ih)();

void(kbc_ih2)();
#endif
