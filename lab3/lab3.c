#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include"kbd.h"


#include <stdbool.h>
#include <stdint.h>


uint32_t COUNTERIT = 0;
/* Declare global variables to use with kbd interrupts
 * OUTPUT_BUFFER_DATA - 16bits
 * ST - (Status register) 8bits
*/
uint16_t OUTPUT_BUFFER_DATA = 0;
uint8_t ST = 0;



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

bool (kbd_scancode_make)(){
  return !(OUTPUT_BUFFER_DATA & KBD_BREAK);
}

uint8_t (kbd_scancode_size)(){
  if(OUTPUT_BUFFER_DATA > 255)
    return 2;
  else
    return 1;
}

uint8_t* (kdb_scancode_bytes)(uint8_t bytes[]){
  uint16_t tmp = OUTPUT_BUFFER_DATA;
  for(int i = 0; i < kbd_scancode_size() && tmp; i++){
    bytes[i] = utils_get_LSB(tmp);
    tmp = tmp >> 8;
  }
  return bytes;
}

int(kbd_test_scan)() {
  int ipc_status;
  message msg;

  uint8_t irq_set = 1;
  kbd_subscribe_int(&irq_set);
  uint8_t bytes[2];
  int r;
  while(bytes[0] != KBD_ESC_RELEASE) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
            /* process it */
            OUTPUT_BUFFER_DATA = 0;
            kbc_ih();
            kbc_ih2();

            //Print Scancode
            kbd_print_scancode(kbd_scancode_make(), kbd_scancode_size(), kdb_scancode_bytes(bytes));

          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  kbd_unsubscribe_int();
  kbd_print_no_sysinb(COUNTERIT);
  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
