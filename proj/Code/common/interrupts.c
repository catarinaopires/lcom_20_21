#include <lcom/lcf.h>
#include <stdint.h>

// Using int global variable to obey minix SYS_IRQCTL calls
int HOOK_ID[2];
int HOOK_ID_COUNTER = 0;


int (subscribe_int)(int irq, int policy, uint8_t *bit_no){
    HOOK_ID[HOOK_ID_COUNTER] = *bit_no;

    if(sys_irqsetpolicy(irq, policy, &HOOK_ID[HOOK_ID_COUNTER]) != OK){
        printf("Error in %s!", __func__);
        return 1;
    }
    
    HOOK_ID_COUNTER++;
    return 0;
}


int (unsubscribe_int)(){
        
    for(int i = 0; i < HOOK_ID_COUNTER; i++){
        if(sys_irqrmpolicy(&HOOK_ID[i]) != OK){
            printf("Error in %s!", __func__);
            return 1;
        }
    }
    return 0;
}
