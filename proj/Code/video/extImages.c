#include "extImages.h"

void draw_xpm(uint16_t x, uint16_t y, xpm_image_t* img, video_instance* instance){

    int nrBytes = instance->mode_info.BytesPerScanLine/instance->mode_info.XResolution;     // Number of bytes of color
    uint8_t *img_it;
    img_it = img->bytes;

    for(uint32_t lines = 0; lines < img->height; lines++){
        for(uint32_t cols = 0; cols < img->width; cols++){

            uint32_t  color = 0;
            for(int i = 0; i < nrBytes; i++){
                color |= (img_it[(cols * nrBytes + lines * nrBytes * img->width) + i] << 8*i);
            }

            draw_pixel(cols+x, lines+y, color, instance);
        }
    }
}

Sprite* create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed, xpm_image_t* img) {
    // Allocate space for the "object"
    Sprite* sp = (Sprite*) malloc( sizeof(Sprite));
    if( sp == NULL )
        return NULL;

    // Read the sprite pixmap
    xpm_load(pic, XPM_8_8_8_8, img);

    sp->drawing.map = img;

    if( sp->drawing.map == NULL ) {
        free(sp);
        return NULL;
    }

    sp->drawing.width = img->width;
    sp->drawing.height = img->height;
    sp->drawing.x = x;
    sp->drawing.y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;

    return sp;
}

void destroy_sprite(Sprite* sp){
    if( sp == NULL )
        return;
    if( sp->drawing.map )
        free(sp->drawing.map);
    free(sp);
    sp = NULL;     // XXX: pointer is passed by value should do this @ the caller
}

int check_collisions_sprite(Sprite** arr){
    size_t sz = sizeof(arr)/sizeof(arr[0]);

    for(size_t i = 0; i < sz; i++){

        if((arr[i]->drawing.x <= arr[i + 1]->drawing.x && arr[i]->drawing.x+arr[i]->drawing.width >= arr[i + 1]->drawing.x) || 
        (arr[i]->drawing.x <= arr[i + 1]->drawing.x+arr[i + 1]->drawing.width && 
        arr[i]->drawing.x+arr[i]->drawing.width >=  arr[i + 1]->drawing.x+arr[i + 1]->drawing.width)){
      
            return 1;
        }
    } 
    return 0;
}

int draw_sprite(uint16_t x, uint16_t y, xpm_image_t* img, video_instance* instance){

    int nrBytes = instance->mode_info.BytesPerScanLine/instance->mode_info.XResolution;     // Number of bytes of color
    uint8_t *img_it;
    img_it = img->bytes;

    for(uint32_t lines = 0; lines < img->height; lines++){
        for(uint32_t cols = 0; cols < img->width; cols++){

            uint32_t  color = 0;
            for(int i = 0; i < nrBytes; i++){
                color |= (img_it[(cols * nrBytes + lines * nrBytes * img->width) + i] << 8*i);
            }

            if(draw_pixel(cols+x, lines+y, color, instance))
                return 1;
        }
    }
    return 0;
}

int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, video_instance* instance){

    // Clear screen (transparency color)
    draw_rectangle(sp->drawing.x, sp->drawing.y, sp->drawing.height, sp->drawing.width, xpm_transparency_color(XPM_8_8_8_8), instance);

    // Update positions
    if(((sp->drawing.x < xf) && (sp->xspeed > 0)) || ((sp->drawing.x > xf) && (sp->xspeed < 0))){
        sp->drawing.x += sp->xspeed;
    }
    if(((sp->drawing.y < yf) && (sp->yspeed > 0)) || ((sp->drawing.y > yf) && (sp->yspeed < 0))){
        sp->drawing.y += sp->yspeed;
    }

    // Draw image in new position
    if(draw_sprite(sp->drawing.x, sp->drawing.y, sp->drawing.map, instance)){
        return -1;
    }

    // If in the end, draw again and return (Movement finished)
    if(((sp->drawing.x >= xf && sp->xspeed >= 0) || (sp->drawing.x <= xf && sp->xspeed <= 0)) &&
       ((sp->drawing.y >= yf && sp->yspeed >= 0) || (sp->drawing.y <= yf && sp->yspeed <= 0))){
        return 1;
    }
    else
        return 0;
}

