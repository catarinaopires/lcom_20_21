#pragma once

int (subscribe_int)(int irq, int policy, uint8_t *bit_no);

int unsubscribe_int();
