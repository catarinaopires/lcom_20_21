#pragma once

#include <stdarg.h>

typedef struct {
  int x, y; // current position
  int width, height;  // dimensions
  int xspeed, yspeed; // current speed
  char*map;          // the pixmap
} Sprite;

Sprite* create_sprite(const uint8_t* pic[], int x, int y,int xspeed, int yspeed);
void destroy_sprite(Sprite*sp);


typedef struct {Sprite*sp;     // standard sprite
   int aspeed;     // no. frames per pixmap
   int cur_aspeed; // no. frames left to next change
   int num_fig;    // number of pixmaps
   int cur_fig;    // current pixmap
   char**map;     // array of pointers to pixmaps
}AnimSprite;

AnimSprite(uint8_t no_pic, char*pic1[], ...);
int animate_animSprite(AnimSprite*sp,);
void destroy_animSprite(AnimSprite*sp);
