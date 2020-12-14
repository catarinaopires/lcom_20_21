#pragma once
#include "../program_options.h"
#include <stdint.h>

typedef struct interrupt_info{
    int hook_id;
    int irq_line;
}interrupt_info;

interrupt_info INTERRUPTS[INTERRUPTS_ARR_SIZE];

void interrupt_arr_initializer(interrupt_info* arr);

interrupt_info interrupt_info_collector(const int* irq, const uint8_t* bit_no);

int interrupt_subscribe(int irq, int policy, uint8_t *bit_no);

int interrupt_unsubscribe(int irq);

int interrupt_unsubscribe_all();
