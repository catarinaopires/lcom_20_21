#include"utils.h"
#include <stdint.h>
#include "kbd_macros.h"


#ifdef COUNTER_SYS_INB
  extern uint32_t COUNTERIT;
#endif

int (util_sys_inb8)(int port, uint8_t *value) {
  unsigned int value32;

  //Calls minix SYS_DEVIO to read from the specified port
  if(sys_inb(port, &value32) != OK)
    return 1;

  //Derefrences the value pointer, changing the memory content to the value read with the SYS_DEVIO call
  *value = value32;

#ifdef COUNTER_SYS_INB
  COUNTERIT += 1;
#endif

  return 0;

}

int (util_sys_inb16)(int port, uint16_t *value) {
  unsigned int value32;

  //Calls minix SYS_DEVIO to read from the specified port
  if(sys_inb(port, &value32) != OK)
    return 1;

  //Derefrences the value pointer, changing the memory content to the value read with the SYS_DEVIO call
  *value = value32;

#ifdef COUNTER_SYS_INB
  COUNTERIT += 1;
#endif

  return 0;
}

uint8_t (utils_get_LSB)(uint16_t val) {
  return val;
}
