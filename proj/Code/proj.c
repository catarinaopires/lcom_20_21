// IMPORTANT: you must include the following line in all your C files
#include "video/video.h"
#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "video/extImages.h"
#include "video/images/ball.xpm"
#include "video/images/bomb.xpm"
#include "video/images/clock.xpm"

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

  video_instance instance = video_init_empty();
  instance.mode = 0x14C;
  instance.video_get_mode_info = video_get_mode_info;
  instance.video_change_mode = video_change_mode;
  instance.video_map_vram_mem = video_map_vram_mem;
  instance.video_flip_page = NULL;
  instance.video_get_current_buffer = video_get_current_buffer; 
  video_get_mode_info(&instance);
  instance.bytesPerPixel = instance.mode_info.BitsPerPixel / 8;
  video_map_vram_mem(&instance, 1);
  video_change_mode(&instance, MODE_1152x864);

   //draw_rectangle(100,100,100,100, (ENG_RED|ENG_GREEN|ENG_BLUE), &instance);
  
  // xpm_image_t ball;
  // xpm_load(ball_xpm, XPM_8_8_8_8, &ball);
  // draw_xpm(150, 20, &ball, &instance);

/* Image i = image_construct(ball_xpm, XPM_8_8_8_8,0,0);
  printf("draw: %d\n", image_draw(&i, &instance));
  sleep(2);*/

  Sprite* sprite = create_sprite(bomb_xpm, 0, 0, 5, 0);
  image_draw(&sprite->drawing, &instance);
  sleep(2);

  Sprite* sprite1 = create_sprite(ball_xpm, 1000, 0, -5, 0);
  image_draw(&sprite1->drawing, &instance);

  Sprite* arr[] = {sprite, sprite1};
  sleep(5);
  int status =0;
  int st = 0;
  while (st != 1 && status != 1)
  {  
    sleep(1);

    if(check_collisions_sprite(arr)){
      printf("collision2\n");
      break;
    }
    else{
      st = move_sprite(sprite1,12,0, &instance);
    }

    if(check_collisions_sprite(arr)){
      printf("collision1\n");
      break;
    }
    else{
      status = move_sprite(sprite,500,0, &instance);
    }
    
    if(status == 1){
      printf("movement finished\n");
      if(st == 1)
        break;
    }

    if(st == 1)
      printf("movement finished2\n");
      if(status ==1)
        break;

  }
  
  destroy_sprite(sprite1);
  destroy_sprite(sprite);
  
  sleep(2);
  
  instance.video_change_mode(&instance, MODE_TEXT);
  
  return 0;
}
