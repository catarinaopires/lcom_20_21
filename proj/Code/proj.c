// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
//#include "lab5.h"
#include "video/video.h"
#include "video/images/ball.xpm"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/*static int print_usage() {
  printf("Usage: <mode - hex>\n");

  return 1;
}*/

int(proj_main_loop)(int argc, char *argv[]) {
  /* 
   * Substitute the code below by your own
   */

  //
  // if you're interested, try to extend the command line options so that the usage becomes:
  // <mode - hex> <minix3 logo  - true|false> <grayscale - true|false> <delay (secs)>
  //
  /*bool const minix3_logo = true;
  bool const grayscale = false;
  uint8_t const delay = 5;
  uint16_t mode;

  if (argc != 1)
    return print_usage();

  // parse mode info (it has some limitations for the sake of simplicity)
  if (sscanf(argv[0], "%hx", &mode) != 1) {
    printf("%s: invalid mode (%s)\n", __func__, argv[0]);

    return print_usage();
  }*/

  //return proj_demo(mode, minix3_logo, grayscale, delay);

    vbe_mode_info_t vmi_p;

    if (vbe_get_mode_info(0x14C, &vmi_p)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    void* video_mem = vram_map_memory(vmi_p.PhysBasePtr, (vmi_p.XResolution*vmi_p.YResolution*vmi_p.BitsPerPixel)/8);

    if (video_change_mode(0x14C)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    xpm_image_t img;
    xpm_load(ball_xpm, XPM_8_8_8_8, &img);


  draw_xpm(0, 0, (uint64_t)(video_mem), &img, vmi_p);

  vg_exit();
  return 0;
}
