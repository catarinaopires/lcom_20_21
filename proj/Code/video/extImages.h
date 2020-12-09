#pragma once
#include <lcom/lcf.h>
#include "video.h"

typedef struct Image Image;
struct Image{
  int x, y;             // current position
  xpm_image_t* img;      // the image data
  uint8_t * (*load)(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img);
  int (*draw)(Image* this, video_instance* instance);

};

/** @brief Draws image starting in position (x,y).
 * @param this Image
 * @param instance Struct video instance
 */
int image_draw(Image* this, video_instance* instance);

/** @brief Constructor of Image.
 * @param map Xpm file
 * @param type Xpm type
 */
Image image_construct(xpm_map_t map, enum xpm_image_type type);

typedef struct Sprite Sprite;
struct Sprite{
  Image drawing;
  int xspeed, yspeed;   // Current speed
  void (*destroy_sprite)(Sprite* sp);
  int (*check_collisions_sprite)(Sprite**arr);
  int (*draw_sprite)(Sprite* this, video_instance* instance);
  int (*move_sprite)(Sprite* sp, uint16_t xf, uint16_t yf, video_instance* instance);
};

/** @brief Creates a new sprite with pixmap "pic", with specified position (within the screen limits) and speed;
 * @param pic Image in format xpm
 * @param x Horizontal position of the beginning of the image
 * @param y Vetical position of the beginning of the image
 * @param xspeed Horizontal speed (Number of pixels between frames)
 * @param yspeed Vertical speed (Number of pixels between frames)
 * @return Pointer to object Sprite created, NULL in case of failiure (invalid pixmap)
 */
Sprite* create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed);

/** @brief The "fig" sprite is erased from memory used resources released.
 * @param sp Sprite to erase
 */
void destroy_sprite(Sprite* sp);

/** @brief Checks if sprite can be drawn in new position without colliding with other images.
 * @param arr Array os sprites to check if each other collide
 */ 
int check_collisions_sprite(Sprite**arr);

/** @brief Draws sprite starting in position (x,y).
 * @param this Image
 * @param instance Struct video instance
 * @return returns 0 uppon success (image drawn), 1 with failiure (not possible to draw)
 */
int draw_sprite(Sprite* this, video_instance* instance);

/** @brief Moves sprite with its according speed until final position given by (xf,yf).
 * @param sp Sprite 
 * @param xf Last horizontal position of the beginning of the image
 * @param yf Last vetical position of the beginning of the image
 * @param instance Struct video instance
 * @return returns 0 uppon success (image moved), 1 if image reached final position 
 * and -1 if it was not possible to draw in next position
 */
int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, video_instance* instance);
