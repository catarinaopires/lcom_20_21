// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "i8042.h"
#include "i8254.h"
#include "interrupts.h"
#include <stdint.h>
#include <stdio.h>

#ifndef IRQ_SET
#  define IRQ_SET 12
#endif
// Any header files included below this line should have been created by you

extern uint8_t OUTPUT_BUFF_DATA;

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


int (mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  int r;
  message msg;
  uint8_t irq_set = IRQ_SET;
  uint8_t counter = 0;
  uint8_t bytes[3] = {0, 0, 0};

  if(mouse_write(KBC_ENABLE_DATA_REP_STR))
    return 1;

  if(subscribe_int(KBC_MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set))
    return 1;


  while (cnt) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
            mouse_ih();
            bytes[counter] = OUTPUT_BUFF_DATA;
            if (counter == 2) {
              counter = 0;
              cnt--;
              struct packet toPrint = mouse_process_packet(bytes);
              mouse_print_packet(&toPrint);
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
  }

  if(unsubscribe_int())
    return 1;

  if(mouse_write(KBC_DISABLE_DATA_REP_STR))
    return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    printf("%s(%u): under construction\n", __func__, idle_time);
    
  int ipc_status;
  int r;
  message msg;
  uint8_t irq_set_mouse = IRQ_SET;
  uint8_t irq_set_timer = TIMER0_IRQ;
  uint8_t counter = 0;
  int TIMER_COUNTER = 0;
  uint8_t bytes[3] = {0, 0, 0};

  if(mouse_write(KBC_STREAM_MODE))
    return 1;

  if(mouse_write(KBC_ENABLE_DATA_REP_STR))
    return 1;

  // Subscribe keyboard interruptions
  if(subscribe_int(KBC_MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_mouse))
    return 1;

  // Subscribe timer interruptions
  if(subscribe_int(TIMER0_IRQ, IRQ_REENABLE, &irq_set_timer))
    return 1;

  while (TIMER_COUNTER/60 < idle_time) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { /* subscribed interrupt */
            mouse_ih();
            TIMER_COUNTER = 0;
            bytes[counter] = OUTPUT_BUFF_DATA;
            if (counter == 2) {
              counter = 0;
              struct packet toPrint = mouse_process_packet(bytes);
              mouse_print_packet(&toPrint);
            }
            else if (counter == 1 || counter == 0) {
              counter++;
            }
          }

          if (msg.m_notify.interrupts & BIT(irq_set_timer)) { /* subscribed interrupt */
            /* process it */
            TIMER_COUNTER++;  // Interrupt handler   
            //printf("increase counter: %d\n", TIMER_COUNTER);                
          }

          break;
        default:
          break; /* no other notifications expected: do nothing */
      }

    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  // Unsubscribe both interruptions
  if(unsubscribe_int())
    return 1;
  
  if(mouse_write(KBC_DISABLE_DATA_REP_STR))
    return 1;

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
