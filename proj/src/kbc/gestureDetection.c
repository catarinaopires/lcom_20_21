#include "gestureDetection.h"
#include <lcom/lcf.h>

void gestureDetection_detect_event(mouse_packet_processed *processed_packet, mouse_event *current) {

  if((processed_packet->lb && processed_packet->rb) || processed_packet->mb){
    current->type = BUTTON_EV;
  } 
  else if(processed_packet->rb){

    current->type = RB_PRESSED;
  }
  else if(processed_packet->lb){
    current->type = LB_PRESSED;
  }
  else if(!processed_packet->rb && (current->type == RB_PRESSED)){
    current->type = RB_RELEASED;
  }
  else if(!processed_packet->lb && (current->type == LB_PRESSED)){
    current->type = LB_RELEASED;
  }

  current->counter_x += processed_packet->delta_x;
  current->counter_y += processed_packet->delta_y;
  return;
}

void gestureDetection_draw_process_state_H(horizontal_state* drawState, mouse_event* mouseState, uint8_t x_len, uint8_t tolerance){
  switch (*drawState) {
    case INIT_H:
      if (mouseState->type == LB_PRESSED) {
        mouseState->counter_x = 0;
        mouseState->counter_y = 0;
        *drawState = DRAW_H;
      }
      break;

    case DRAW_H:
      if(abs(mouseState->counter_y) > abs(tolerance)){
        *drawState = FAIL_H;
      }
      if (mouseState->type == LB_RELEASED) {
        if(abs(mouseState->counter_x) < x_len){
          *drawState = FAIL_H;
        }
        else{
          *drawState = COMP_H;
        }
      }
      break;

    case COMP_H:
      break;
    case FAIL_H:
      break;
  }
}

