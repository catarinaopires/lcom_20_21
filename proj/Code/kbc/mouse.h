#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Type to store the bytes of a mouse packet read directly from the kbc
 */
typedef uint8_t mouse_packet_raw[3];

/**
 * @brief Type to save processed mouse packets
 */
typedef struct mouse_packet_processed{
  bool rb, mb, lb;  // right, middle and left mouse buttons pressed
  int16_t delta_x;  // mouse x-displacement: rightwards is positive
  int16_t delta_y;  // mouse y-displacement: upwards is positive
  bool x_ov, y_ov;  // mouse x-displacement and y-displacement overflows
}mouse_packet_processed;

/** @brief Function to process the mouse packets read from kbc
 * @param packet mouse_packet_raw read from the kbc
 * @param processed mouse_packet processed
 */
void mouse_process_packets(mouse_packet_raw* packet, mouse_packet_processed* processed);