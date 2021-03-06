#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.h"

int (util_sys_inb)(int port, uint8_t *value) {
    unsigned int value32;

    // Calls minix SYS_DEVIO to read from the specified port
    if(sys_inb(port, &value32) != OK) {
        printf("Error in %s function's sys_inb call", __func__);
        return 1;
    }

    // Dereferences the value pointer, changing the memory content to the value read with the SYS_DEVIO call
    *value = value32;
    return 0;
}

int (util_get_LSB)(uint16_t val, uint8_t *lsb) {

  // Dereferences the arg in pointer and changes its memory content
  *lsb = val;
  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {

  // Dereferences the arg in pointer and changes its memory content
  *msb = val >> 8;
  return 0;
}

int32_t utils_min(int32_t val1, int32_t val2) {
  if(val1 > val2)
    return val2;
  else
    return val1;
}

void bcd_to_decimal(uint8_t *value) {
  *value = *value - 6 * (*value >> 4);
}

void decimal_to_bcd(uint8_t *value) {
  *value = (((*value/10) << 4) | (*value % 10));
}
