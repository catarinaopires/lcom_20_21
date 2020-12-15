#include "extImages.h"

int image_draw(Image* this, video_instance* instance){
    int nrBytes = instance->mode_info.BytesPerScanLine/instance->mode_info.XResolution;     // Number of bytes of color
    uint8_t *img_it;
    img_it = this->img.bytes;

    if(this->img.height + this->y >= instance->mode_info.YResolution ||
    this->img.width + this->x >= instance->mode_info.XResolution)
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

   /* if( sp->drawing.img == NULL ) {
        free(sp);
        return NULL;
    }*/

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

int draw_sprite(Sprite* this, video_instance* instance){
    return image_draw(&this->drawing, instance);;
}

int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, video_instance* instance){

    // Clear screen (transparency color)
    //draw_rectangle(sp->drawing.x, sp->drawing.y, sp->drawing.img.height, sp->drawing.img.width, xpm_transparency_color(XPM_8_8_8_8), instance);

    // Update positions if possible
    if(((sp->drawing.x + sp->xspeed < xf) && (sp->xspeed > 0)) || ((sp->drawing.x + sp->xspeed> xf) && (sp->xspeed < 0))){
        sp->drawing.x += sp->xspeed;
    }
    if(((sp->drawing.y + sp->yspeed < yf) && (sp->yspeed > 0)) || ((sp->drawing.y + sp->yspeed > yf) && (sp->yspeed < 0))){
        sp->drawing.y += sp->yspeed;
    }

    // Draw image in new position
    if(draw_sprite(sp, instance)){
        return -1;
    }

    // Movement finished
    if(((sp->drawing.x >= xf && sp->xspeed >= 0) || (sp->drawing.x <= xf && sp->xspeed <= 0)) &&
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
