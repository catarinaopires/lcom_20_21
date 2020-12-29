#pragma once
#include <stdint.h>
#include <lcom/lcf.h>

/* /\*/
typedef enum { INIT1,
  DRAW1,
  INIT2,
  DRAW2,
  COMP } state_t;


typedef enum vertical_state{ 
  INIT_V,
  DRAW_V,
  COMP_V } vertical_state;

typedef enum horizontal_state{ 
  INIT_H,
  DRAW_H,
  COMP_H } horizontal_state;

void draw_process_state(state_t* drawState, struct mouse_ev* mouseState,uint8_t x_len, uint8_t tolerance);
void draw_process_state_H(horizontal_state* drawState, struct mouse_ev* mouseState,uint8_t x_len, uint8_t tolerance);
void draw_process_state_V(vertical_state* drawState, struct mouse_ev* mouseState, uint8_t y_len, uint8_t tolerance);
void draw_process_state_C(vertical_state* drawStateV, horizontal_state* drawStateH, struct mouse_ev* mouseState, uint8_t len, uint8_t tolerance);
void (mouse_detect_event_ours)(struct packet *pp, struct mouse_ev* current);
