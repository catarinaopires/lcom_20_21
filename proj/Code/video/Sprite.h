#pragma once
#include <stdarg.h>
#include <lcom/lab5.h>

typedef struct {
  int x, y;             // current position
  int width, height;    // dimensions
  int xspeed, yspeed;   // current speed
  xpm_image_t* map;     // the pixmap
} Sprite;

Sprite* create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed, xpm_image_t* img);
void destroy_sprite(Sprite* sp);
int check_collisions_sprite(xpm_image_t* img, int nrBytes);
int draw_sprite(uint16_t x, uint16_t y, uint64_t video_mem, xpm_image_t* img,vbe_mode_info_t vmi_p);
int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, void* video_mem, vbe_mode_info_t vmi_p);
