#include "video.h"

int video_change_mode(uint16_t mode){
  struct reg86 r;

  memset(&r, 0, sizeof(r));	/* zero the structure */

  r.ax = 0x4F02;         // VBE call, function 02 -- set VBE mode
  r.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
  r.intno = 0x10;

  if (sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

void* vram_map_memory(phys_bytes base, unsigned int size){

  int r;
  struct minix_mem_range mr;        /* physical memory range */
  unsigned int vram_base = base;    /* VRAM’s physical addresss */
  unsigned int vram_size = size;    /* VRAM’s size, but you can use the frame-buffer size, instead */
  void *video_mem;                  /* frame-buffer VM address */

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  return video_mem;
}


void draw_rectangle(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint32_t color, char* video_mem,vbe_mode_info_t vmi_p){
  /*uint64_t ptr = (uint64_t)(video_mem);
  uint32_t step = vmi_p.BitsPerPixel/8;
  uint32_t hres = vmi_p.XResolution;
  uint32_t vres =  vmi_p.YResolution;

  int firstPos = ((x + (y*hres))*vmi_p.BitsPerPixel)/8;
  ptr = ptr + firstPos;


  for(int line = 0; line < height; line++) {
    for(int col = 0; col < width; col++){
      memset((void*)ptr, color, step);

      if(ptr + step < (uint64_t)(video_mem + hres * vres * step)){
        ptr += step;
      }
      else
      {
        break;
      }

    }

    if(ptr < (uint64_t)(video_mem + hres * vres * step)){
      ptr = ptr + (hres - width) * step;
    }
    else
    {
      break;
    }
  }*/

    uint64_t video_it;
    int x_max ;
    int y_max ;

    if(x+width > vmi_p.XResolution)
        x_max =  vmi_p.XResolution;
    else
        x_max =x+width;
    if(y+height > vmi_p.YResolution)
        y_max = vmi_p.YResolution;
    else
        y_max = y+height;



    for(int i = x; i < x_max; i++){
        for(int j = y; j < y_max; j++){

            video_it = (uint64_t)video_mem + (i+ j*vmi_p.XResolution)*(vmi_p.BitsPerPixel/8);
            memset((void*)video_it, color, (vmi_p.BitsPerPixel/8));

        }
    }
}

/*void draw_xpm(uint16_t x, uint16_t y, uint64_t video_mem, xpm_image_t* img){
  uint64_t video_it = video_mem + ((x + (y*1024)));

  uint8_t *img_it;
  img_it = img->bytes;

  for(uint32_t lines = 0; lines < img->height; lines++){
    for(uint32_t cols = 0; cols < img->width; cols++){
      memset((void*)(video_it),(int)(img_it[cols + lines * img->width]), 1);
      video_it += 1;
    }
    video_it = video_it + (1024 - img->width);
  }
}*/

void draw_xpm(uint16_t x, uint16_t y, uint64_t video_mem, xpm_image_t* img,vbe_mode_info_t vmi_p){

    int nrBytes = vmi_p.BytesPerScanLine/vmi_p.XResolution;     // Number of bytes of color
    uint8_t *img_it;
    img_it = img->bytes;

    for(uint32_t lines = 0; lines < img->height; lines++){
        for(uint32_t cols = 0; cols < img->width; cols++){

            uint32_t  color = 0;
            for(int i = 0; i < nrBytes; i++){
                color |= (img_it[(cols * nrBytes + lines * nrBytes * img->width) + i] << 8*i);
            }

            draw_pixel(cols+x, lines+y, video_mem, vmi_p, color);
        }
    }
}

void draw_pixel(uint32_t cols, uint32_t lines, uint64_t video_mem, vbe_mode_info_t vmi_p, uint32_t color){
    int nrBytes = vmi_p.BytesPerScanLine/vmi_p.XResolution;
    uint8_t* video_it = (uint8_t*) video_mem + (cols + lines * vmi_p.XResolution)*nrBytes;

    // Handle every byte of color
    for(int i = 0; i < nrBytes; i++){
        video_it[i] = (uint8_t) (color >> 8*i);
    }
}
