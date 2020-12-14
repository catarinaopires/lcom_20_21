#pragma once
#include <lcom/lcf.h>
#include <stdint.h>

// Define Keyboard Controller (KBC) IRQ
#define KBC_IRQ 1

// Define some useful KBC ports
#define KBC_ST_REG 0x64             // Status register (ST)
#define KBC_CMD 0x64                // Command register (CMDREG)
#define KBC_CMD_ARGS 0x60           // Argument register for some commands
#define KBC_OUT_BUF 0x60            // Output register

// KBC COMMANDS
#define KBC_CMD_READ 0x20           // Read command
#define KBC_CMD_WRITE 0x60          // Write command

// Define wait time for KBC poll reads
#define WAIT_KBC 20000              // Max KBC's response time

// Breakcode flag (Output buffer's bit 7 is high)
#define KBC_BREAKCODE_FLAG BIT(7)   // Breakcode Flag (When up the scancode is a breakcode)

// Data precessing from the ST register
#define KBC_OUTPUT_BUFF_FULL BIT(0) // Output buffer full flag, when bit 0 from ST is full
#define KBC_INPUT_BUFF_FULL BIT(1)  // Input buffer full flag, when bit 1 from ST is full
#define KBC_PAR_ERR BIT(7)          // Parity error flag, bit 7 goes high
#define KBC_TO_ERR BIT(6)           // Timeout error flag, bit 6 goes high
#define KBC_AUX BIT(5)              // If high OBF came from mouse, if 0 came from kbd

// Detect 2 byte scancodes
#define KBC_SCANCODE_LEN_2 0xE0     // First byte of 2-byte scancodes
#define KBC_UP_MAKECODE 0x48        // Makecode of the up arrow
#define KBC_UP_BREAKCODE 0xC8       // Breakcode of the up arrow
#define KBC_DOWN_MAKECODE 0x50      // Makecode of the down arrow
#define KBC_DOWN_BREAKCODE 0xD0     // Breakcode of the down arrow
#define KBC_LEFT_MAKECODE 0x4B      // Makecode of the left arrow
#define KBC_LEFT_BREAKCODE 0xCB     // Breakcode of the left arrow
#define KBC_RIGHT_MAKECODE 0x4D     // Makecode of the right arrow
#define KBC_RIGHT_BREAKCODE 0xCD    // Breakcode of the right arrow

// Define 'Esc' breakcode for loop termination
#define KBC_ESC_BREAKCODE 0x81      // Breakcode for ESC

/** @brief Possible directions based on arrows' keys.
*/
typedef enum direction{
    none,
    right,
    left,
    up,
    down,
} direction;

/** @brief Parses scancode and changes direction based on it.
 * If given scancode if a makecode or breakcode of the keys in the array 'keys', direction changes.
 * Else, discards it.
*/
void check_movement_r_l(uint8_t* bytes, direction* dir, uint8_t* keys);

/** @brief Keyboard interruption handler.
*/
void (kbc_ih)();

/** @brief Keyboard interruption handler.
 * @return Returns 1 if scancode is makecode, 0 if is breakcode.
*/
int is_make_code();


/*void check_movement_array_idea(uint8_t* bytes, direction* dir, uint8_t* keys);*/
/*void check_movement(uint8_t bytes[], direction* dir);*/

/*
typedef enum direction{
    none,
    right,
    left,
    up,
    down,
    right_up,
    right_down,
    left_up,
    left_down
} direction;*/
