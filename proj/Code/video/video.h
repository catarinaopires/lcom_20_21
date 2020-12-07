#pragma once

#include <stdint.h>
#include <minix/type.h>
#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <lcom/liblm.h>
#include <machine/int86.h>


#define BIOS_VIDEO_FUNCTION 0x10                           // BIOS video function        

/** 
 * VBE useful defines 
*/
#define VBE_RETURN_VBE_MODE_INFO     0x4F01                // Implementation for video_vbe_get_mode_info
#define VBE_SET_VBE_MODE             0x4F02                // Changes display modes
#define VBE_SET_GET_DISPLAY_START    0x4F07                // Function to select the pixel displayed in the top left corner. Used for page flipping
#define VBE_SET_GET_DISPLAY_START_VR 0x80                  // Subfunction of VBE_SET_GET_DISPLAY_START to set the display start during vertical retrace

/**
 * Display Modes
*/
#define MODE_TEXT     0x003                                // Minix default text mode
#define MODE_1024x768 0x105                                // Indexed color mode (8)
#define MODE_1152x864 0X14C                                // Direct color mode (8:8:8:8) 32bit color
#define ENG_RED       0x8C0000
#define ENG_GREEN     0x2D00
#define ENG_BLUE      0x0016


/** @brief Struct to save relevant functions and data to manipulate graphics
 * 
*/
typedef struct video_instance video_instance;
struct video_instance{
    int32_t mode;
    vbe_mode_info_t mode_info;
    uint8_t bytesPerPixel;
    enum{
        page1,
        page2
    }page;
    void* mapped_vram_addr[2];
    int (*video_get_mode_info)(video_instance *thisInstance);
    int (*video_change_mode)(video_instance *thisInstance, uint16_t mode);
    int (*video_map_vram_mem)(video_instance *thisInstance, uint8_t bufferNr);
    int (*video_flip_page)(video_instance *thisInstance);
    void* (*video_get_current_buffer)(video_instance *thisInstance);
};

/** @brief Function to retrieve informations about the graphics card, changing the content of the struct
 * @param instance Struct video instance
 * @return returns 0 uppon success, 1 with failiure
 * */
int video_get_mode_info(video_instance *instance);

/** @brief Funtion to change between display modes
 * @param thisInstance Reference to struct video instance
 * @param mode Mode to change to
 * @return returns 0 uppon success, 1 with failiure
 * **/
int video_change_mode(video_instance *instance, uint16_t mode);

/** @brief Function to map graphics memory to the process' address space
 * @param instance Reference to struct video instance
 * @param bufferNr Number of buffer (between 1 and 2)
 * @return returns 0 uppon success, 1 with failiure
 * */
int video_map_vram_mem(video_instance *instance, uint8_t bufferNr);

/** @brief Function to flip pages with the 0x07's bios function subfuction 0x80
 * @param instance Reference to struct video instance
 * @return Returns 0 uppon success, 1 with failiure
*/
//static int video_flip_page(video_instance *instance);

/** @brief Returns the current buffer in use
 * @param instance Reference to struct video instance
 * @return Pointer to the current buffer in use
*/
void* video_get_current_buffer(video_instance *instance);

/** @brief Populates mode, mode_info, mapps memory and changes mode
 * @param instance Reference to struct video instance
 * @param mode Mode to initilize
 * @return Creates object of type video_instance
*/
//video_instance video_init_single_buffer(uint16_t mode);

/** @brief Populates mode, mode_info, mapps memory for the 2 buffers and changes mode
 * @param instance Reference to struct video instance
 * @param mode Mode to initilize
 * @return Creates object of type video_instance
*/
//video_instance video_init_double_buffer(uint16_t mode);

/** @brief Empty video_instance
 * @return Creates object of type video_instance
*/
video_instance video_init_empty();


void draw_rectangle(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint32_t color, video_instance *instance);

void draw_xpm(uint16_t x, uint16_t y, xpm_image_t* img, video_instance* instance);

void draw_pixel(uint32_t cols, uint32_t lines, uint32_t color, video_instance* instance);