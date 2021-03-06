// IMPORTANT: you must include the following line in all your C files

#include <stdbool.h>
#include <stdint.h>
#include <lcom/proj.h>
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "video/video.h"
#include "video/extImages.h"
#include "timer/i8254.h"
#include "common/counters.h"
#include "common/interrupts.h"
#include "kbc/i8042.h"
#include "kbc/keyboard.h"
#include "kbc/mouse.h"
#include "kbc/gestureDetection.h"
#include "rtc/rtc.h"



#include "video/images/Background_Menu.xpm"
#include "video/images/MENU.xpm"
#include "video/images/PLAY.xpm"
#include "video/images/QUIT.xpm"
#include "video/images/choose_game.xpm"
#include "video/images/logo.xpm"

#include "video/images/0.xpm"
#include "video/images/1.xpm"
#include "video/images/2.xpm"
#include "video/images/3.xpm"
#include "video/images/4.xpm"
#include "video/images/5.xpm"
#include "video/images/6.xpm"
#include "video/images/7.xpm"
#include "video/images/8.xpm"
#include "video/images/9.xpm"
#include "video/images/pts.xpm"

#include "video/images/back.xpm"
#include "video/images/gameOver.xpm"
#include "video/images/wonGame.xpm"

#include "video/images/background_reaction_game.xpm"
#include "video/images/bomb.xpm"
#include "video/images/bomb1.xpm"
#include "video/images/player_green.xpm"
#include "video/images/reactionGameText.xpm"
#include "video/images/virus.xpm"

#include "video/images/background_drawing_game.xpm"
#include "video/ui_elements/cursor.xpm"

#include "video/images/background_keys_game.xpm"
#include "video/images/keysGameText.xpm"

#include "video/images/background_explain_time.xpm"
#include "video/images/time_game.xpm"
#include "video/images/sec_txt.xpm"

#include "video/images/drawingGameText.xpm"

#include <math.h>

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


/**
 * @brief proj_main_loop
 */ 
int(proj_main_loop)(int argc, char *argv[]) {

  typedef enum modules {
    MENU,
    MENU_CHOOSE,
    DESCRIPTION_REACTION,
    REACTION_GAME,
    DESCRIPTION_KEYS,
    DESCRIPTION_DRAWING,
    DRAWING_GAME,
    KEYS_GAME,
    DESCRIPTION_TIME,
    TIME_GAME,
    WIN_MENU,
    LOSE_MENU
  } modules;

  static modules module = MENU;

  video_instance instance = video_init_empty();
  instance.mode = MODE_1152x864;
  video_get_mode_info(&instance);
  instance.bytesPerPixel = instance.mode_info.BitsPerPixel / 8;
  video_map_vram_mem(&instance, 3);

  // Needed by MENU module
  Image background_menu = image_construct(background_menu_xpm, XPM_8_8_8_8, 0, 0);
  Image nr_0 = image_construct(n0_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_1 = image_construct(n1_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_2 = image_construct(n2_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_3 = image_construct(n3_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_4 = image_construct(n4_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_5 = image_construct(n5_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_6 = image_construct(n6_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_7 = image_construct(n7_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_8 = image_construct(n8_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_9 = image_construct(n9_xpm, XPM_8_8_8_8, 0, 810);
  Image nr_2_pts = image_construct(pts_xpm, XPM_8_8_8_8, 0, 810);
  Image numbers[] = {nr_0, nr_1, nr_2, nr_3, nr_4, nr_5, nr_6, nr_7, nr_8, nr_9, nr_2_pts};
  Image logo = image_construct(logo_xpm, XPM_8_8_8_8, 480, 15);
  Image menu = image_construct(menu_xpm, XPM_8_8_8_8, 430, 290);
  Image play = image_construct(PLAY_xpm, XPM_8_8_8_8, 475, 440);
  Image quit = image_construct(QUIT_xpm, XPM_8_8_8_8, 475, 532);
  Image back = image_construct(back_xpm, XPM_8_8_8_8, 880, 800);
  int quit_option = 0;
  int collision = 0;
  Sprite *cursor = create_sprite(cursor_xpm, 576, 432, 0, 0);

  // Needed by MENU_CHOOSE
  // Cursor
  Image menu_choose = image_construct(choose_game_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by DESCRIPTION_REACTION
  // Cursor
  Image menu_description_reaction = image_construct(reactionGameText_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by REACTION_GAME
  // Numbers
  Image background_reaction = image_construct(background_reaction_game_xpm, XPM_8_8_8_8, 0, 0);
  Sprite *player_reaction_game = create_sprite(player_green_xpm, 0, 670, 0, 0);
  Sprite *bomb1 = create_sprite(bomb1_xpm, 350, 0, 0, 1);
  Sprite *bomb = create_sprite(bomb_xpm, 700, 0, 0, 1);
  Sprite *virus = create_sprite(virus_xpm, 500, 0, 0, 1);
  Sprite *arr_reaction_game[] = {player_reaction_game, bomb1, bomb};
  Sprite *arr_reaction_game_virus[] = {player_reaction_game, virus};
  int infected = 0, counter_infected = 0;
  static direction d = none;
  int counter_sec = 0;
  uint8_t keys[2] = {0, 0};

  // Needed by DESCRIPTION_KEYS
  // Cursor
  Image menu_description_keys = image_construct(keysGameText_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by KEYS_GAME
  uint8_t keys_game[3] = {0, 0, 0};
  Image background_keys = image_construct(background_keys_game_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by DESCRIPTION_DRAWING
  // Cursor
  Image menu_description_drawing = image_construct(drawingGameText_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by DRAWING_GAME
  // Cursor
  Image background_drawing = image_construct(background_drawing_game_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by DESCRIPTION_TIME
  // Cursor
  Image menu_description_time = image_construct(background_explain_time_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by TIME_GAME
  Image background_time = image_construct(time_game_xpm, XPM_8_8_8_8, 0, 0);
  Image sec_txt = image_construct(sec_txt_xpm, XPM_8_8_8_8, 10, 800);
  uint8_t came_from_time_game = 0;
  float s_counted;

  // Needed by WIN_MENU
  //cursor
  Image background_win = image_construct(wonGame_xpm, XPM_8_8_8_8, 0, 0);

  // Needed by LOSE_MENU
  // Cursor
  // Back
  Image background_lose = image_construct(gameOver_xpm, XPM_8_8_8_8, 0, 0);

  // Start timers
  counters *counters1 = counters_counters_initialize();
  counter_type *counters_time_game = counters_counter_init(counters1);

  if (counters_time_game == NULL) {
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
  if (interrupt_subscribe(RTC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_rtc))
    return 1;

  // Enable Alarm interrupts
  if (rtc_set_flag(RTC_REGISTER_B, RTC_AIE, 1))
    return 1;

  // Configure alarm
  int h = 0, m = 1, s = 0;

  rtc_calculate_finish_alarm(&h, &m, &s);
  if (rtc_config_alarm(h, m, s))
    return 1;

  // Configure and subscribe mouse interrupts
  uint8_t irq_set_mouse = MOUSE_IRQ;
  uint8_t mouse_counter = 0;
  mouse_packet_raw mouse;
  mouse_packet_processed pMouse;
  mouse_event mouse_current_event;
  mouse_current_event.type = BUTTON_EV;
  horizontal_state hState = 0;
  pMouse.lb = 0;
  #define NrOfPoints 21474
  uint32_t nrOfPoints = 0;
  uint32_t x_points[NrOfPoints];                      // Array of x positions of trace of mouse
  uint32_t y_points[NrOfPoints];                      // Array of y positions of trace of mouse
  if (mouse_write_cmd(MOUSE_ENABLE_DATA_REP_STR))
    return 1;

  if (interrupt_subscribe(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_mouse))
    return 1;

  // Array with flag to know the which ones are activated
  // 0 - Timer
  // 1 - Keyboard
  // 2 - Mouse
  // 3 - RTC
  uint8_t interrupt_flags[4] = {0, 0, 0, 0};

  video_change_mode(&instance, MODE_1152x864);
  while (!quit_option) {
    /* Get a request message. */

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                        // hardware interrupt notification
          if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { // subscribed interrupt
            kbc_ih();
            mouse.bytes[mouse_counter] = KBC_OUTPUT_BUFF_DATA;

            if (mouse_counter == 2) {
              mouse_counter = 0;
              interrupt_flags[2] = 1;
            }
            else if (mouse_counter == 1 || mouse_counter == 0) {
              mouse_counter++;
            }
          }

          if (msg.m_notify.interrupts & BIT(irq_set_kbc)) {
            kbc_ih();
            keyboard[keyboard_counter] = KBC_OUTPUT_BUFF_DATA;

            if (keyboard_counter == 1) {
              keyboard_counter = 0;
              interrupt_flags[1] = 1;
            }
            else {
              if (KBC_OUTPUT_BUFF_DATA == KEYBOARD_SCANCODE_LEN_2)
                keyboard_counter++;
              else {
                interrupt_flags[1] = 1;
              }
            }
          }

          if (msg.m_notify.interrupts & BIT(irq_set_timer)) { /* subscribed interrupt */
            interrupt_flags[0] = 1;
          }

          if (msg.m_notify.interrupts & BIT(irq_set_rtc)) {
            if (!rtc_ih())
              interrupt_flags[3] = 1;
          }

          break;

        default:
          break; /* no other notifications expected: do nothing */
      }
    }

    // Application specific code
    switch (module) {
      case MENU:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);
          if (pMouse.lb) {
            // If chosen option is play
            if (check_collision_options(cursor->drawing, 460, 430, 180, 60)) {
              module = MENU_CHOOSE;
            }
            // If chosen option is quit
            else if (check_collision_options(cursor->drawing, 460, 530, 180, 60)) {
              quit_option = 1;
            }
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &background_menu);
          display_time_menu(numbers, rtc_get_value(RTC_HOURS_REG), rtc_get_value(RTC_MINUTES_REG), &instance);
          image_draw(&logo, &instance);
          image_draw(&menu, &instance);

          move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
          change_speed(cursor, 0, 0);

          if (check_collision_options(cursor->drawing, 460, 430, 180, 60)) {
            draw_rectangle(460, 430, 180, 60, 0xbdbdfb, &instance);
          }
          else {
            draw_rectangle(460, 430, 180, 60, 0x8373ff, &instance);
          }
          image_move_to_pos(&play, 470, 440);
          image_draw(&play, &instance);

          if (check_collision_options(cursor->drawing, 460, 530, 180, 60)) {
            draw_rectangle(460, 530, 180, 60, 0xbdbdfb, &instance);
          }
          else {
            draw_rectangle(460, 530, 180, 60, 0x8373ff, &instance);
          }
          image_draw(&quit, &instance);
          draw_sprite(cursor, 1, &instance);
          video_flip_page(&instance);
        }
        break;

      case MENU_CHOOSE:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);

          if (pMouse.lb) {
            // If chosen option is play reaction game
            if (check_collision_options(cursor->drawing, 100, 300, 300, 250)) {
              module = DESCRIPTION_REACTION;
            }
            // If chosen option is drawing game
            else if (check_collision_options(cursor->drawing, 750, 300, 300, 220)) {
              module = DESCRIPTION_DRAWING;
            }
            // If chosen keys game
            else if (check_collision_options(cursor->drawing, 450, 525, 210, 230)) {
              module = DESCRIPTION_KEYS;
            }
            // If chosen time game
            else if (check_collision_options(cursor->drawing, 450, 100, 200, 200)) {
              module = DESCRIPTION_TIME;
            }
            else if (check_collision_options(cursor->drawing, 100, 700, 180, 60)) {
              module = MENU;
            }
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &menu_choose);

          if (check_collision_options(cursor->drawing, 100, 700, 180, 60)) {
            draw_rectangle(100, 700, 180, 60, 0xbdbdfb, &instance);
          }
          else {
            draw_rectangle(100, 700, 180, 60, 0x8373ff, &instance);
          }
          image_move_to_pos(&back, 120, 710);
          image_draw(&back, &instance);

          move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
          draw_sprite(cursor, 1, &instance);
          change_speed(cursor, 0, 0);
          video_flip_page(&instance);
        }
        break;

      case DESCRIPTION_REACTION:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);
          if (pMouse.lb) {
            // If chosen option is play reaction game
            if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
              module = REACTION_GAME;
            }
            else if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
              module = MENU_CHOOSE;
            }
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &menu_description_reaction);
          move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
          change_speed(cursor, 0, 0);

          if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_SELECTED, &instance);
          }
          else {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_BOXES, &instance);
          }
          image_move_to_pos(&play, 490, 640);
          image_draw(&play, &instance);

          if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
            draw_rectangle(100, 630, 180, 60, 0xbdbdfb, &instance);
          }
          else {
            draw_rectangle(100, 630, 180, 60, 0x8373ff, &instance);
          }
          image_move_to_pos(&back, 120, 640);
          image_draw(&back, &instance);
          draw_sprite(cursor, 1, &instance);
          video_flip_page(&instance);
        }
        break;

      case REACTION_GAME:
        // If game is starting, reset needed values
        if (!counter_sec) {
          // Reset values of alarm
          h = 0, m = 1, s = 0;
          rtc_calculate_finish_alarm(&h, &m, &s);
          if (rtc_config_alarm(h, m, s))
            return 1;
          image_move_to_pos(&player_reaction_game->drawing, 0, 670);
          image_move_to_pos(&bomb1->drawing, 350, 0);
          image_move_to_pos(&bomb->drawing, 700, 0);
          image_move_to_pos(&virus->drawing, 500, 0);
          // Reset values
          infected = 0;
          counter_infected = 0;
          d = none;
          keys[0] = 0;
          keys[1] = 0;
        }
        if (interrupt_flags[1]) {
          interrupt_flags[1] = 0;
          collision = check_collisions_sprite(arr_reaction_game, 3);
          if (!collision) {
            check_movement_r_l(&keyboard[0], &d, &keys[0]);
          }
          else {
            counter_sec = 0;
            module = LOSE_MENU;
          }
          if (check_collisions_sprite(arr_reaction_game_virus, 2)) {
            infected = 1;
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &background_reaction);
          assemble_directions_r_l(player_reaction_game, &d, &instance, infected);

          display_time_menu(numbers, 0, 60 - counter_sec / 60, &instance);
          counter_sec++;

          if (infected) {
            counter_infected++;
            // Player infected only for 8 seconds
            if (counter_infected >= 8 * 60) {
              infected = 0;
              counter_infected = 0;
            }
          }

          collision = check_collisions_sprite(arr_reaction_game, 3);
          if (!collision) {
            if (move_sprite(bomb1, 0, 725, 0, &instance) != 0) {
              bomb1->drawing.x = rand() % (instance.mode_info.XResolution - bomb1->drawing.img.width);
              bomb1->drawing.y = 0;
            }
            draw_sprite(bomb1, 0, &instance);
          }
          // Adds bomb with delay comparing to the other bomb
          if (counter_sec >= 3 * 60) {
            collision = check_collisions_sprite(arr_reaction_game, 3);
            if (!collision) {
              if (move_sprite(bomb, 0, 725, 0, &instance) != 0) {
                bomb->drawing.x = rand() % (instance.mode_info.XResolution - bomb->drawing.img.width);
                bomb->drawing.y = 0;
              }
              draw_sprite(bomb, 0, &instance);
            }
            else {
              counter_sec = 0;
              module = LOSE_MENU;
            }
          }
          // Adds virus with delay comparing to first bomb
          if (counter_sec >= 6 * 60) {
            collision = check_collisions_sprite(arr_reaction_game, 3);
            if (!collision) {
              if (check_collisions_sprite(arr_reaction_game_virus, 2)) {
                infected = 1;
              }

              if (move_sprite(virus, 0, 725, 0, &instance) != 0) {
                virus->drawing.x = rand() % (instance.mode_info.XResolution - virus->drawing.img.width);
                virus->drawing.y = 0;
              }
              draw_sprite(virus, 0, &instance);
            }
            else {
              counter_sec = 0;
              module = LOSE_MENU;
            }
          }
          video_flip_page(&instance);
        }
        if (interrupt_flags[3]) {
          interrupt_flags[3] = 0;
          counter_sec = 0;
          module = WIN_MENU;
        }
        break;

      case DESCRIPTION_KEYS:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);
          if (pMouse.lb) {
            // If chosen option is play reaction game
            if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
              // Reset values
              keyboard[0] = 0;
              keyboard[1] = 0;
              keys_game[0] = 0;
              keys_game[1] = 0;
              keys_game[2] = 0;
              module = KEYS_GAME;
            }
            else if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
              module = MENU_CHOOSE;
            }
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &menu_description_keys);

          if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_SELECTED, &instance);
          }
          else {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_BOXES, &instance);
          };
          image_move_to_pos(&play, 485, 640);
          image_draw(&play, &instance);
          if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
            draw_rectangle(100, 630, 180, 60, 0xbdbdfb, &instance);
          }
          else {
            draw_rectangle(100, 630, 180, 60, 0x8373ff, &instance);
          }
          image_move_to_pos(&back, 120, 640);
          image_draw(&back, &instance);

          move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
          draw_sprite(cursor, 1, &instance);
          change_speed(cursor, 0, 0);
          video_flip_page(&instance);
        }
        break;

      case KEYS_GAME:
        if (interrupt_flags[1]) {
          interrupt_flags[1] = 0;

          if (assemble_keys(&keyboard[0], &keys_game[0]) == 1) {
            // Reset values
            keyboard[0] = 0;
            keyboard[1] = 0;
            keys_game[0] = 0;
            keys_game[1] = 0;
            keys_game[2] = 0;
            module = WIN_MENU;
          }
          else if (assemble_keys(&keyboard[0], &keys_game[0]) == -1) {
            keyboard[0] = 0;
            keyboard[1] = 0;
            keys_game[0] = 0;
            keys_game[1] = 0;
            keys_game[2] = 0;
            module = LOSE_MENU;
          }
        }
        if (interrupt_flags[0]) {
          fill_buffer(&instance, video_get_next_buffer(&instance), &background_keys);
          video_flip_page(&instance);
        }
        break;

      case DESCRIPTION_DRAWING:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);

          if (pMouse.lb) {
            // If chosen option is play reaction game
            if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
              module = DRAWING_GAME;
            }
            else if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
              module = MENU_CHOOSE;
            }
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &menu_description_drawing);

          if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_SELECTED, &instance);
          }
          else {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_BOXES, &instance);
          };
          image_move_to_pos(&play, 485, 640);
          image_draw(&play, &instance);

          if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
            draw_rectangle(100, 630, 180, 60, 0xbdbdfb, &instance);
          }
          else {
            draw_rectangle(100, 630, 180, 60, 0x8373ff, &instance);
          }
          image_move_to_pos(&back, 120, 640);
          image_draw(&back, &instance);

          move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
          draw_sprite(cursor, 1, &instance);
          change_speed(cursor, 0, 0);
          video_flip_page(&instance);
        }
        break;
      
      case DRAWING_GAME:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_counter = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);
          gestureDetection_detect_event(&pMouse, &mouse_current_event);
          gestureDetection_draw_process_state_H(&hState, &mouse_current_event, 100, 15);
          if(hState == COMP_H){
            // Reset values of counters
            mouse_current_event.counter_x = 0;
            mouse_current_event.counter_y = 0;
            hState = INIT_H;
            module = WIN_MENU;
          }
          if(hState == FAIL_H){
            // Reset values of counters
            mouse_current_event.counter_x = 0;
            mouse_current_event.counter_y = 0;
            hState = INIT_H;
            module = LOSE_MENU;
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &background_drawing);

          if(hState == INIT_H){
            nrOfPoints = 0;
          }
          if(hState == DRAW_H){
            // Save positions to draw trace of mouse
            x_points[nrOfPoints] = cursor->drawing.x;
            y_points[nrOfPoints] = cursor->drawing.y;
            nrOfPoints++;
          }
          video_draw_from_array(x_points, y_points, nrOfPoints, 2, COLOR_MENU_BOXES, &instance);
                    
          move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
          draw_sprite(cursor, 1, &instance);
          change_speed(cursor, 0, 0);
          video_flip_page(&instance);
        }
        break;
      
      case DESCRIPTION_TIME:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);

          if (pMouse.lb) {
            // If chosen option is play reaction game
            if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
              module = TIME_GAME;
            }
            else if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
              module = MENU_CHOOSE;
            }
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          fill_buffer(&instance, video_get_next_buffer(&instance), &menu_description_time);

          if (check_collision_options(cursor->drawing, 470, 630, 180, 60)) {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_SELECTED, &instance);
          }
          else {
            draw_rectangle(470, 630, 180, 60, COLOR_MENU_BOXES, &instance);
          };
          image_move_to_pos(&play, 485, 640);
          image_draw(&play, &instance);

          if (check_collision_options(cursor->drawing, 100, 630, 180, 60)) {
            draw_rectangle(100, 630, 180, 60, 0xbdbdfb, &instance);
          }
          else {
            draw_rectangle(100, 630, 180, 60, 0x8373ff, &instance);
          }
          image_move_to_pos(&back, 120, 640);
          image_draw(&back, &instance);

          move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
          draw_sprite(cursor, 1, &instance);
          change_speed(cursor, 0, 0);
          video_flip_page(&instance);
        }
        break;

      case TIME_GAME:
        if (interrupt_flags[2]) {
          interrupt_flags[2] = 0;
          mouse_process_packet(&mouse, &pMouse);
          change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);
          if (pMouse.mb) {
            counters_counter_stop(counters1, counters_time_game);
            s_counted = (int) counters_get_seconds(counters_time_game, 60);
            counters_counter_reset(counters_time_game);
            counters_counter_resume(counters1, counters_time_game);
            if (s_counted >= 15 - 1 && s_counted <= 15 + 2) {
              module = WIN_MENU;
              s_counted = 0.0;
            }
            else {
              came_from_time_game = 1;
              module = LOSE_MENU;
            }
          }
        }
        if (interrupt_flags[0]) {
          interrupt_flags[0] = 0;
          counters_counter_increase(counters_time_game);
          fill_buffer(&instance, video_get_next_buffer(&instance), &background_time);

          video_flip_page(&instance);
        }
        break;

      case WIN_MENU:
        if (interrupt_flags[2]) {
        interrupt_flags[2] = 0;
        mouse_process_packet(&mouse, &pMouse);
        change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);

        if (pMouse.lb) {
          // If chosen option is go back
          if (check_collision_options(cursor->drawing, 860, 790, 180, 60)) {
            module = MENU;
          }
        }
      }
        if (interrupt_flags[0]) {
        interrupt_flags[0] = 0;
        fill_buffer(&instance, video_get_next_buffer(&instance), &background_win);

        if (check_collision_options(cursor->drawing, 860, 790, 180, 60)) {
          draw_rectangle(860, 790, 180, 60, 0xbdbdfb, &instance);
        }
        else {
          draw_rectangle(860, 790, 180, 60, 0x8373ff, &instance);
        }
        image_move_to_pos(&back, 880, 800);
        image_draw(&back, &instance);

        move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
        draw_sprite(cursor, 1, &instance);
        change_speed(cursor, 0, 0);
        video_flip_page(&instance);
      }
        break;

      case LOSE_MENU:
        if (interrupt_flags[2]) {
        interrupt_flags[2] = 0;
        mouse_process_packet(&mouse, &pMouse);
        change_speed(cursor, pMouse.delta_x, -pMouse.delta_y);

        if (pMouse.lb) {
          // If chosen option is play
          if (check_collision_options(cursor->drawing, 860, 790, 180, 60)) {
            module = MENU;
            came_from_time_game = 0;
            s_counted = 0.0;
          }
        }
      }
        if (interrupt_flags[0]) {
        interrupt_flags[0] = 0;
        fill_buffer(&instance, video_get_next_buffer(&instance), &background_lose);

        if (check_collision_options(cursor->drawing, 860, 790, 180, 60)) {
          draw_rectangle(860, 790, 180, 60, 0xbdbdfb, &instance);
        }
        else {
          draw_rectangle(860, 790, 180, 60, 0x8373ff, &instance);
        }
        image_move_to_pos(&back, 880, 800);
        image_draw(&back, &instance);

        // If lose game of Time, show user's counted seconds
        if(came_from_time_game && s_counted < 100){
          image_draw(&sec_txt, &instance);

          image_move_to_pos(&numbers[(int)(s_counted/10)], 670,805);
          image_draw(&numbers[(int)s_counted/10], &instance);

          image_move_to_pos(&numbers[((int)s_counted%10)], 700,805);
          image_draw(&numbers[(int)s_counted%10], &instance);
        }

        move_sprite(cursor, instance.mode_info.XResolution, instance.mode_info.YResolution, 1, &instance);
        draw_sprite(cursor, 1, &instance);
        change_speed(cursor, 0, 0);
        video_flip_page(&instance);
      }
        break;

      default:
        break;
    }
  }
  // Use of the timers
  counters_counter_destructor(counters1, counters_time_game);
  counters_counters_destructor(counters1);
  counters1 = NULL;
  counters_time_game = NULL;


  if (interrupt_unsubscribe(irq_set_rtc))
    return 1;

  // Disable alarm interrupts
  if (rtc_set_flag(RTC_REGISTER_B, RTC_AIE, 0))
    return 1;

  // Unsubscribe both interruptions
  if (interrupt_unsubscribe_all())
    return 1;

  if (mouse_write_cmd(MOUSE_DISABLE_DATA_REP_STR))
    return 1;

  destroy_sprite(virus);
  destroy_sprite(bomb1);
  destroy_sprite(bomb);
  destroy_sprite(player_reaction_game);
  destroy_sprite(cursor);

  video_default_page(&instance);
  video_change_mode(&instance, MODE_TEXT);

  return 0;
}
