#include "interrupts.h"
#include <lcom/lcf.h>

void interrupt_arr_initializer(interrupt_info* arr){
    interrupt_info info;
    info.irq_line = -1;
    info.hook_id = -1;
    for(uint8_t i = 0; i < INTERRUPTS_SIMULTANEOUSLY_SUBSCRIBED; i++){
        arr[i] = info;
    }
}

interrupt_info interrupt_info_collector(const int* irq, const uint8_t* bit_no){
    interrupt_info info;
    info.irq_line = *irq;
    info.hook_id = *bit_no;
    return info;
}

int interrupt_subscribe(int irq, int policy, uint8_t *bit_no){
    int8_t temp = -1;
    for(uint8_t i = 0; i < INTERRUPTS_SIMULTANEOUSLY_SUBSCRIBED; i++){
        if(INTERRUPTS[i].irq_line == -1){
            temp = i;
            break;
        }
    }
    
    if(temp == -1){
        printf("There's no available space in the INTERRUPTS vector");
        return 1;
    }

    INTERRUPTS[temp] = interrupt_info_collector(&irq, bit_no);

    if(sys_irqsetpolicy(irq, policy, &INTERRUPTS[temp].hook_id) != OK){
        printf("Error in %s!", __func__);
        return 1;
    }
    
    return 0;
}

int interrupt_unsubscribe(int irq){
    for(int i = 0; i < INTERRUPTS_SIMULTANEOUSLY_SUBSCRIBED; i++){
        if(INTERRUPTS[i].irq_line == irq){
            if(sys_irqrmpolicy(&INTERRUPTS[i].hook_id) != OK){
                printf("Error in %s!", __func__);
                return 1;
            }
            INTERRUPTS[i].irq_line = -1;
        }
    }
    return 0;
}

int interrupt_unsubscribe_all(){
    for(int i = 0; i < INTERRUPTS_SIMULTANEOUSLY_SUBSCRIBED; i++){
        if(INTERRUPTS[i].irq_line != -1){
            if(sys_irqrmpolicy(&INTERRUPTS[i].hook_id) != OK){
                printf("Error in %s!", __func__);
                return 1;
            }
            INTERRUPTS[i].irq_line = -1;
        }
    }
    return 0;
}
