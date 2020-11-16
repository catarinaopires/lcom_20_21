// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "i8042.h"
#include "i8254.h"
#include "interrupts.h"
#include "timer.h"
#include <stdint.h>
#include <stdio.h>

#ifndef IRQ_SET
#  define IRQ_SET 12
#endif

int TIMER_COUNTER = 0;

struct mouse_ev *CURRENT;

struct mouse_ev *INITIAL_POINT;
typedef enum { INIT1,
               DRAW1,
               INIT2,
               DRAW2,
               COMP } state_t;

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

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  int r;
  message msg;
  uint8_t irq_set = IRQ_SET;
  uint8_t counter = 0;
  uint8_t bytes[3] = {0, 0, 0};

  if (mouse_write(KBC_ENABLE_DATA_REP_STR))
    return 1;

  if (subscribe_int(KBC_MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set))
    return 1;

  while (cnt) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                  /* hardware interrupt notification */
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

  if (unsubscribe_int())
    return 1;

  if (mouse_write(KBC_DISABLE_DATA_REP_STR))
    return 1;

  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  printf("%s(%u): under construction\n", __func__, idle_time);

  int ipc_status;
  int r;
  message msg;
  uint8_t irq_set_mouse = IRQ_SET;
  uint8_t irq_set_timer = TIMER0_IRQ;
  uint8_t counter = 0;
  int TIMER_COUNTER = 0;
  uint8_t bytes[3] = {0, 0, 0};

  if (mouse_write(KBC_STREAM_MODE))
    return 1;

  if (mouse_write(KBC_ENABLE_DATA_REP_STR))
    return 1;

  // Subscribe mouse interruptions
  if (subscribe_int(KBC_MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_mouse))
    return 1;

  // Subscribe timer interruptions
  if (subscribe_int(TIMER0_IRQ, IRQ_REENABLE, &irq_set_timer))
    return 1;

  while (TIMER_COUNTER / 60 < idle_time) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                        /* hardware interrupt notification */
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
          if (msg.m_notify.interrupts & BIT(irq_set_timer)) {
            timer_int_handler();
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
  if (unsubscribe_int())
    return 1;

  if (mouse_write(KBC_DISABLE_DATA_REP_STR))
    return 1;

  return 0;
}


struct mouse_ev *(mouse_detect_event_ours)(struct packet *pp) {
  struct mouse_ev *new_event = (struct mouse_ev *) malloc(sizeof(struct mouse_ev));

  if (pp->rb && CURRENT->type != RB_PRESSED) {
    new_event->type = RB_PRESSED;
  }

  if (pp->lb && CURRENT->type != LB_PRESSED) {
    printf("\nLB_PRESSED\n\n");
    new_event->type = LB_PRESSED;
  }

  if (pp->mb && CURRENT->type != BUTTON_EV) {
    new_event->type = BUTTON_EV;
  }

  if (!pp->rb && CURRENT->type != RB_RELEASED)
    new_event->type = RB_RELEASED;

  else if (!pp->mb && CURRENT->type != BUTTON_EV) {
    new_event->type = BUTTON_EV;
  }

  else if (!pp->lb && CURRENT->type != LB_RELEASED)
    new_event->type = LB_RELEASED;

  else {
    // Mouse movement
    new_event->type = MOUSE_MOV;
  }

  new_event->delta_x = pp->delta_x;
  new_event->delta_y = pp->delta_y;

  return new_event;
}


int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  int r;
  message msg;
  struct packet toPrint;
  uint8_t irq_set_mouse = IRQ_SET;
  uint8_t counter = 0;
  uint8_t bytes[3] = {0, 0, 0};

  // Initialize current gesture
  CURRENT = (struct mouse_ev *) malloc(sizeof(struct mouse_ev));
  CURRENT->type = BUTTON_EV;

  static state_t st = INIT1;
  uint16_t temp_min;

  if (mouse_write(KBC_ENABLE_DATA_REP_STR))
    return 1;

  // Subscribe mouse interruptions
  if (subscribe_int(KBC_MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_mouse))
    return 1;

  while (st != COMP) {
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
            bytes[counter] = OUTPUT_BUFF_DATA;
            if (counter == 2) {
              counter = 0;
              toPrint = mouse_process_packet(bytes);
              CURRENT = mouse_detect_event_ours(&toPrint);
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

    /* Now, do application dependent event handling */
    switch (st) {
      case INIT1:
        printf("\nIN INIT1\n\n");
        if (CURRENT->type == LB_PRESSED) {
          INITIAL_POINT = CURRENT;
          st = DRAW1;
        }
        break;

      case DRAW1:
        printf("\nIN DRAW1\n\n");
        if (CURRENT->type == LB_RELEASED) {
          temp_min = CURRENT->delta_x - INITIAL_POINT->delta_x - 2 * tolerance;
          if (temp_min >= x_len - tolerance) {
            if ((((CURRENT->delta_y - INITIAL_POINT->delta_y) - 2 * tolerance) / temp_min) > 1) {
              st = INIT2;
              INITIAL_POINT = CURRENT;
            }
            else {
              st = INIT1;
            }
          }
          else {
            st = INIT1;
          }
        }
        else {
          st = INIT1;
        }

        break;

      case INIT2:
        printf("\nIN INIT2\n\n");
        if (CURRENT->type == RB_PRESSED) {
          if (abs(CURRENT->delta_x - INITIAL_POINT->delta_x) <= tolerance) {
            INITIAL_POINT = CURRENT;
            st = DRAW2;
          }
        }
        break;

      case DRAW2:
        printf("\nIN DRAW2\n\n");
        if (CURRENT->type == RB_RELEASED) {
          temp_min = CURRENT->delta_x - INITIAL_POINT->delta_x - 2 * tolerance;
          if (temp_min >= x_len) {
            if ((((CURRENT->delta_y - INITIAL_POINT->delta_y) + 2 * tolerance) / temp_min) < -1) {
              st = COMP;
            }
            else {
              st = INIT1;
            }
          }
          else {
            st = INIT1;
          }
        }
        else {
          st = INIT1;
        }

        break;

      case COMP:
        printf("\nIN COMP\n\n");
        break;
    }
  }

  // Unsubscribe both interruptions
  if (unsubscribe_int())
    return 1;

  if (mouse_write(KBC_DISABLE_DATA_REP_STR))
    return 1;

  return 0;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  uint8_t stat = 0;
  uint8_t bytes[3] = {0, 0, 0};

  if (mouse_write(KBC_DISABLE_DATA_REP_STR))
    return 1;

  if (mouse_write(KBC_REMOTE_MODE))
    return 1;

  while (cnt) {
    mouse_write(KBC_READ_DATA);
    for (int counter = 0; counter < 3; counter++) {
      mouse_ih();
      bytes[counter] = OUTPUT_BUFF_DATA;
      if (counter == 2) {
        cnt--;
        struct packet toPrint = mouse_process_packet(bytes);
        mouse_print_packet(&toPrint);
      }
    }
    tickdelay(micros_to_ticks(period * 1000));
  }

  if (mouse_write(KBC_STREAM_MODE))
    return 1;

  if (mouse_write(KBC_DISABLE_DATA_REP_STR))
    return 1;

  while (1) {
    if (util_sys_inb(KBC_ST_REG, &stat) != OK) {
      return 1;
    }
    // loop while 8042 input buffer is full
    if ((stat & KBC_INPUT_BUFF_FULL) == 0) {

      if (sys_outb(KBC_CMD_REG, KBC_CMD_WRITE) != OK)
        return 1;

      if (sys_outb(KBC_CMD_ARGS, minix_get_dflt_kbc_cmd_byte()) != 0)
        return 1;
      break;
    }
    else
      tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return 0;
}
