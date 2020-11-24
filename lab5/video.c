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
  uint64_t ptr = (uint64_t)(video_mem);
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
  }
}

