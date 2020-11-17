#pragma once
#include <stdint.h>
#include <lcom/lcf.h>
#include <machine/int86.h>
#define MODE_1024x768 0x105

int video_change_mode(uint16_t mode);

struct minix_mem_range vram_map_memory(phys_bytes base, unsigned int size);

