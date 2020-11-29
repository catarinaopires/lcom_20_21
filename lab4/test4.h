#pragma once
#include <stdint.h>
#include <lcom/lcf.h>


typedef enum { INIT1,
  DRAW1,
  INIT2,
  DRAW2,
  COMP } state_t;

void draw_process_state(state_t* drawState, struct mouse_ev* mouseState,uint8_t x_len, uint8_t tolerance);

void (mouse_detect_event_ours)(struct packet *pp, struct mouse_ev* current);
