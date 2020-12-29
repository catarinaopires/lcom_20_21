#include "mouse.h"
#include "i8042.h"
#include <lcom/lcf.h>

int mouse_write_cmd(uint8_t cmd) {
  uint8_t stat = 0;
  uint8_t ack = 0;

  while (1) {
    if (util_sys_inb(KBC_ST_REG, &stat) != 0)
      return 1;

    if ((stat & KBC_INPUT_BUFF_FULL) == 0) {

      if (sys_outb(KBC_CMD, KBC_WRITE_MOUSE) != OK)
        return 1;

      if (util_sys_inb(KBC_ST_REG, &stat) != 0)
        return 1;

      if ((stat & KBC_INPUT_BUFF_FULL) == 0) {

        if (sys_outb(KBC_CMD_ARGS, cmd) != OK)
          return 1;

        if (util_sys_inb(KBC_OUT_BUF, &ack) != 0)
          return 1;

        if (ack == MOUSE_RESPONSE_ACK)
          return 0;
      }
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
}

void mouse_process_packet(mouse_packet_raw *packet, mouse_packet_processed *processed) {
  processed->rb = (packet->bytes[0] & MOUSE_DATA_RB);
  processed->lb = (packet->bytes[0] & MOUSE_DATA_LB);
  processed->mb = (packet->bytes[0] & MOUSE_DATA_MB);

  processed->x_ov = (packet->bytes[0] & MOUSE_DATA_X_OVF);
  processed->y_ov = (packet->bytes[0] & MOUSE_DATA_Y_OVF);

  processed->delta_x = (packet->bytes[0] & MOUSE_DATA_MSB_XDELTA) ? (packet->bytes[1] | ~(0xFF)) : (packet->bytes[1]);
  processed->delta_y = (packet->bytes[0] & MOUSE_DATA_MSB_YDELTA) ? (packet->bytes[2] | ~(0xFF)) : (packet->bytes[2]);
}