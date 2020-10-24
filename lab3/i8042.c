#include "i8042.h"
#include <lcf/lcf.h>

void kbc_ih() {
}

int is_make_code(){
  if(OUTPUT_BUFF_DATA & KBC_BREAKCODE_FLAG){
    return 0;
  }
  return 1;
}