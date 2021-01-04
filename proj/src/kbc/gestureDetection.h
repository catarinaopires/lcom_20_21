#pragma once
#include <stdint.h>
#include "mouse.h"
#include <lcom/lab4.h>

/*
enum mouse_ev_t { 
  LB_PRESSED,
  LB_RELEASED,
  RB_PRESSED,
  RB_RELEASED,
  BUTTON_EV,
  MOUSE_MOV 
};
*/

typedef struct mouse_event {
  enum mouse_ev_t type;
  int counter_x, counter_y;
} mouse_event;

typedef enum { 
  INIT1,
  DRAW1,
  INIT2,
  DRAW2,
  COMP
} state_t;


typedef enum vertical_state{ 
  INIT_V,
  DRAW_V,
  COMP_V 
} vertical_state;

typedef enum horizontal_state{ 
  INIT_H,
  DRAW_H,
  COMP_H,
  FAIL_H 
} horizontal_state;

//void draw_process_state(state_t* drawState, struct mouse_event* mouseState,uint8_t x_len, uint8_t tolerance);
void gestureDetection_draw_process_state_H(horizontal_state* drawState, mouse_event* mouseState, uint8_t x_len, uint8_t tolerance);
//void draw_process_state_V(vertical_state* drawState, struct mouse_event* mouseState, uint8_t y_len, uint8_t tolerance);
//void draw_process_state_C(vertical_state* drawStateV, horizontal_state* drawStateH, struct mouse_event* mouseState, uint8_t len, uint8_t tolerance);
void gestureDetection_detect_event(mouse_packet_processed *processed_packet, mouse_event* current);
