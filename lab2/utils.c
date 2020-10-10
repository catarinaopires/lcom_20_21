#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  lsb = val;

  printf("%s \n", __func__);

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
 
  msb = val >> 8;

  printf("%s \n", __func__);

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  unsigned int value32;

  sys_inb(port, value32);
  value = *value32 + 24;

  return 0;
}
