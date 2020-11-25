#pragma once
#include <stdint.h>
#include <lcom/lcf.h>
#include <machine/int86.h>

#define MODE_1024x768 0x105

int video_change_mode(uint16_t mode);

void* vram_map_memory(phys_bytes base, unsigned int size);

void draw_rectangle(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint32_t color, char* video_mem,vbe_mode_info_t vmi_p);

void draw_xpm(uint16_t x, uint16_t y, uint64_t video_mem, xpm_map_t* xpm);
