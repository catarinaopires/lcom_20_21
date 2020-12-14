#include "counters.h"
#include <lcom/lcf.h>

counters* counters_counters_initialize(){
    counters* timers = malloc(sizeof(counters));
    
    if(timers == NULL){
        printf("Failed allocating memory for the timers array");
        return NULL;
    }

    for(uint8_t i = 0; i < COUNTERS_SIMULTANEOUSLY_ACTIVE; i++){
        timers->active_counters[i] = NULL;
    }

    for(uint8_t i = 0; i < 2 * COUNTERS_SIMULTANEOUSLY_ACTIVE; i++){
        timers->inactive_counters[i] = NULL;
    }
    timers->stored_act = 0;
    timers->stored_inact = 0;
    return timers;
}

counter_type* counters_counter_init(counters* timers){
    
    if(timers->stored_act + 1 >= COUNTERS_SIMULTANEOUSLY_ACTIVE - timers->stored_inact){
        printf("No more space for timers");
        return NULL;
    }
    
    counter_type* timer_counter = malloc(sizeof(counter_type));
    
    if(timer_counter == NULL){
        printf("Failed allocating memory for the counter");
        return NULL;
    }
    
    *timer_counter = 0;

    for(uint8_t counter = 0; counter < COUNTERS_SIMULTANEOUSLY_ACTIVE; counter++){
        if(timers->active_counters[counter] == NULL){
            timers->active_counters[counter] = timer_counter;
        }
    }

    timers->stored_act++;

    return timer_counter;
}

void counters_counter_increase(counter_type* counter){
    *counter = *counter + 1;
}

void counters_counter_stop(counters* timers, counter_type* counter){
    for(uint8_t counterToo = 0; counterToo < COUNTERS_SIMULTANEOUSLY_ACTIVE; counterToo++){
        if(timers->active_counters[counterToo] == counter){
            for(uint8_t counterToo1 = 0; counterToo1 < 2 * COUNTERS_SIMULTANEOUSLY_ACTIVE; counterToo1++){
                if(timers->inactive_counters[counterToo1] == NULL){
                    timers->inactive_counters[counterToo1] = counter;
                    timers->stored_inact++;
                    timers->stored_act--;
                }
            }

            timers->active_counters[counterToo] = NULL;
        }
    }
}

float counters_get_seconds(counter_type* counter, uint32_t frequency){
    return ((float)(*counter) / (float) frequency);
}

void counters_counter_destructor(counters* timers, counter_type* counter){
    for(uint8_t counterToo = 0; counterToo < 2 * COUNTERS_SIMULTANEOUSLY_ACTIVE; counterToo++){
        if(timers->inactive_counters[counterToo] == counter){
            free(counter);
            timers->inactive_counters[counterToo] = NULL;
            timers->stored_inact--;
        }
    }
}

void counters_counters_destructor(counters* counters){
    for(uint8_t i = 0; i < COUNTERS_SIMULTANEOUSLY_ACTIVE; i++){
        if(counters->active_counters[i] != NULL){
            free(counters->active_counters[i]);
            counters->active_counters[i] = NULL;
        }
    }

    for(uint8_t i = 0; i < 2 * COUNTERS_SIMULTANEOUSLY_ACTIVE; i++){
        if(counters->inactive_counters[i] != NULL){
            free(counters->inactive_counters[i]);
            counters->inactive_counters[i] = NULL;
        }
    }

    free(counters);
}
