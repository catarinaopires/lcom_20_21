#ifndef LAB3_UTILS_H
#define LAB3_UTILS_H
#include <lcom/lcf.h>

int (util_sys_inb8)(int port, uint8_t *value);
int (util_sys_inb16)(int port, uint16_t *value);
uint8_t (utils_get_LSB)(uint16_t val);

#endif //LAB3_UTILS_H
