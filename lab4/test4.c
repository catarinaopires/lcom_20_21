#include "test4.h"

void (mouse_detect_event_ours)(struct packet *pp, struct mouse_ev* current) {

  if(pp->rb){
    current->type = RB_PRESSED;
  }
  else if(pp->lb){
    current->type = LB_PRESSED;
    printf("%d \n", current->type);
  }
  else if(!pp->rb && (current->type == RB_PRESSED)){
    current->type = RB_RELEASED;
    printf("%d \n", current->type);
  }
  else if(!pp->lb && (current->type == LB_PRESSED)){
    current->type = LB_RELEASED;
    printf("%d \n", current->type);
  }
  current->delta_x += pp->delta_x;
  current->delta_y += pp->delta_y;

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
