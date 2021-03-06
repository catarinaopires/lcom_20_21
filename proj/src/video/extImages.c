#include "extImages.h"
#include <lcom/lcf.h>

int image_draw(Image* this, video_instance* instance){
    int nrBytes = instance->mode_info.BytesPerScanLine/instance->mode_info.XResolution;     // Number of bytes of color
    uint8_t *img_it;
    img_it = this->img.bytes;

    if(this->img.height + this->y > instance->mode_info.YResolution ||
    this->img.width + this->x > instance->mode_info.XResolution)
        return 1;

    for(uint32_t lines = 0; lines < this->img.height; lines++){
        for(uint32_t cols = 0; cols < this->img.width; cols++){

            uint32_t  color = 0;
            for(int i = 0; i < nrBytes; i++){
                color |= (img_it[(cols * nrBytes + lines * nrBytes * this->img.width) + i] << 8*i);
            }

            if(draw_pixel(cols + this->x, lines + this->y, color, instance))
                return 1;
        }
    }
    return 0;
}

void image_move_to_pos(Image* this_image, int x, int y){
    this_image->x = x;
    this_image->y = y;
}

Image image_construct(xpm_map_t map, enum xpm_image_type type, int x, int y){
    Image image;
    image.load = xpm_load;

    image.load(map, type, &image.img);
    image.x = x;
    image.y = y;
    image.draw = image_draw;

    return image; 
}

Sprite* create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed) {
    // Allocate space for the "object"
    Sprite* sp = (Sprite*) malloc(sizeof(Sprite));
    if( sp == NULL )
        return NULL;

    // Read the sprite pixmap
    sp->drawing = image_construct(pic, XPM_8_8_8_8,x,y);

    sp->drawing.x = x;
    sp->drawing.y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;

    sp->destroy_sprite = destroy_sprite;
    sp->check_collisions_sprite = check_collisions_sprite;
    sp->draw_sprite = draw_sprite;
    sp->move_sprite = move_sprite;
    sp->change_speed = change_speed;

    return sp;
}

void destroy_sprite(Sprite* sp){
    if( sp == NULL )
        return;
    free(sp);
    sp = NULL;     // XXX: pointer is passed by value should do this @ the caller
}

int check_collisions_sprite(Sprite** arr, size_t sz){
    for(size_t i = 0; i < sz - 1; i++){
        for(size_t j = i+1; j < sz; j++) {

            if ((arr[i]->drawing.x <= arr[j]->drawing.x + arr[j]->drawing.img.width) &&
            (arr[i]->drawing.x + arr[j]->drawing.img.width >=  arr[j]->drawing.x) &&
            (arr[i]->drawing.y <= arr[j]->drawing.y + arr[j]->drawing.img.height) &&
            (arr[i]->drawing.y + arr[j]->drawing.img.height >=  arr[j]->drawing.y)){

                return 1;
            }
        }
    } 
    return 0;
}

uint8_t check_collision_options(Image img, int x, int y, int width, int height){

  if (img.x >= x && img.x <= x + width && img.y >= y && img.y <= y + height){
    return 1;
  }
  return 0;
}

int draw_sprite(Sprite* this, int draw_if_outbounds, video_instance* instance){
    uint8_t *img_it;
    img_it = this->drawing.img.bytes;

    if(!draw_if_outbounds && (this->drawing.img.height + this->drawing.y > instance->mode_info.YResolution ||
    this->drawing.img.width + this->drawing.x > instance->mode_info.XResolution))
        return 1;

    for(uint32_t lines = 0; lines < this->drawing.img.height; lines++){
        for(uint32_t cols = 0; cols < this->drawing.img.width; cols++){
            
            if(draw_if_outbounds && (this->drawing.x + cols > instance->mode_info.XResolution - 1 || 
                this->drawing.y + lines > instance->mode_info.YResolution - 1))
                continue;

            uint32_t  color = 0;
            for(int i = 0; i < instance->bytesPerPixel; i++){
                color |= (img_it[(cols * instance->bytesPerPixel + lines * instance->bytesPerPixel * this->drawing.img.width) + i] << 8*i);
            }

            if(draw_pixel(cols + this->drawing.x, lines + this->drawing.y, color, instance))
                return 1;
        }
    }
    return 0;
}

int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, int draw_if_outbounds, video_instance* instance){

    // Update positions if possible
    if(draw_if_outbounds){
        sp->drawing.x += sp->xspeed;
        sp->drawing.y += sp->yspeed;
        if(sp->drawing.x < 0)
            sp->drawing.x = 0;
        if(sp->drawing.x >= instance->mode_info.XResolution)
            sp->drawing.x = instance->mode_info.XResolution - 1;
        if(sp->drawing.y < 0)
            sp->drawing.y = 0;
        if(sp->drawing.y >= instance->mode_info.YResolution)
            sp->drawing.y = instance->mode_info.YResolution - 1;
    }
    else{
        if(((sp->drawing.x < xf) && (sp->xspeed > 0)) || ((sp->drawing.x > xf) && (sp->xspeed < 0))){
            sp->drawing.x += sp->xspeed;
        
            if(sp->drawing.x + sp->drawing.img.width >= instance->mode_info.XResolution)
                sp->drawing.x = instance->mode_info.XResolution - 1 - sp->drawing.img.width;
        }
        if(((sp->drawing.y < yf) && (sp->yspeed > 0)) || ((sp->drawing.y > yf) && (sp->yspeed < 0))){
            sp->drawing.y += sp->yspeed;
        
            if(sp->drawing.y + sp->drawing.img.height >= instance->mode_info.YResolution)
                sp->drawing.y = instance->mode_info.YResolution - 1 - sp->drawing.img.height;
        }
    }

    // Movement finished
    if(!draw_if_outbounds && ((sp->drawing.x >= xf && sp->xspeed >= 0) || (sp->drawing.x <= xf && sp->xspeed <= 0)) &&
       ((sp->drawing.y >= yf && sp->yspeed >= 0) || (sp->drawing.y <= yf && sp->yspeed <= 0))){

        return 1;
    }
    else
        return 0;
}

void change_speed(Sprite* this, int xspeed, int yspeed){
    this->xspeed = xspeed;
    this->yspeed = yspeed;
}

void fill_buffer(video_instance* instance, void* buffer, Image* img){
    memcpy(buffer, img->img.bytes, instance->mode_info.BytesPerScanLine * instance->mode_info.YResolution);
}

void display_time_menu(Image* numbers, int first_value, int second_value, video_instance* instance){
  image_move_to_pos(&numbers[(first_value/10)], 20,810);
  image_draw(&numbers[first_value/10], instance);

  image_move_to_pos(&numbers[first_value%10], 60,810);
  image_draw(&numbers[first_value%10], instance);

  numbers[10].x = 90;
  image_draw(&numbers[10], instance);

  image_move_to_pos(&numbers[(second_value / 10)], 120,810);
  image_draw(&numbers[second_value/10], instance);

  image_move_to_pos(&numbers[second_value%10], 160,810);
  image_draw(&numbers[second_value%10], instance);
}

void assemble_directions_r_l(Sprite *sprite, direction *dir, video_instance *instance, int infected) {
  switch (*dir) {
    case right:
      if (infected) {
        change_speed(sprite, -5, 0);
        move_sprite(sprite, 0, 0, 0, instance);
        draw_sprite(sprite, 0, instance);
        change_speed(sprite, 0, 0);
      }
      else {
        change_speed(sprite, 5, 0);
        move_sprite(sprite, instance->mode_info.XResolution - sprite->drawing.img.width, 0, 0, instance);
        draw_sprite(sprite, 0, instance);
        change_speed(sprite, 0, 0);
      }
      break;
    case left:
      if (infected) {
        change_speed(sprite, 5, 0);
        move_sprite(sprite, instance->mode_info.XResolution - sprite->drawing.img.width, 0, 0, instance);
        draw_sprite(sprite, 0, instance);
        change_speed(sprite, 0, 0);
      }
      else {
        change_speed(sprite, -5, 0);
        move_sprite(sprite, 0, 0, 0, instance);
        draw_sprite(sprite, 0, instance);
        change_speed(sprite, 0, 0);
      }
      break;
    default:
      change_speed(sprite, 0, 0);
      move_sprite(sprite, instance->mode_info.XResolution, instance->mode_info.YResolution - sprite->drawing.img.height, 0, instance);
      draw_sprite(sprite, 0, instance);
      break;
  }
}
