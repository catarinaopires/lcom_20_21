#pragma once
#include <lcom/lcf.h>
#include "video.h"

void draw_xpm(uint16_t x, uint16_t y, xpm_image_t* img, video_instance* instance);

typedef struct Image Image;
struct Image{
  int x, y;             // current position
  int width, height;    // dimensions
  xpm_image_t* map;     // the pixmap
  uint8_t * (*image_load)(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img);
  void (*image_draw)(uint16_t x, uint16_t y, xpm_image_t* img, video_instance* instance);

};

typedef struct Sprite Sprite;
struct Sprite{
  Image drawing;
  int xspeed, yspeed;   // current speed
};

/*Sprite create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed, xpm_image_t* img);
void destroy_sprite(Sprite* sp);
int check_collisions_sprite(xpm_image_t* img, int nrBytes);
int draw_sprite(uint16_t x, uint16_t y, uint64_t video_mem, xpm_image_t* img,vbe_mode_info_t vmi_p);
int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, void* video_mem, vbe_mode_info_t vmi_p);
*/
