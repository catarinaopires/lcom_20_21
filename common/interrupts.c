#include <lcf/lcf.h>

#include <stdint.h>

// Using int global variable to obey minix SYS_IRQCTL calls
int HOOK_ID;


int subscribe_int(int irq, int policy, uint8_t *bit_no, ){
    HOOK_ID = *bit_no;

    if(sys_irqsetpolicy(irq, policy, &HOOK_ID) != OK){
        printf("Error in %s!", __func__);
        return 1;
    }
    return 0;
}


int unsubscribe_int(){
    if(sys_irqrmpolicy(&HOOK_ID) != OK){
        printf("Error in %s!", __func__);
        return 1;
    }
    return 0;
}