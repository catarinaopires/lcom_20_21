#include "video.h"

int video_get_mode_info(video_instance *instance){
  struct reg86 reg;
  memset(&reg, 0, sizeof(reg));

  mmap_t phys_lm_to_virt;

  lm_alloc(sizeof(vbe_mode_info_t), &phys_lm_to_virt);
  
  reg.ax = VBE_RETURN_VBE_MODE_INFO;
  reg.es = PB2BASE(phys_lm_to_virt.phys);
  reg.di = PB2OFF(phys_lm_to_virt.phys);
  reg.cx = instance->mode;
  reg.intno = BIOS_VIDEO_FUNCTION;

  if(sys_int86(&reg) != OK){
    lm_free(&phys_lm_to_virt);
    printf("Call to sys_int failled\n");
    return 1;
  }

  switch(reg.ah){
    case 0:
      // everything runs as expected
      if(reg.al == 0x4f){
        instance->mode_info = *(vbe_mode_info_t*)phys_lm_to_virt.virt;
        lm_free(&phys_lm_to_virt);
        instance->bytesPerPixel = instance->mode_info.BytesPerScanLine / instance->mode_info.XResolution;
        return 0;
      }
    
    case 1:
      // Function call failed
      printf("Funtion call failed\n");
      break;

    case 2:
      // Not supported by hardware
      printf("Function not supported by hardware\n");
      break;

    case 3:
      // Function invalid in current video mode
      printf("Function invalid for the current video mode\n");
      break;
  }

  lm_free(&phys_lm_to_virt);
  return 1;
}

int video_change_mode(video_instance *instance, uint16_t mode){
  struct reg86 reg;
  memset(&reg, 0, sizeof(reg));

  reg.ax = VBE_SET_VBE_MODE;         
  reg.bx = 1 << 14 | mode; 
  reg.intno = BIOS_VIDEO_FUNCTION;

  if (sys_int86(&reg) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  switch(reg.ah){
    case 0:
      if(reg.al == 0x4f){
        // everything runs as expected
        instance->mode = mode;
        return 0;
      }
    case 1:
      // Function call failed
      printf("Funtion call failed\n");
      break;

    case 2:
      // Not supported by hardware
      printf("Function not supported by hardware\n");
      break;

    case 3:
      // Function invalid in current video mode
      printf("Function invalid for the current video mode\n");
      break;
  }

  return 1;
}

int video_map_vram_mem(video_instance *instance, uint8_t bufferNr){
  printf("In map memory\n");
  if(bufferNr < 1 || bufferNr > 2){
    printf("Invalid number of buffers\n");
    return 1;
  }
  
  int r;
  struct minix_mem_range mem_range; // physical memory range 
  void *video_mem;                  // frame-buffer VM address 
  unsigned int vram_size = ((instance->mode_info.XResolution * instance->mode_info.YResolution * instance->mode_info.BitsPerPixel) / 8);
  
  // Allow memory mapping
  mem_range.mr_base = (phys_bytes) instance->mode_info.PhysBasePtr;
  mem_range.mr_limit = mem_range.mr_base + vram_size;
  for(uint8_t buffer = 0; buffer < 2; buffer++){
    if(buffer < bufferNr){
     if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))){
       printf("sys_privctl (ADD_MEM) failed: %d\n", r);
       return 1;
      }
    
      // Map memory 
      video_mem = vm_map_phys(SELF, (void *) mem_range.mr_base, vram_size);
      if (video_mem == MAP_FAILED){
        printf("couldn’t map video memory\n");
       return 1;
      }

      instance->mapped_vram_addr[buffer] = video_mem;

      mem_range.mr_base = mem_range.mr_limit;
      mem_range.mr_limit = mem_range.mr_base + vram_size;
    }
    else{
      instance->mapped_vram_addr[buffer] = NULL;
    }
  }

  return 0;
}

int video_flip_page(video_instance *instance){
  
  if(instance->mapped_vram_addr[1] == NULL){
    printf("Double buffer not in use\n");
    return 1;
  }

  struct reg86 reg;
  memset(&reg, 0, sizeof(reg));
  
  reg.ax = VBE_SET_GET_DISPLAY_START;
  reg.bh = 0x00;
  reg.bl = VBE_SET_GET_DISPLAY_START_VR;
  reg.dx = instance->page * instance->mode_info.XResolution;
  reg.intno = BIOS_VIDEO_FUNCTION;

  instance->page = (instance->page + 1) % 2; 

  if(sys_int86(&reg) != OK){
    printf("Call to sys_int failled\n");
    return 1;
  }

  switch(reg.ah){
    case 0:
      if(reg.al == 0x4f){
        // everything runs as expected
         return 0;
      }
    
    case 1:
      // Function call failed
      printf("Funtion call failed\n");
      break;

    case 2:
      // Not supported by hardware
      printf("Function not supported by hardware\n");
      break;

    case 3:
      // Function invalid in current video mode
      printf("Function invalid for the current video mode\n");
      break;
  }

  return 1;
}

void* video_get_current_buffer(video_instance *instance){
  return instance->mapped_vram_addr[instance->page];
}

/*
video_instance video_init_single_buffer(uint16_t mode){
  
  video_instance instance;
  instance.mode = 0;
  instance.page = 0;
  instance.mapped_vram_addr[0] = NULL;
  instance.mapped_vram_addr[1] = NULL;
  
  instance.video_get_mode_info = video_get_mode_info;
  instance.video_change_mode = video_change_mode;
  instance.video_map_vram_mem = video_map_vram_mem;
  instance.video_flip_page = video_flip_page;
  instance.video_get_current_buffer = video_get_current_buffer;
  
  if(instance.video_get_mode_info(&instance) != OK){
    printf("Fail getting mode info");
    instance.mode = -1;
    return instance;
  }
  
  if(instance.video_map_vram_mem(&instance, 1) != OK){
    printf("Fail mapping memory");
    instance.mode = -2;
    return instance;
  }
  if(instance.video_change_mode(&instance, mode) != OK){
    printf("Fail changing mode");
    instance.mode = -3;
    return instance;
  }
}
*/
/*
video_instance video_init_double_buffer(uint16_t mode){
  video_instance instance;
  instance.mode = 0;
  instance.page = 0;
  instance.mapped_vram_addr[0] = NULL;
  instance.mapped_vram_addr[1] = NULL;

  instance.video_get_mode_info = video_get_mode_info;
  instance.video_change_mode = video_change_mode;
  instance.video_map_vram_mem = video_map_vram_mem;
  instance.video_flip_page = video_flip_page;
  instance.video_get_current_buffer = video_get_current_buffer; 
  
  if(instance.video_get_mode_info(&instance) != OK){
    printf("Fail getting mode info");
    instance.mode = -1;
    return instance;
  }
  
  if(instance.video_map_vram_mem(&instance, 2) != OK){
    printf("Fail mapping memory");
    instance.mode = -2;
    return instance;
  }
  if(instance.video_change_mode(&instance, mode) != OK){
    printf("Fail changing mode");
    instance.mode = -3;
    return instance;
  }
}
*/

video_instance video_init_empty(){
  video_instance instance;
  instance.mode = 0;
  instance.page = 0;
  instance.mapped_vram_addr[0] = NULL;
  instance.mapped_vram_addr[1] = NULL;
  return instance;
}

void draw_rectangle(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint32_t color, video_instance *instance){

  uint32_t hres = instance->mode_info.XResolution;
  uint32_t vres = instance->mode_info.YResolution;

  for(uint32_t line = y; line < height + y; line++) {
    for(uint32_t col = x; col < width + x; col++){
      if(col < hres){
        draw_pixel(col, line, color, instance);
      }
      else
      {
        break;
      }
      
    }
    if(line >= vres){
      break;
    }
  }
}

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

void draw_pixel(uint32_t col, uint32_t line, uint32_t color, video_instance* instance){
  
  void* video_it = instance->video_get_current_buffer(instance) + (col + line * instance->mode_info.XResolution) * instance->bytesPerPixel;
    
  memcpy(video_it, (const void*)(&color), instance->bytesPerPixel);  // Set Pixel color
}

