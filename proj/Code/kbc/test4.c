#include "test4.h"

void (mouse_detect_event_ours)(struct packet *pp, struct mouse_ev* current) {

  if(pp->rb){
    current->type = RB_PRESSED;
  }
  else if(pp->lb){
    current->type = LB_PRESSED;
  }
  else if(!pp->rb && (current->type == RB_PRESSED)){
    current->type = RB_RELEASED;
  }
  else if(!pp->lb && (current->type == LB_PRESSED)){
    current->type = LB_RELEASED;
  }
  current->delta_x += pp->delta_x;
  current->delta_y += pp->delta_y;

  printf("delta_x = %d\n", current->delta_x );
  printf("delta_y = %d\n", current->delta_y );


  return;
}

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


// Horizontal Movement
void draw_process_state_H(horizontal_state* drawState, struct mouse_ev* mouseState, uint8_t x_len, uint8_t tolerance){
  switch (*drawState) {
    case INIT_H:
      printf("\nIN INIT\n\n");
      if (mouseState->type == RB_PRESSED) {
        mouseState->delta_x = 0;
        mouseState->delta_y = 0;
        *drawState = DRAW_H;
      }
      break;

    case DRAW_H:
      if(abs(mouseState->delta_y) > abs(tolerance)){
        *drawState = INIT_H;
      }
      if (mouseState->type == RB_RELEASED) {
        if(abs(mouseState->delta_x) < x_len){
          *drawState = INIT_H;
        }
        else{
          *drawState = COMP_H;
        }
      }
      break;

    case COMP_H:
      break;
  }
}

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

// Cross Movement
void draw_process_state_C(vertical_state* drawStateV, horizontal_state* drawStateH, struct mouse_ev* mouseState, uint8_t len, uint8_t tolerance){
  if(*drawStateV != COMP_V){
    draw_process_state_V(drawStateV, mouseState, len, tolerance);
  }
  else{
    draw_process_state_H(drawStateH, mouseState, len, tolerance);
  }
}
