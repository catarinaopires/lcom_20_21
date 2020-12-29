// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "i8042.h"
#include "i8254.h"
#include "interrupts.h"
#include "timer.h"
#include "test4.h"
#include <stdint.h>
#include <stdio.h>

#ifndef IRQ_SET
#  define IRQ_SET 12
#endif

int TIMER_COUNTER = 0;

// Any header files included below this line should have been created by you

extern uint8_t KBC_OUTPUT_BUFF_DATA;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab4/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}


int(mouse_test_gesture)(uint8_t len, uint8_t tolerance) {
    int ipc_status;
    int r;
    message msg;
    struct packet toPrint;
    uint8_t irq_set_mouse = IRQ_SET;
    uint8_t counter = 0;
    uint8_t bytes[3] = {0, 0, 0};

    // Initialize current gesture
    struct mouse_ev mouseState;
    mouseState.type = BUTTON_EV;

    /*static state_t drawState = INIT1;*/
    static vertical_state drawStateV = INIT_V;
    static horizontal_state drawStateH = INIT_H;

    if (mouse_write(KBC_ENABLE_DATA_REP_STR))
        return 1;

    // Subscribe mouse interruptions
    if (subscribe_int(KBC_MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_mouse))
        return 1;

    while (drawStateV != COMP_V || drawStateH != COMP_H) {
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                                  /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { /* subscribed interrupt */
                        mouse_ih();
                        bytes[counter] = KBC_OUTPUT_BUFF_DATA;
                        if (counter == 2) {
                            counter = 0;
                            toPrint = mouse_process_packet(bytes);
                            //mouse_print_packet(&toPrint);
                            mouse_detect_event_ours(&toPrint, &mouseState);
                            //printf("%d\n", mouseState.type);
                            //printf("draw state: %d, mouse state: %d", drawState, mouseState);
                            draw_process_state_C(&drawStateV, &drawStateH, &mouseState, len, tolerance);
                        }
                        else if (counter == 1 || counter == 0) {
                            counter++;
                        }
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        }
        else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }

        /* Now, do application dependent event handling */
    }

    // Unsubscribe both interruptions
    if (unsubscribe_int())
        return 1;

    if (mouse_write(MOUSE_DISABLE_DATA_REP_STR))
        return 1;

    return 0;
}
