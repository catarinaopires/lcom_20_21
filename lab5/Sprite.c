#include "Sprite.h"
#include "video.h"

/**
 * Creates a new sprite with pixmap "pic", with specified*position (within the screen limits) and speed;
 * Does not draw the sprite on the screen
 *
 * Returns NULL on invalid pixmap.
 */
Sprite* create_sprite(const uint8_t* pic[], int x, int y,int xspeed, int yspeed) {
//allocate space for the "object"
  Sprite* sp = (Sprite*) malloc( sizeof(Sprite));
  xpm_image_t img;
  if( sp == NULL )
    return NULL;
  // read the sprite pixmap
  sp->map = xpm_load(pic, XPM_INDEXED, &img);
  if( sp->map == NULL ) {
    free(sp);
    return NULL;
  }
  sp->width = img.width;
  sp->height=img.height;
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  return sp;
}

void destroy_sprite(Sprite*sp){
  if( sp == NULL )
    return;
  if( sp ->map )
    free(sp->map);
  free(sp);
  sp = NULL;     // XXX: pointer is passed by value should do this @ the caller
}

int move_sprite(Sprite* sp, xf, yf){

    // TODO: CLEAR SCREEN (transparency color)

    // TODO: DRAW IMAGE IN NEW POSITION

    // TODO: Update positions


    return 0;

}




AnimSprite* create_animSprite(uint8_t no_pic,char*pic1[]) {
  AnimSprite *asp = malloc(sizeof(AnimSprite)); // create a standard sprite with first pixmap
  asp->sp = create_sprite(pic1, 0, 0, 0, 0);    // allocate array of pointers to pixmaps
  asp->map = malloc((no_pic) * sizeof(char *)); // initialize the first pixmap
  asp->map[0] = asp->sp->map;
  asp->num_fig = no_pic;
  va_list ap;
  va_start(ap, pic1);
  for (i = 1; i < no_pic; i++) {
    char **tmp = va_arg(ap, char **);
    xpm_image_t img;
    asp->map[i] = xpm_load(tmp, XPM_INDEXED, &img);
    if (asp->map[i] == NULL || img.width != asp->sp->width || img.height != asp->sp->height) { // failure: realease allocated memory
      for (j = 1; j < i; j++) {
        free(asp->map[i]);
      }
      free(asp->map);
      destroy_sprite(asp->sp);
      free(asp);
      va_end(ap);
      return NULL;
    }
    va_end(ap);
  }
  return asp;
}

void destroy_animSprite(AnimSprite*sp){
  if( sp == NULL )
    return;
  if( sp ->map )
    for(uint32_t i = 0; i < sp->num_fig, i++){
      free(sp->map[i]);
    }
  free(sp);
  sp = NULL;     // XXX: pointer is passed by value should do this @ the caller
}