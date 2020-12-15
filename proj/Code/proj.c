// IMPORTANT: you must include the following line in all your C files
#include "video/video.h"
#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "common/interrupts.h"
#include "common/counters.h"
#include "video/extImages.h"
#include "video/images/ball.xpm"
#include "video/images/bomb1.xpm"
#include "video/images/bomb.xpm"
#include "video/images/player_green.xpm"
#include "video/images/background3.xpm"
#include "kbc/i8042.h"
#include "timer/i8254.h"


uint8_t OUTPUT_BUFF_DATA;

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

void assemble_directions_r_l(Sprite* sprite, direction* dir, video_instance* instance){
    switch (*dir) {
        case right:
            change_speed(sprite, 5, 0);
            move_sprite(sprite, instance->mode_info.XResolution - sprite->drawing.img.width, 0, instance);
            break;
        case left:
            change_speed(sprite, -5, 0);
            move_sprite(sprite, 0, 0, instance);
            break;
        default:
            change_speed(sprite, 0, 0);
            move_sprite(sprite, instance->mode_info.XResolution, instance->mode_info.YResolution - sprite->drawing.img.height, instance);
            break;
    }
}

int jogo_reacao(void){
    video_instance instance = video_init_empty();
    instance.mode = 0x14C;
    instance.video_get_mode_info = video_get_mode_info;
    instance.video_change_mode = video_change_mode;
    instance.video_map_vram_mem = video_map_vram_mem;
    instance.video_flip_page = NULL;
    instance.video_get_current_buffer = video_get_current_buffer;
    video_get_mode_info(&instance);
    instance.bytesPerPixel = instance.mode_info.BitsPerPixel / 8;
    video_map_vram_mem(&instance, 3);
    video_change_mode(&instance, MODE_1152x864);

    Image background = image_construct(background3_xpm, XPM_8_8_8_8,0,0);


    Sprite* player = create_sprite(player_green_xpm, 0,670, 0, 0);

    Sprite* bomb1 = create_sprite(bomb1_xpm, 350, 0, 0, 1);
    Sprite* bomb = create_sprite(bomb_xpm, 700, 0, 0, 1);
    Sprite* arr[] = {player, bomb1, bomb};

    // Start timers
    counters* counters1 = counters_counters_initialize();
    counter_type* counter1 = counters_counter_init();
    if(counter1 == NULL){
        return 1;
    }


    int ipc_status;
    int r;
    message msg;
    uint8_t irq_set_kbc = KBC_IRQ;
    uint8_t irq_set_timer = TIMER0_IRQ;
    uint8_t counter = 0;
    uint8_t bytes[2] = {0, 0};

    uint8_t keys[2] = {0, 0};
    static direction d = none;
    int collision = 0;
    int counter_sec = 0;

    // Subscribe keyboard interruptions
    interrupt_arr_initializer(INTERRUPTS);
    if (interrupt_subscribe(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_kbc))
        return 1;

    // Subscribe timer interruptions
    if (interrupt_subscribe(TIMER0_IRQ, IRQ_REENABLE, &irq_set_timer))
        return 1;

    while (!collision && bytes[0] != KBC_ESC_BREAKCODE) {
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                             /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(irq_set_kbc)) {
                        kbc_ih();
                        bytes[counter] = OUTPUT_BUFF_DATA;

                        if (counter == 1) {
                            counter = 0;

                            collision = check_collisions_sprite(arr, 3);
                            if (!collision) {
                                check_movement_r_l(&bytes[0], &d, &keys[0]);
                            }

                        } else {
                            if (OUTPUT_BUFF_DATA == KBC_SCANCODE_LEN_2)
                                counter++;
                            else {
                                collision = check_collisions_sprite(arr, 3);
                                if (!collision) {
                                    check_movement_r_l(&bytes[0], &d, &keys[0]);
                                }
                            }
                        }
                    }

                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) { /* subscribed interrupt */
                        fill_buffer(&instance, video_get_next_buffer(&instance), &background);
                        assemble_directions_r_l(player, &d, &instance);

                        counter_sec++;
                        counters_counter_increase(counter1);

                        collision = check_collisions_sprite(arr, 3);
                        if (!collision) {
                            if (move_sprite(bomb1, 0, 725, &instance) != 0) {
                                bomb1->drawing.x = rand() % (instance.mode_info.XResolution - bomb1->drawing.img.width);
                                bomb1->drawing.y = 0;
                            }
                        }
                        // Adds bomb with delay comparing to the other bomb
                        if (counter_sec >= 3 * 60) {
                            collision = check_collisions_sprite(arr, 3);
                            if (!collision) {
                                if (move_sprite(bomb, 0, 725, &instance) != 0) {
                                    bomb->drawing.x = rand() % (instance.mode_info.XResolution - bomb->drawing.img.width);
                                    bomb->drawing.y = 0;
                                }
                            }
                        }
                        video_flip_page(&instance);
                    }

                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }

        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }

    // Use of the timers
    counters_counter_stop(counters1, counter1);
    float a = counters_get_seconds(counter1, 60);
    printf("%d seconds\n", (int)a);
    counters_counter_destructor(counters1, counter1);
    counters_counters_destructor(counters1);
     counters1 = NULL;

    // Unsubscribe both interruptions
    if (interrupt_unsubscribe_all())
        return 1;

    destroy_sprite(bomb1);
    destroy_sprite(bomb);
    destroy_sprite(player);

    video_default_page(&instance);
    instance.video_change_mode(&instance, MODE_TEXT);

    return 0;
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

    return jogo_reacao();
}
