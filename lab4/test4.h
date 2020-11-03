#pragma once

struct packet {
  uint8_t bytes[3];
  bool rb;
  bool mb;
  bool lb;
  int16_t delta_x;
  int16_t delta_y;
  bool x_ov;
  bool y_ov;
};


void mouse_print_packet(struct packet *pp);
 