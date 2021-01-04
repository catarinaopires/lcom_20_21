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

  if(!bytes[0])
    return 0;

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
