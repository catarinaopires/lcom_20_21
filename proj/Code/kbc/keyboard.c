#include "keyboard.h"
#include <lcom/lcf.h>
static uint8_t r_l_arrows[2] = {KEYBOARD_LEFT_MAKECODE, KEYBOARD_RIGHT_MAKECODE};
static uint8_t keys_game[3] = {KEYBOARD_P_MAKECODE, KEYBOARD_M_MAKECODE, KEYBOARD_J_MAKECODE};

int is_make_code(uint8_t* buff_data){
  if(*buff_data & KEYBOARD_BREAKCODE_FLAG){
    return 0;
  }
  return 1;
}

void check_movement_r_l(uint8_t* bytes, direction* dir, uint8_t* keys){
  for(size_t i = 0; i < 2; i++){
    // If breakcode of some arrow, remove it from array of keys
    if((keys[i] | BIT(7)) == bytes[1]){
      keys[i] = 0;
      break;
    }
  }

  // If makecode of some arrow, add it in array of keys
  for(size_t j = 0; j < 2; j++){
    if(bytes[1] == r_l_arrows[j]){
      keys[j] = bytes[1];
      break;
    }
  }

  if(keys[0] != 0 && keys[1] != 0){
    *dir = none;
  }
  else if (keys[0]){
    *dir = left;
  }
  else if (keys[1]){
    *dir = right;
  }
  else{
    *dir = none;
  }
}

int assemble_keys(uint8_t* bytes, uint8_t* keys){
  int wrong = 1;

  // If makecode of some key, add it in array of keys
  for(size_t j = 0; j < 3; j++){
    if(bytes[0] == keys_game[j]){
      keys[j] = bytes[0];
      wrong = 0;
    }
  }

  if(!wrong) {
    for (size_t j = 0; j < 3; j++) {
      // Does not have all keys of game
      if (keys[j] == 0) {
        return 0;
      }
    }
    // Level completed with success
    return 1;
  }
  else{
    return -1;
  }
}

/*static uint8_t setas[4] = {KBC_UP_MAKECODE, KBC_DOWN_MAKECODE, KBC_LEFT_MAKECODE, KBC_RIGHT_MAKECODE};*/

/*void check_movement(uint8_t bytes[], direction* dir){
    // TODO: NONE (SAME DIRECTION EX UP+DOWN) - THEN RELEASE - ERROR

    switch (*dir) {
        case right:
            if(bytes[1] == KBC_RIGHT_BREAKCODE || bytes[1] == KBC_LEFT_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_UP_MAKECODE)
                *dir = right_up;
            else if (bytes[1] == KBC_DOWN_MAKECODE)
                *dir = right_down;
            break;
        case right_down:
            if(bytes[1] == KBC_RIGHT_BREAKCODE)
                *dir = down;
            else if(bytes[1] == KBC_DOWN_BREAKCODE)
                *dir = right;
            break;
        case right_up:
            if(bytes[1] == KBC_RIGHT_BREAKCODE)
                *dir = up;
            else if(bytes[1] == KBC_UP_BREAKCODE)
                *dir = right;
            break;
        case left:
            if(bytes[1] == KBC_LEFT_BREAKCODE || bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_UP_MAKECODE)
                *dir = left_up;
            else if (bytes[1] == KBC_DOWN_MAKECODE)
                *dir = left_down;
            break;
        case left_down:
            if(bytes[1] == KBC_LEFT_BREAKCODE)
                *dir = down;
            else if(bytes[1] == KBC_DOWN_BREAKCODE)
                *dir = left;
            break;
        case left_up:
            if(bytes[1] == KBC_LEFT_BREAKCODE)
                *dir = up;
            else if(bytes[1] == KBC_UP_BREAKCODE)
                *dir = left;
            break;
        case up:
            if(bytes[1] == KBC_UP_BREAKCODE || bytes[1] == KBC_DOWN_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = right_up;
            else if (bytes[1] == KBC_LEFT_MAKECODE)
                *dir = left_up;
            break;
        case down:
            if(bytes[1] == KBC_DOWN_BREAKCODE || bytes[1] == KBC_UP_MAKECODE)
                *dir = none;
            else if (bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = right_down;
            else if (bytes[1] == KBC_LEFT_MAKECODE)
                *dir = left_down;
            break;
        default:
            if(bytes[1] == KBC_UP_MAKECODE)
                *dir = up;
            else if(bytes[1] == KBC_DOWN_MAKECODE)
                *dir = down;
            else if(bytes[1] == KBC_RIGHT_MAKECODE)
                *dir = right;
            else if(bytes[1] == KBC_LEFT_MAKECODE)
                *dir = left;
            else
                *dir = none;
            break;

    }
}*/

/*void check_movement_array_idea(uint8_t* bytes, direction* dir, uint8_t* keys){

    for(size_t i = 0; i < 4; i++){
        // If breakcode of some arrow, remove it from array of keys
        if((keys[i] | BIT(7)) == bytes[1]){
            if(keys[i] == KBC_LEFT_MAKECODE)
                printf("release left\n");
            keys[i] = 0;
            break;
        }
    }

    // If makecode of some arrow, add it in array of keys
    for(size_t j = 0; j < 4; j++){
        if(bytes[1] == setas[j]){
            if(keys[j] == KBC_LEFT_MAKECODE)
                printf("pressed left\n");
            keys[j] = bytes[1];
            break;
        }
    }

    printf("u: %d,d: %d,l: %d,r: %d  \n", keys[0]!= 0, keys[1]!=0, keys[2]!=0, keys[3]!=0);
    if(keys[0] != 0 && keys[1] != 0){
        if(keys[2] != 0){
            if(keys[3] != 0){
                *dir = none;
            }
            else{
                // ERRO!!
                printf("entra\n");
                *dir = left;
            }
        }
        else{
            if(keys[3]){
                *dir = right;
            }
            else{
                *dir = none;
            }
        }
    }
    else if(keys[2] != 0 && keys[3] != 0){
        if(keys[0]){
            if(keys[1]){
                *dir = none;
            }
            else{
                *dir = up;
            }
        }
        else{
            if(keys[1]){
                *dir = down;
            }
            else{
                *dir = none;
            }
        }
    }
    else if (keys[0] != 0 && keys[2] != 0){     // UP+LEFT
        *dir = left_up;
    }
    else if (keys[0] != 0 && keys[3] != 0){
        *dir = right_up;
    }
    else if (keys[1] != 0 && keys[2] != 0){     // DOWN+LEFT
        *dir = left_down;
    }
    else if (keys[1] != 0 && keys[3] != 0){
        *dir = right_down;
    }
    else if (keys[0]){
        *dir = up;
    }
    else if (keys[1]){
        *dir = down;
    }
    else if (keys[2]){
        *dir = left;
    }
    else if (keys[3]){
        *dir = right;
    }
    else{
        *dir = *dir;
    }
}*/