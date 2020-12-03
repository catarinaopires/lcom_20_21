 #pragma once
  
 #include "pixmap.h"
 #include "xpm.h"
 #include "vbe.h"
 #include "video_gr.h"
  
 int (video_test_init)(uint16_t mode, uint8_t delay);
  
 int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                            uint32_t color);
  
 int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);
  
 int (video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);
  
 int (video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                       int16_t speed, uint8_t fr_rate);
  
 int (video_test_controller)();
  