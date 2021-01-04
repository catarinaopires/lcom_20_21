#pragma once
#include <stdint.h>
#include "mouse.h"
#include <lcom/lab4.h>

/**
 * @brief Mouse event.
 */
typedef struct mouse_event {
  enum mouse_ev_t type;       // Type of event
  int counter_x, counter_y;   // Counters for displacement
} mouse_event;

/**
 * @brief State machine for horizontal movement pressing the Left Button.
 */
typedef enum horizontal_state{ 
  INIT_H,
  DRAW_H,
  COMP_H,
  FAIL_H 
} horizontal_state;

/** @brief Receives mouse Event and updates state in machine state for horizontal drawing.
 * @param drawState Machine state
 * @param mouseState Mouse event
 * @param x_len Length of x of drawing to complete
 * @param tolerance Tolerance of y for horizontal drawing
 */
void gestureDetection_draw_process_state_H(horizontal_state* drawState, mouse_event* mouseState, uint8_t x_len, uint8_t tolerance);

/** @brief Detect mouse event.
 * @param processed_packet Processed packet
 * @param current Current mouse event, updated in function
 */
void gestureDetection_detect_event(mouse_packet_processed *processed_packet, mouse_event* current);
