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
    xpm_load(pic, XPM_8_8_8_8, img);

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

int check_collisions_sprite(xpm_image_t* img, int nrBytes){
    uint8_t *img_it;
    img_it = img->bytes;

    // Check collisions
    // Check if in new position of sprite there are non-transparent pixels
    for(uint32_t lines = 0; lines < img->height; lines++){
        for(uint32_t cols = 0; cols < img->width; cols++){

            if(img_it[cols * nrBytes + lines * nrBytes * img->width] != xpm_transparency_color(XPM_8_8_8_8)){
                return 1;
            }
        }
    }
    return 0;
}

int draw_sprite(uint16_t x, uint16_t y, uint64_t video_mem, xpm_image_t* img,vbe_mode_info_t vmi_p){

    int nrBytes = vmi_p.BytesPerScanLine/vmi_p.XResolution;     // Number of bytes of color
    uint8_t *img_it;
    img_it = img->bytes;

    // Check collisions
    if(check_collisions_sprite(img, nrBytes)){
        return 1;
    }

    for(uint32_t lines = 0; lines < img->height; lines++){
        for(uint32_t cols = 0; cols < img->width; cols++){

            uint32_t  color = 0;
            for(int i = 0; i < nrBytes; i++){
                color |= (img_it[(cols * nrBytes + lines * nrBytes * img->width) + i] << 8*i);
            }

            draw_pixel(cols+x, lines+y, video_mem, vmi_p, color);
        }
    }
    return 0;
}

int move_sprite(Sprite* sp, uint16_t xf, uint16_t yf, void* video_mem, vbe_mode_info_t vmi_p){

    // Clear screen (transparency color)
    draw_rectangle(sp->x, sp->y, sp->height, sp->width, xpm_transparency_color(XPM_8_8_8_8), video_mem, vmi_p);

    // Update positions
    if(((sp->x < xf) && (sp->xspeed > 0)) || ((sp->x > xf) && (sp->xspeed < 0))){
        sp->x += sp->xspeed;
    }
    if(((sp->y < yf) && (sp->yspeed > 0)) || ((sp->y > yf) && (sp->yspeed < 0))){
        sp->y += sp->yspeed;
    }

    // Draw image in new position
    // If collision
    if(draw_sprite(sp->x, sp->y, (uint64_t)video_mem, sp->map, vmi_p)){
        return -1;
    }

    // If in the end, draw again and return (Movement finished)
    if(((sp->x >= xf && sp->xspeed > 0) || (sp->x <= xf && sp->xspeed < 0)) &&
       ((sp->y >= yf && sp->yspeed > 0) || (sp->y <= yf && sp->yspeed < 0))){
        return 1;
    }
    else
        return 0;
}
