#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "interrupts.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef IRQ_SET
#  define IRQ_SET 1
#endif

extern uint8_t OUTPUT_BUFF_DATA;
extern int SCAN_COUNTER;
extern int POLL_COUNTER;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(kbd_test_scan)() {
  int ipc_status;
  int r;
  message msg;
  uint8_t irq_set = IRQ_SET;
  uint8_t counter = 0;
  uint8_t bytes[2] = {0, 0};

  if(subscribe_int(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set))
    return 1;

  while (bytes[0] != KBC_ESC_BREAKCODE) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
            kbc_ih();
            bytes[counter] = OUTPUT_BUFF_DATA;
            if (counter == 1) {
              counter = 0;
              kbd_print_scancode(is_make_code(), 2, &bytes[0]);
            }
            else {
              if (OUTPUT_BUFF_DATA == KBC_SCANCODE_LEN_2)
                counter++;
              else
                kbd_print_scancode(is_make_code(), 1,  &bytes[0]);
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
  kbd_print_no_sysinb(SCAN_COUNTER);
  return 0;
}

int(kbd_test_poll)() {
   /* To be completed by the students */
  printf("%s is implemented!\n", __func__);

  uint8_t stat;
  uint8_t counter = 0;
  uint8_t bytes[2] = {0, 0};
  uint8_t cmd;

  while( 1 ) {
    util_sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 input buffer is not empty */
    if( (stat & KBC_OUTPUT_BUFF_FULL) == 0 ) {

      sys_outb(KBC_CMD_REG, KBC_CMD_READ);
      util_sys_inb(KBC_OUT_BUF, &cmd);   // Read command byte

      // Change Bit 0 to 0 to disable keyboard interrupt
      cmd = cmd & 0xfe;

      // Write new command
      sys_outb(KBC_CMD_REG, KBC_OUT_BUF);
      sys_outb(KBC_OUT_BUF, cmd);

      break;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }


  while(bytes[0] != KBC_ESC_BREAKCODE){
    if (kbc_read_poll() == 0){
      if (counter == 1) {
        bytes[counter] = OUTPUT_BUFF_DATA;
        counter = 0;
        kbd_print_scancode(is_make_code(), 2, bytes);
      }
      else {
        bytes[counter] = OUTPUT_BUFF_DATA;
        if (OUTPUT_BUFF_DATA == KBC_SCANCODE_LEN_2)
          counter++;
        else
          kbd_print_scancode(is_make_code(), 1, bytes);
      }
     
    }
  }
  kbd_print_no_sysinb(POLL_COUNTER); 


  while( 1 ) {
    util_sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 input buffer is not empty */
    if( (stat & KBC_OUTPUT_BUFF_FULL) == 0 ) {

      // TODO: ADD KBC_CMD_READ = 0x20 TO MACROS
      sys_outb(KBC_CMD_REG, KBC_CMD_READ);
      util_sys_inb(KBC_OUT_BUF, &cmd);   // Read command byte

       // Change Bit 0 to 1 to enable keyboard interrupt
      cmd = cmd | BIT(0);

      // Write new command
      sys_outb(KBC_CMD_REG, KBC_OUT_BUF);
      sys_outb(KBC_OUT_BUF, cmd);

      break;
    }
    tickdelay(micros_to_ticks(WAIT_KBC)); 
  }

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
