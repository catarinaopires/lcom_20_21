#pragma once
#include <lcom/lcf.h>
#include "video.h"


/**
 * @brief Struct to save information of images.
*/
typedef struct Image Image;
struct Image{
  int x, y;             // current position
  xpm_image_t img;      // the image data
  uint8_t * (*load)(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img);
  int (*draw)(Image* this, video_instance* instance);

};

/** @brief Draws image starting in position (x,y).
 * @param this Image
 * @param instance Struct video instance
 * @return Returns 0 upon success, 1 with failure
 */
int image_draw(Image* this, video_instance* instance);

/** @brief Constructor of Image.
 * @param map Xpm file
 * @param type Xpm type
 * @return Image
 */
Image image_construct(xpm_map_t map, enum xpm_image_type type, int x, int y);

/**
 * @brief Struct to save information of Sprites.
*/
typedef struct Sprite Sprite;
struct Sprite{
  Image drawing;
  int xspeed, yspeed;   // Current speed
  void (*destroy_sprite)(Sprite* sp);
  int (*check_collisions_sprite)(Sprite** arr, size_t sz);
  int (*draw_sprite)(Sprite* this, int draw_if_outbounds, video_instance* instance);
  int (*move_sprite)(Sprite* sp, uint16_t xf, uint16_t yf, int draw_if_outbounds, video_instance* instance);
  void (*change_speed)(Sprite* this, int xspeed, int yspeed);
};

/** @brief Creates a new sprite with pixmap "pic", with specified position (within the screen limits) and speed;
 * @param pic Image in format xpm
 * @param x Horizontal position of the beginning of the image
 * @param y Vetical position of the beginning of the image
 * @param xspeed Horizontal speed (Number of pixels between frames)
 * @param yspeed Vertical speed (Number of pixels between frames)
 * @return Pointer to object Sprite created, NULL in case of failure (invalid pixmap)
 */
Sprite* create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed);

/** @brief The "fig" sprite is erased from memory used resources released.
 * @param sp Sprite to erase
 */
void destroy_sprite(Sprite* sp);

/** @brief Checks if sprite can be drawn in new position without colliding with other images.
 * @param arr Array os sprites to check if each other collide
 * @param sz Size of array
 * @return returns 0 upon success (no collisions), 1 if there was a collision
 */ 
int check_collisions_sprite(Sprite**arr, size_t sz);

/** @brief Checks collision between point of image and rectangle.
 * @param img Image whose point of beginning will be used to check collision
 * @param x Initial x of rectangle
 * @param y Initial y of rectangle
 * @param width Width of rectangle
 * @param height Height of rectangle
 * @return returns 1 upon collision, 0 otherwise.
 */
uint8_t check_collision_options(Image img, int x, int y, int width, int height);

/** @brief Draws sprite starting in position (x,y).
 * @param this Image
 * @param draw_if_outbounds if 0 doesn't draw if image outbounds, otherwise draws image
 * @param instance Struct video instance
 * @return returns 0 upon success (image drawn), 1 with failure (not possible to draw)
 */
int draw_sprite(Sprite* this, int draw_if_outbounds, video_instance* instance);

/** @brief Moves sprite with its according speed until final position given by (xf,yf).
 * @param sp Sprite 
 * @param xf Last horizontal position of the beginning of the image
 * @param yf Last vetical position of the beginning of the image
 * @param draw_if_outbounds if 0 doesn't draw if image outbounds, otherwise draws image
 * @param instance Struct video instance
 * @return returns 0 uppon success (image moved), 1 if image reached final position 
 * and -1 if it was not possible to draw in next position
 */
int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, int draw_if_outbounds, video_instance* instance);

/** @brief Changes speed of the sprite.
 * @param this Sprite
 * @param xspeed Horizontal speed (Number of pixels between frames)
 * @param yspeed Vertical speed (Number of pixels between frames)
 */
void change_speed(Sprite* this, int xspeed, int yspeed);

/** @brief Function to load an image directly to the buffer
 * @param reference for the buffer
*/
void fill_buffer(video_instance* instance, void* buffer, Image* img);
