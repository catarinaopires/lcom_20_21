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
/*
void draw_process_state(state_t* drawState, struct mouse_ev* mouseState, uint8_t x_len, uint8_t tolerance){
  switch (*drawState) {
    case INIT1:
      printf("\nIN INIT1\n\n");
      if (mouseState->type == LB_PRESSED) {
        mouseState->delta_x = 0;
        mouseState->delta_y = 0;
        *drawState = DRAW1;
      }
      break;

    case DRAW1:
      printf("\nIN DRAW1\n\n");
      if(mouseState->delta_y < -tolerance){
        *drawState = INIT1;
      }
      if (mouseState->type == LB_RELEASED) {
        if(mouseState->delta_x < x_len){
          *drawState = INIT1;
        }
        else{
          *drawState = INIT2;
        }
      }
      break;

    case INIT2:
      printf("\nIN INIT2\n\n");
      if (mouseState->type == RB_PRESSED) {
        mouseState->delta_x = 0;
        mouseState->delta_y = 0;
        *drawState = DRAW2;
      }
      break;

    case DRAW2:
      printf("\nIN DRAW2\n\n");
      if(mouseState->delta_y > tolerance){
        *drawState = INIT1;
      }
      if (mouseState->type == RB_RELEASED) {
        if(mouseState->delta_x < x_len){
          *drawState = INIT1;
        }
        else{
          *drawState = COMP;
        }
      }
      break;

    case COMP:
      printf("\nIN COMP\n\n");
      break;
  }
}

*/
// Horizontal Movement
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
/*
// Vertical Movement
void draw_process_state_V(vertical_state* drawState, struct mouse_ev* mouseState, uint8_t y_len, uint8_t tolerance){
  switch (*drawState) {
    case INIT_V:
      printf("\nIN INIT\n\n");
      if (mouseState->type == LB_PRESSED) {
        mouseState->delta_x = 0;
        mouseState->delta_y = 0;
        *drawState = DRAW_V;
      }
      break;

    case DRAW_V:
      if(abs(mouseState->delta_x) > abs(tolerance)){
        *drawState = INIT_V;
      }
      if (mouseState->type == LB_RELEASED) {
        if(abs(mouseState->delta_y) < y_len){
          *drawState = INIT_V;
        }
        else{
          *drawState = COMP_V;
        }
      }
      break;

    case COMP_V:
      break;
  }
}

// L Movement
void draw_process_state_C(vertical_state* drawStateV, horizontal_state* drawStateH, struct mouse_ev* mouseState, uint8_t len, uint8_t tolerance){
  if(*drawStateV != COMP_V){
    draw_process_state_V(drawStateV, mouseState, len, tolerance);
  }
  else{
    draw_process_state_H(drawStateH, mouseState, len, tolerance);
  }
}
*/
