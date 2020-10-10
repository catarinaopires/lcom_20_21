#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  //Derefrences the arg in pointer and changes its memory content
  *lsb = val;
  return 0;

}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  //Derefrences the arg in pointer and changes its memory content
  *msb = val >> 8;
  return 0;

}

int (util_sys_inb)(int port, uint8_t *value) {
  unsigned int value32;

  //Calls minix SYS_DEVIO to read from the specified port
  sys_inb(port, value32);

  //Derefrences the value pointer, changing the memory content to the value read with the SYS_DEVIO call
  *value = *(*value32 + 24);

  return 0;
}
