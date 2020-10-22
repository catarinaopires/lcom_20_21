#ifndef LAB3_KBD_MACROS_H
#define LAB3_KBD_MACROS_H
#include <lcom/lcf.h>

#define KBD_IRQ 1

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60

//Status register bit 0 goes to high when out buf is full
#define KBC_OBF BIT(0)

//If Bit 7 is high breakcode
#define KBD_BREAK BIT(7)
#define KBD_ESC_RELEASE 0x81

#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)

#define WAIT_KBC 20000

#define COUNTER_SYS_INB


#endif
