// IMPORTANT: you must include the following line in all your C files
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <machine/int86.h>
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "video.h"
#include "i8042.h"
#include "i8254.h"
#include "interrupts.h"
#include "Sprite.h"
#include "virus.xpm"
#include "ball.xpm"

uint8_t OUTPUT_BUFF_DATA;

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

  memset(&r, 0, sizeof(r));  // zero the structure

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
    vbe_mode_info_t vmi_p;

    if (vbe_get_mode_info(mode, &vmi_p)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    void* video_mem = vram_map_memory(vmi_p.PhysBasePtr, (vmi_p.XResolution*vmi_p.YResolution*vmi_p.BitsPerPixel)/8);

    if (video_change_mode(mode)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    draw_rectangle(x, y, height, width, color, video_mem, vmi_p);


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
        /*Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {  /*received notification*/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                              /*hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(irq_set)) {  /*subscribed interrupt*/
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
                    break;  /*no other notifications expected: do nothing */
            }

        }
        else {  /*received a standard message, not a notification
       no standard messages expected: do nothing */
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
    vbe_mode_info_t vmi_p;

    if (vbe_get_mode_info(mode, &vmi_p)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    void* video_mem = vram_map_memory(vmi_p.PhysBasePtr, (vmi_p.XResolution*vmi_p.YResolution*vmi_p.BitsPerPixel)/8);

    if (video_change_mode(mode)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    int width = vmi_p.XResolution / no_rectangles;
    int height = vmi_p.YResolution / no_rectangles;

    for(int lines = 0; lines < no_rectangles; lines++){
        // Check if resolution can be divided equally for the nr of rectangles
        // Check if after painting rectangle, it is left at least one height

        for(int col = 0; col < no_rectangles; col ++){
            uint32_t color;
            if(mode == 0x105){
                color = (first + (lines * no_rectangles + col) * step) % (1 << vmi_p.BitsPerPixel);
            }
            else {
                uint32_t rcolor = ((first & (BIT(7) | BIT(6) | BIT(5))) >> 5 & vmi_p.RedMaskSize + col * step) % (1 << vmi_p.RedMaskSize);
                uint32_t gcolor = ((first & (BIT(2) | BIT(3) | BIT(4))) >> 2 & vmi_p.GreenMaskSize + lines * step) % (1 << vmi_p.GreenMaskSize);
                uint32_t bcolor = (first & (BIT(0) | BIT(1)) + (lines + col)) % (1 << vmi_p.BlueMaskSize);
                color = rcolor << 5 | gcolor << 2 | bcolor;
            }
            draw_rectangle(col * width, lines * height, height, width, color, video_mem, vmi_p);

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

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    vbe_mode_info_t vmi_p;

    if (vbe_get_mode_info(MODE_1024x768, &vmi_p)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    void* video_mem = vram_map_memory(vmi_p.PhysBasePtr, (vmi_p.XResolution*vmi_p.YResolution*vmi_p.BitsPerPixel)/8);

    if (video_change_mode(MODE_1024x768)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    xpm_image_t img;
    xpm_load(xpm, XPM_INDEXED, &img);

    draw_xpm(x, y, (uint64_t)(video_mem), &img, vmi_p);

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
        /*Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {  /*received notification*/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                              /*hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(irq_set)) {  /*subscribed interrupt*/
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
                    break;  /*no other notifications expected: do nothing */
            }

        }
        else {  /*received a standard message, not a notification
       no standard messages expected: do nothing */
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

int (video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
    vbe_mode_info_t vmi_p;
    int movementFinished = 0;
    int ipc_status;
    int r;
    message msg;
    uint8_t irq_set_kbc = KBC_IRQ;
    uint8_t irq_set_timer = TIMER0_IRQ;
    uint8_t timer_counter = 0;
    uint8_t counter_kbc = 0;
    uint8_t bytes[2] = {0, 0};

    if (vbe_get_mode_info(MODE_1024x768, &vmi_p)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    void* video_mem = vram_map_memory(vmi_p.PhysBasePtr, (vmi_p.XResolution*vmi_p.YResolution*vmi_p.BitsPerPixel)/8);

    if (video_change_mode(MODE_1024x768)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    // Speed: number of pixels between frames
    uint16_t xspeed, yspeed;
    int nrFramesBetweenMov = -1;
    int frameCounter = 0;

    // If speed is negative, then it specifies the number of frames required for a 1 pixel displacement
    // Speed: 1 pixel between frames
    if(speed < 0){
        nrFramesBetweenMov = abs(speed);
        // Vertical movement
        if(xi == xf){
            yspeed = 1;
            xspeed = 0;
        }
            // Horizontal movement
        else{
            xspeed = 1;
            yspeed = 0;
        }
    }
        // If speed is positive, then it specifies the displacement in pixels between consecutive frames
        // Speed: speed pixels between frames
    else{
        // Vertical movement
        if(xi == xf){
            yspeed = abs(speed);
            xspeed = 0;
        }
            // Horizontal movement
        else{
            xspeed = abs(speed);
            yspeed = 0;
        }
    }

    xpm_image_t img;
    Sprite* sprite = create_sprite(xpm, xi, yi, xspeed, yspeed, &img);
    draw_xpm(xi,yi, (uint64_t)video_mem, sprite->map, vmi_p);

    if(subscribe_int(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_kbc)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    if(subscribe_int(TIMER0_IRQ, IRQ_REENABLE, &irq_set_timer)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    // EXIT WHEN ESC PRESSED OR MOVEMENT DONE
    while ((bytes[0] != KBC_ESC_BREAKCODE)) {
        // Get a request message.
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {  // Received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                              // Hardware interrupt notification
                    if (msg.m_notify.interrupts & BIT(irq_set_kbc)) {  // Subscribed interrupt
                        kbc_ih();
                        bytes[counter_kbc] = OUTPUT_BUFF_DATA;
                        if (counter_kbc == 1) {
                            counter_kbc = 0;
                        }
                        else {
                            if (OUTPUT_BUFF_DATA == KBC_SCANCODE_LEN_2)
                                counter_kbc++;
                        }
                    }

                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {
                        timer_counter++;
                        if(!(timer_counter % (60 / fr_rate))){
                            // DRAW
                            if(nrFramesBetweenMov == -1){
                                movementFinished = move_sprite(sprite, xf, yf, video_mem, vmi_p);
                            }
                            else{
                                frameCounter++;
                                if(frameCounter == nrFramesBetweenMov){
                                    movementFinished = move_sprite(sprite, xf, yf, video_mem, vmi_p);
                                    frameCounter = 0;
                                }
                            }
                        }
                    }
                    break;
                default:
                    break;  // no other notifications expected: do nothing
            }

        }
        else { //  received a standard message, not a notification
            // no standard messages expected: do nothing
        }
    }

    if(unsubscribe_int()){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    destroy_sprite(sprite);

    // Reset to text mode
    if (vg_exit() != OK)
        return 1;
    return 0;

}

int (video_move_every_direction)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t xspeed, int16_t yspeed, uint8_t fr_rate) {
    vbe_mode_info_t vmi_p;
    int movementFinished = 0;
    int ipc_status;
    int r;
    message msg;
    uint8_t irq_set_kbc = KBC_IRQ;
    uint8_t irq_set_timer = TIMER0_IRQ;
    uint8_t timer_counter = 0;
    uint8_t counter_kbc = 0;
    uint8_t bytes[2] = {0, 0};

    if (vbe_get_mode_info(MODE_1152x864, &vmi_p)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    void* video_mem = vram_map_memory(vmi_p.PhysBasePtr, (vmi_p.XResolution*vmi_p.YResolution*vmi_p.BitsPerPixel)/8);

    if (video_change_mode(MODE_1152x864)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    xpm_image_t img;
    Sprite* sprite = create_sprite(xpm, xi, yi, xspeed, yspeed, &img);
    draw_xpm(xi,yi, (uint64_t)video_mem, sprite->map, vmi_p);

    if(subscribe_int(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &irq_set_kbc)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    if(subscribe_int(TIMER0_IRQ, IRQ_REENABLE, &irq_set_timer)){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    // EXIT WHEN ESC PRESSED OR MOVEMENT DONE
    while ((bytes[0] != KBC_ESC_BREAKCODE)) {
        // Get a request message.
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {   /*Received notification*/
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                               /*Hardware interrupt notification*/
                    if (msg.m_notify.interrupts & BIT(irq_set_kbc)) {   /*Subscribed interrupt*/
                        kbc_ih();
                        bytes[counter_kbc] = OUTPUT_BUFF_DATA;
                        if (counter_kbc == 1) {
                            counter_kbc = 0;
                        }
                        else {
                            if (OUTPUT_BUFF_DATA == KBC_SCANCODE_LEN_2)
                                counter_kbc++;
                        }
                    }

                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {
                        timer_counter++;
                        if(!(timer_counter % (60 / fr_rate))){
                            // DRAW
                            movementFinished = move_sprite(sprite, xf, yf, video_mem, vmi_p);
                        }
                    }
                    break;
                default:
                    break;   /*no other notifications expected: do nothing*/
            }

        }
        else {   /*received a standard message, not a notification*/
             /*no standard messages expected: do nothing*/
        }
    }

    if(unsubscribe_int()){
        if (vg_exit() != OK)
            return 1;
        return 1;
    }

    destroy_sprite(sprite);

    // Reset to text mode
    if (vg_exit() != OK)
        return 1;
    return 0;
}

int(video_test_controller)() {
    /* To be completed */
    printf("%s(): under construction\n", __func__);

    return 1;
}
