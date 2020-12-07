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

/*
static int print_usage() {
  printf("Usage: <mode - hex>\n");

  return 1;
}
*/

int(proj_main_loop)(int argc, char *argv[]) {
  /* 
  Substitute the code below by your own
   

  //
  // if you're interested, try to extend the command line options so that the usage becomes:
  // <mode - hex> <minix3 logo  - true|false> <grayscale - true|false> <delay (secs)>
  //
  bool const minix3_logo = true;
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

  printf("In proj main loop");
  video_instance instance = video_init_empty();
  instance.mode = 0x14C;
  instance.video_get_mode_info = video_get_mode_info;
  instance.video_change_mode = video_change_mode;
  instance.video_map_vram_mem = video_map_vram_mem;
  //instance.video_flip_page = video_flip_page;
  instance.video_get_current_buffer = video_get_current_buffer; 
  printf("out1\n");
  video_get_mode_info(&instance);
  //vbe_get_mode_info(MODE_1152x864, &instance.mode_info);
  instance.bytesPerPixel = instance.mode_info.BitsPerPixel / 8;
  printf("out2 %d \n", instance.bytesPerPixel);
  video_map_vram_mem(&instance, 1);
  printf("out3\n");
  video_change_mode(&instance, MODE_1152x864);
  printf("out4\n");
  video_change_mode(&instance, MODE_1152x864);

  draw_rectangle(20,20,100,100, (ENG_RED|ENG_GREEN|ENG_BLUE), &instance);
  //draw_xpm(150, 20, ball_xpm, &instance);

  sleep(10);
  
  instance.video_change_mode(&instance, MODE_TEXT);
  
  return 0;
}
