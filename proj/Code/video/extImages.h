#pragma once
#include <lcom/lcf.h>
#include "video.h"

/** @brief Draws xpm starting in position (x,y).
 * @param x Horizontal position of the beginning of the image
 * @param y Vetical position of the beginning of the image
 * @param img Image
 * @param instance Struct video instance
 */
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

/** @brief Creates a new sprite with pixmap "pic", with specified position (within the screen limits) and speed;
 * @param pic Image in format xpm
 * @param x Horizontal position of the beginning of the image
 * @param y Vetical position of the beginning of the image
 * @param xspeed Horizontal speed (Number of pixels between frames)
 * @param yspeed Vertical speed (Number of pixels between frames)
 * @param img Image
 * @return Pointer to object Sprite created, NULL in case of failiure (invalid pixmap)
 */
Sprite* create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed, xpm_image_t* img);

/** @brief The "fig" sprite is erased from memory used resources released.
 * @param sp Sprite to erase
 */
void destroy_sprite(Sprite* sp);

/** @brief Checks if sprite can be drawn in new position without colliding with other images.
 * @param arr Array os sprites to check if each other collide
 */ 
int check_collisions_sprite(Sprite**arr);

/** @brief Draws sprite starting in position (x,y).
 * @param x Horizontal position of the beginning of the image
 * @param y Vetical position of the beginning of the image
 * @param img Image
 * @param instance Struct video instance
 * @return returns 0 uppon success (image drawn), 1 with failiure (not possible to draw)
 */
int draw_sprite(uint16_t x, uint16_t y, xpm_image_t* img, video_instance* instance);

/** @brief Moves sprite with its according speed until final position given by (xf,yf).
 * @param sp Sprite 
 * @param xf Last horizontal position of the beginning of the image
 * @param yf Last vetical position of the beginning of the image
 * @param instance Struct video instance
 * @return returns 0 uppon success (image moved), 1 if image reached final position 
 * and -1 if it was not possible to draw in next position
 */
int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, video_instance* instance);
