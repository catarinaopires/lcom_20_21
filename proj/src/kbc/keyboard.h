#pragma once
#include <stdint.h>

// Detect 2 byte scancodes
#define KEYBOARD_SCANCODE_LEN_2 0xE0  // First byte of 2-byte scancodes

// Useful make/break codes

#define KEYBOARD_UP_MAKECODE 0x48     // Makecode of the up arrow
#define KEYBOARD_UP_BREAKCODE 0xC8    // Breakcode of the up arrow
#define KEYBOARD_DOWN_MAKECODE 0x50   // Makecode of the down arrow
#define KEYBOARD_DOWN_BREAKCODE 0xD0  // Breakcode of the down arrow
#define KEYBOARD_LEFT_MAKECODE 0x4B   // Makecode of the left arrow
#define KEYBOARD_LEFT_BREAKCODE 0xCB  // Breakcode of the left arrow
#define KEYBOARD_RIGHT_MAKECODE 0x4D  // Makecode of the right arrow
#define KEYBOARD_RIGHT_BREAKCODE 0xCD // Breakcode of the right arrow
#define KEYBOARD_ESC_BREAKCODE 0x81   // Breakcode for ESC

#define KEYBOARD_P_MAKECODE 0x19
#define KEYBOARD_P_BREAKCODE 0x99
#define KEYBOARD_M_MAKECODE 0x32
#define KEYBOARD_M_BREAKCODE 0xC2
#define KEYBOARD_J_MAKECODE 0x24
#define KEYBOARD_J_BREAKCODE 0xA4

// Breakcode flag (Output buffer's bit 7 is high)
#define KEYBOARD_BREAKCODE_FLAG BIT(7) // Breakcode Flag (When up the scancode is a breakcode)

typedef uint8_t keyboard_packet[2];

/**
 * @brief Possible directions based on arrows' keys.
 */
typedef enum direction {
  none,
  right,
  left,
  up,
  down,
} direction;

/**
 * @brief Vector for the arrows
 */
static uint8_t r_l_arrows[2];

/**
 * @brief Makecodes for game of keys.
 */
static uint8_t keys_game[3];

/** @brief Parses scancode and changes direction based on it.
 * If given scancode if a makecode or breakcode of the keys in the array 'keys', direction changes.
 * Else, discards it.
 */
void check_movement_r_l(uint8_t *bytes, direction *dir, uint8_t *keys);

/** @brief Keyboard interruption handler.
 * @return Returns 1 if scancode is makecode, 0 if is breakcode.
 */
int is_make_code(uint8_t *buff_data);

int assemble_keys(uint8_t* bytes, uint8_t* keys);
