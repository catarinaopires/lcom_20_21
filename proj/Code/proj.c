// IMPORTANT: you must include the following line in all your C files
#include "video/extImages.h"
#include "video/video.h"
#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you

#include "common/counters.h"
#include "common/interrupts.h"
#include "kbc/i8042.h"
#include "kbc/keyboard.h"
#include "kbc/mouse.h"
#include "timer/i8254.h"
#include "rtc/rtc.h"

#include "video/images/logo.xpm"
#include "video/images/Background_Menu.xpm"
#include "video/images/MENU.xpm"
#include "video/images/PLAY.xpm"
#include "video/images/QUIT.xpm"

#include "video/images/ReactionGameText.xpm"
#include "video/images/background_reaction_game.xpm"
#include "video/images/bomb.xpm"
#include "video/images/bomb1.xpm"
#include "video/images/player_green.xpm"

#include "video/images/background_drawing_game.xpm"
#include "video/ui_elements/cursor.xpm"

#include "video/images/drawingGameText.xpm"

uint8_t KBC_OUTPUT_BUFF_DATA;

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

void assemble_directions_r_l(Sprite *sprite, direction *dir, video_instance *instance) {
  switch (*dir) {
    case right:
      change_speed(sprite, 5, 0);
      move_sprite(sprite, instance->mode_info.XResolution - sprite->drawing.img.width, 0, 0, instance);
      break;
    case left:
      change_speed(sprite, -5, 0);
      move_sprite(sprite, 0, 0, 0, instance);
      break;
    default:
      change_speed(sprite, 0, 0);
      move_sprite(sprite, instance->mode_info.XResolution, instance->mode_info.YResolution - sprite->drawing.img.height,0, instance);
      break;
  }
}

void display_menu(video_instance *instance) {
  Image background = image_construct(background_menu_xpm, XPM_8_8_8_8, 0, 0);
  fill_buffer(instance, video_get_next_buffer(instance), &background);

  Image logo = image_construct(logo_xpm, XPM_8_8_8_8, 480, 15);
  image_draw(&logo, instance);

  Image menu = image_construct(menu_xpm, XPM_8_8_8_8, 430, 290);
  image_draw(&menu, instance);

  draw_rectangle(460, 430, 180, 60, 0x8373ff, instance);
  Image play = image_construct(PLAY_xpm, XPM_8_8_8_8, 475, 440);
  image_draw(&play, instance);

  draw_rectangle(460, 530, 180, 60, 0x8373ff, instance);
  Image quit = image_construct(QUIT_xpm, XPM_8_8_8_8, 475, 532);
  image_draw(&quit, instance);

  sleep(1);
  video_flip_page(instance);
  sleep(1);
  video_flip_page(instance);
  sleep(5);
}

void display_reaction_game_text(video_instance* instance){
  Image background = image_construct(reactionGameText_xpm, XPM_8_8_8_8,0,0);
  fill_buffer(instance, video_get_next_buffer(instance), &background);

  draw_rectangle(460,630, 180,60, 0x8373ff, instance);
  Image play = image_construct(PLAY_xpm, XPM_8_8_8_8,475,640);
  image_draw(&play, instance);

  sleep(1);
  video_flip_page(instance);
  sleep(1);
  video_flip_page(instance);
  sleep(5);
}

void display_drawing_game_text(video_instance* instance){
  Image background = image_construct(drawingGameText_xpm, XPM_8_8_8_8,0,0);
  fill_buffer(instance, video_get_next_buffer(instance), &background);

  draw_rectangle(470,630, 180,60, 0x8373ff, instance);
  Image play = image_construct(PLAY_xpm, XPM_8_8_8_8,485,640);
  image_draw(&play, instance);

  sleep(1);
  video_flip_page(instance);
  sleep(1);
  video_flip_page(instance);
  sleep(5);
}

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

  typedef enum modules {
    MENU,
    JOGO_REACAO,
    JOGO_DESENHO
  } modules;

  static modules module = JOGO_DESENHO;

  video_instance instance = video_init_empty();
  instance.mode = MODE_1152x864;
  instance.video_get_mode_info = video_get_mode_info;
  instance.video_change_mode = video_change_mode;
  instance.video_map_vram_mem = video_map_vram_mem;
  instance.video_flip_page = NULL;
  instance.video_get_current_buffer = video_get_current_buffer;
  video_get_mode_info(&instance);
  instance.bytesPerPixel = instance.mode_info.BitsPerPixel / 8;
  video_map_vram_mem(&instance, 3);
  video_change_mode(&instance, MODE_1152x864);

  //display_menu(&instance);
  //display_reaction_game_text(&instance);
  // display_drawing_game_text(&instance);


  Image background_reacao = image_construct(background_reaction_game_xpm, XPM_8_8_8_8, 0, 0);
  Image background_drawing = image_construct(background_drawing_game_xpm, XPM_8_8_8_8, 0, 0);
  static direction d = none;
  int collision = 0;
  int counter_sec = 0;

  Sprite *player_jogo_reacao = create_sprite(player_green_xpm, 0, 670, 0, 0);

  Sprite *bomb1 = create_sprite(bomb1_xpm, 350, 0, 0, 1);
  Sprite *bomb = create_sprite(bomb_xpm, 700, 0, 0, 1);
  Sprite *arr_jogo_reacao[] = {player_jogo_reacao, bomb1, bomb};

  Sprite *cursor = create_sprite(cursor_xpm, 576, 432, 0, 0);

  uint8_t keys[2] = {0, 0};

  // Start timers
  counters *counters1 = counters_counters_initialize();
  counter_type *counter1 = counters_counter_init(counters1);

  if (counter1 == NULL) {
    video_default_page(&instance);
    instance.video_change_mode(&instance, MODE_TEXT);
    return 1;
  }

  // Configure interrupts
  int ipc_status;
  int r;
  message msg;
  interrupt_arr_initializer(INTERRUPTS);

  // Subscribe keyboard interruptions
  uint8_t irq_set_kbc = KBC_IRQ;
  uint8_t keyboard_counter = 0;
  keyboard_packet keyboard;
  if (interrupt_subscribe(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_kbc))
    return 1;

  // Subscribe timer interruptions
  uint8_t irq_set_timer = TIMER0_IRQ;
  if (interrupt_subscribe(TIMER0_IRQ, IRQ_REENABLE, &irq_set_timer))
    return 1;

  // Subscribe RTC interruptions
  uint8_t irq_set_rtc = RTC_IRQ;
  uint8_t done = 0;
  if(interrupt_subscribe(RTC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_rtc))
    return 1;

  // Enable Alarm interrupts
  if(rtc_set_flag(RTC_REGISTER_B, RTC_AIE, 1))
    return 1;

  // Config alarm
  int h = 0, m = 1, s = 0;

  rtc_calculate_finish_alarm(&h,&m,&s);
  if(rtc_config_alarm(h, m, s))
    return 1;
  
  // Configure and subscribe mouse interrupts
  uint8_t irq_set_mouse = MOUSE_IRQ;
  uint8_t mouse_counter = 0;
  mouse_packet_raw mouse;
  mouse_packet_processed pMouse;
  pMouse.lb = 0;

  if (mouse_write_cmd(MOUSE_ENABLE_DATA_REP_STR))
    return 1;

  if (interrupt_subscribe(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_mouse))
    return 1;


  while (!pMouse.lb) { //!collision && keyboard[0] != KEYBOARD_ESC_BREAKCODE && !done
    /* Get a request message. */

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (module) {
        case (JOGO_REACAO):
          switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
              if (msg.m_notify.interrupts & BIT(irq_set_kbc)) {
                kbc_ih();
                keyboard[keyboard_counter] = KBC_OUTPUT_BUFF_DATA;

                if (keyboard_counter == 1) {
                  keyboard_counter = 0;

                  collision = check_collisions_sprite(arr_jogo_reacao, 3);
                  if (!collision) {
                    check_movement_r_l(&keyboard[0], &d, &keys[0]);
                  }
                }
                else {
                  if (KBC_OUTPUT_BUFF_DATA == KEYBOARD_SCANCODE_LEN_2)
                    keyboard_counter++;
                  else {
                    collision = check_collisions_sprite(arr_jogo_reacao, 3);
                    if (!collision) {
                      check_movement_r_l(&keyboard[0], &d, &keys[0]);
                    }
                  }
                }
              }

              if (msg.m_notify.interrupts & BIT(irq_set_timer)) {/* subscribed interrupt */

                fill_buffer(&instance, video_get_next_buffer(&instance), &background_reacao);
                assemble_directions_r_l(player_jogo_reacao, &d, &instance);

                counter_sec++;
                counters_counter_increase(counter1);

                collision = check_collisions_sprite(arr_jogo_reacao, 3);
                if (!collision) {
                  if (move_sprite(bomb1, 0, 725, 0, &instance) != 0) {
                    bomb1->drawing.x = rand() % (instance.mode_info.XResolution - bomb1->drawing.img.width);
                    bomb1->drawing.y = 0;
                  }
                }
                // Adds bomb with delay comparing to the other bomb
                if (counter_sec >= 3 * 60) {
                  collision = check_collisions_sprite(arr_jogo_reacao, 3);
                  if (!collision) {
                    if (move_sprite(bomb, 0, 725, 0, &instance) != 0) {
                      bomb->drawing.x = rand() % (instance.mode_info.XResolution - bomb->drawing.img.width);
                      bomb->drawing.y = 0;
                    }
                  }
                }
                video_flip_page(&instance);
              }

              if (msg.m_notify.interrupts & BIT(irq_set_rtc)) {
                if(!rtc_ih())
                  done = 1;
              }

              break;

            default:
              break; /* no other notifications expected: do nothing */
          }
          break;

        case MENU:
          break;

        case JOGO_DESENHO:
          switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:                                        // hardware interrupt notification 
              if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { // subscribed interrupt 
                kbc_ih();
                mouse.bytes[mouse_counter] = KBC_OUTPUT_BUFF_DATA;
      
                if (mouse_counter == 2) {
                  mouse_counter = 0;
                  printf("%x, %x\n", mouse.bytes[0], mouse.bytes[2]);
                  mouse_process_packet(&mouse, &pMouse);
                  //printf("lb = %d, rb = %d, ", pMouse.lb, pMouse.rb);
                  printf("x = %d, y = %d\n", pMouse.delta_x, pMouse.delta_y);
                  change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);
                  //printf("x_speed = %d, y_speed = %d\n", cursor->xspeed, cursor->yspeed);
                  
                }
                else if (mouse_counter == 1 || mouse_counter == 0) {
                  mouse_counter++;
                }
              }

              if (msg.m_notify.interrupts & BIT(irq_set_timer)) { // subscribed interrupt
                counters_counter_increase(counter1);
                fill_buffer(&instance, video_get_next_buffer(&instance), &background_drawing);
                move_sprite(cursor, instance.mode_info.XResolution,instance.mode_info.YResolution, 1, &instance);
                //printf("cursor_x = %d, cursor_y = %d\n", cursor->drawing.x, cursor->drawing.y);
                change_speed(cursor, 0, 0);
                video_flip_page(&instance);
              }

              break;

            default:
              break; // no other notifications expected: do nothing 
          }
          break;
        
        default:
          break;
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  // Use of the timers
  counters_counter_stop(counters1, counter1);
  float a = counters_get_seconds(counter1, 60);
  printf("%d seconds\n", (int) a);
  counters_counter_destructor(counters1, counter1);
  counters_counters_destructor(counters1);
  counters1 = NULL;


  if (interrupt_unsubscribe(irq_set_rtc))
    return 1;

  // Disable alarm interrupts
  if(rtc_set_flag(RTC_REGISTER_B, RTC_AIE, 0))
    return 1;

  // Unsubscribe both interruptions
  if (interrupt_unsubscribe_all())
    return 1;
  
  if (mouse_write_cmd(MOUSE_DISABLE_DATA_REP_STR))
    return 1;

  destroy_sprite(bomb1);
  destroy_sprite(bomb);
  destroy_sprite(player_jogo_reacao);

  video_default_page(&instance);
  instance.video_change_mode(&instance, MODE_TEXT);


  return 0;
}
