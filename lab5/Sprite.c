#include "Sprite.h"
#include "video.h"

/**
 * Creates a new sprite with pixmap "pic", with specified
 * position (within the screen limits) and speed;
 * Does not draw the sprite on the screen
 *
 * Returns NULL on invalid pixmap.
 */
Sprite* create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed, xpm_image_t* img) {
  // Allocate space for the "object"
  Sprite* sp = (Sprite*) malloc( sizeof(Sprite));
  if( sp == NULL )
    return NULL;

  // Read the sprite pixmap
  xpm_load(pic, XPM_INDEXED, img);

  sp->map = img;

  if( sp->map == NULL ) {
    free(sp);
    return NULL;
  }

  sp->width = img->width;
  sp->height = img->height;
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  return sp;
}

void destroy_sprite(Sprite* sp){
  if( sp == NULL )
    return;
  if( sp ->map )
    free(sp->map);
  free(sp);
  sp = NULL;     // XXX: pointer is passed by value should do this @ the caller
}

int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, void* video_mem, vbe_mode_info_t vmi_p){

    // Clear screen (transparency color)
    draw_rectangle(sp->x, sp->y, sp->height, sp->width, xpm_transparency_color(XPM_INDEXED), video_mem, vmi_p);

    // Update positions
    if(sp->x < xf){
        sp->x += sp->xspeed;
    }
    if(sp->y < yf){
        sp->y += sp->yspeed;
    }

    // Draw image in new position
    draw_xpm(sp->x, sp->y, (uint64_t)video_mem, sp->map, vmi_p);

    // If in the end, draw again and return (Movement finished)
    if(sp->x >= xf && sp->y >= yf){
        return 1;
    }
    else
        return 0;
}
