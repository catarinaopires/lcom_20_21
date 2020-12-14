#include "timer.h"
#include <lcom/lcf.h>

timers* timer_timers_initialize(){
    timers* timers = malloc(sizeof(timers));
    
    if(timers == NULL){
        printf("Failed allocating memory for the timers array");
        return NULL;
    }

    for(uint8_t i = 0; i < TIMERS_SIMULTANEOUSLY_ACTIVE; i++){
        timers->active_timers[i] = NULL;
    }

    for(uint8_t i = 0; i < 2 * TIMERS_SIMULTANEOUSLY_ACTIVE; i++){
        timers->inactive_timers[i] = NULL;
    }
    timers->stored_act = 0;
    timers->stored_inact = 0;
    return timers;
}

time_delta* timer_counter_init(timers* timers){
    
    if(timers->stored_act + 1 >= TIMERS_SIMULTANEOUSLY_ACTIVE - timers->stored_inact){
        printf("No more space for timers");
        return NULL;
    }
    
    time_delta* timer_counter = malloc(sizeof(time_delta));
    
    if(timer_counter == NULL){
        printf("Failed allocating memory for the counter");
        return NULL;
    }
    
    *timer_counter = 0;

    for(uint8_t timer = 0; timer < TIMERS_SIMULTANEOUSLY_ACTIVE; timer++){
        if(timers->active_timers[timer] == NULL){
            timers->active_timers[timer] = timer_counter;
        }
    }

    timers->stored_act++;

    return timer_counter;
}

void timer_counter_increase(time_delta* counter){
    *counter = *counter + 1;
}

void timer_counter_stop(timers* timers, time_delta* counter){
    for(uint8_t timer = 0; timer < TIMERS_SIMULTANEOUSLY_ACTIVE; timer++){
        if(timers->active_timers[timer] == counter){
            for(uint8_t timer = 0; timer < 2 * TIMERS_SIMULTANEOUSLY_ACTIVE; timer++){
                if(timers->inactive_timers[timer] == NULL){
                    timers->inactive_timers[timer] = counter;
                    timers->stored_inact++;
                    timers->stored_act--;
                }
            }

            timers->active_timers[timer] = NULL;
        }
    }
}

float timer_counter_seconds(time_delta* counter, uint32_t frequency){
    return ((float)(*counter) / (float) frequency);
}

void timer_counter_destructor(timers* timers, time_delta* counter){
    for(uint8_t timer = 0; timer < 2 * TIMERS_SIMULTANEOUSLY_ACTIVE; timer++){
        if(timers->inactive_timers[timer] == counter){
            free(counter);
            timers->inactive_timers[timer] = NULL;
            timers->stored_inact--;
        }
    }
}

void timer_timers_destructor(timers* timers){
    for(uint8_t i = 0; i < TIMERS_SIMULTANEOUSLY_ACTIVE; i++){
        if(timers->active_timers[i] != NULL){
            free(timers->active_timers[i]);
            timers->active_timers[i] = NULL;
        }
    }

    for(uint8_t i = 0; i < 2 * TIMERS_SIMULTANEOUSLY_ACTIVE; i++){
        if(timers->inactive_timers[i] != NULL){
            free(timers->inactive_timers[i]);
            timers->inactive_timers[i] = NULL;
        }
    }

    free(timers);
}
