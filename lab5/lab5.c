// IMPORTANT: you must include the following line in all your C files
#include "video.h"
#include "i8042.h"
#include "interrupts.h"
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <machine/int86.h>
#include <stdint.h>
#include <stdio.h>

uint8_t OUTPUT_BUFF_DATA;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  struct reg86 r;

  memset(&r, 0, sizeof(r)); /* zero the structure */

  r.ax = 0x4F02;         // VBE call, function 02 -- set VBE mode
  r.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
  r.intno = 0x10;

  if (sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  // Return to text mode
  sleep(delay);
  if (vg_exit() != OK)
    return 1;

  return 0;
}


int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if (video_change_mode(mode)){
      if (vg_exit() != OK)
        return 1;
    return 1;
  }

  vbe_mode_info_t vmi_p;

  if (vbe_get_mode_info(mode, &vmi_p)){
    if (vg_exit() != OK)
        return 1;
    return 1;
  }
    

  struct minix_mem_range mr = vram_map_memory(vmi_p.PhysBasePtr, (vmi_p.XResolution*vmi_p.YResolution*vmi_p.BitsPerPixel)/8);
  char *ptr = (char*)mr.mr_base;        // mem copy

  int x_counter = 0;
  int y_counter = 0;
  int hres = vmi_p.XResolution, vres = vmi_p.YResolution;


  for(int i = 0; i < hres*vres; i++, ptr++) {
    /* Handle a pixel at a time */
    y_counter = i / hres;
    x_counter = i % hres;

    if(y_counter >= y && y_counter <= y + height){
        if(x_counter >= x && x_counter <= x + width){
            *ptr = color;
        }
    }
  }

  int ipc_status;
  int r;
  message msg;
  uint8_t irq_set = KBC_IRQ;    
  uint8_t counter = 0;
  uint8_t bytes[2] = {0, 0};

  if(subscribe_int(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set)){
    if (vg_exit() != OK)
        return 1;
    return 1;
  }

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
            }
            else {
              if (OUTPUT_BUFF_DATA == KBC_SCANCODE_LEN_2)
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

  if(unsubscribe_int()){
    if (vg_exit() != OK)
        return 1;
    return 1;
  }


  // Reset to text mode
  if (vg_exit() != OK)
    return 1;
  return 0;

}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
