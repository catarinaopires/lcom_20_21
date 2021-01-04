#pragma once
#include <lcom/utils.h>
#include <stdbool.h>
#include <stdint.h>

// Mouse IRQ
#define MOUSE_IRQ 12 // Mouse IRQ

// Mouse commands
#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_REMOTE_MODE 0xF0
#define MOUSE_ENABLE_DATA_REP_STR 0xF4
#define MOUSE_DISABLE_DATA_REP_STR 0xF5
#define MOUSE_READ_DATA 0xEB
#define MOUSE_RESPONSE_ACK 0xFA
#define MOUSE_RESPONSE_NACK 0xFE
#define MOUSE_RESPONSE_ERR 0xFC

// Mouse data processing
#define MOUSE_DATA_LB BIT(0)
#define MOUSE_DATA_RB BIT(1)
#define MOUSE_DATA_MB BIT(2)
#define MOUSE_DATA_MSB_XDELTA BIT(4)
#define MOUSE_DATA_MSB_YDELTA BIT(5)
#define MOUSE_DATA_X_OVF BIT(6)
#define MOUSE_DATA_Y_OVF BIT(7)

/**
 * @brief Type to store the bytes of a mouse packet read directly from the kbc
 */
typedef struct mouse_packet_raw {uint8_t bytes[3];} mouse_packet_raw;

/**
 * @brief Type to save processed mouse packets
 */
typedef struct mouse_packet_processed {
  bool rb, mb, lb; // right, middle and left mouse buttons pressed
  bool x_ov, y_ov; // mouse x-displacement and y-displacement overflows
  int16_t delta_x; // mouse x-displacement: rightwards is positive
  int16_t delta_y; // mouse y-displacement: upwards is positive
} mouse_packet_processed;

/** @brief Function to process the mouse packets read from kbc
 * @param packet mouse_packet_raw read from the kbc
 * @param processed mouse_packet processed
 */
void mouse_process_packet(mouse_packet_raw *packet, mouse_packet_processed *processed);

/** @brief Writes given command in control register of KBC.
 * @param cmd Given command to write.
 * @return Returns 0 upon success, 1 otherwise
 */
int mouse_write_cmd(uint8_t cmd);
