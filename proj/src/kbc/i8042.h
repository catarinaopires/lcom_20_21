#pragma once
#include <lcom/utils.h>
#include <stdint.h>

// Define Keyboard Controller (KBC) IRQ
#define KBC_IRQ 1 // KBC IRQ

// Define some useful KBC ports
#define KBC_ST_REG 0x64   // Status register (ST)
#define KBC_CMD 0x64      // Command register (CMDREG)
#define KBC_CMD_ARGS 0x60 // Argument register for some commands
#define KBC_OUT_BUF 0x60  // Output register

// KBC COMMANDS
#define KBC_CMD_READ 0x20    // Read command
#define KBC_CMD_WRITE 0x60   // Write command
#define KBC_WRITE_MOUSE 0xD4 // Write command to mouse

// Define wait time for KBC poll reads
#define WAIT_KBC 20000 // Max KBC's response time

// Data precessing from the ST register
#define KBC_OUTPUT_BUFF_FULL BIT(0) // Output buffer full flag, when bit 0 from ST is full
#define KBC_INPUT_BUFF_FULL BIT(1)  // Input buffer full flag, when bit 1 from ST is full
#define KBC_PAR_ERR BIT(7)          // Parity error flag, bit 7 goes high
#define KBC_TO_ERR BIT(6)           // Timeout error flag, bit 6 goes high
#define KBC_AUX BIT(5)              // If high OBF came from mouse, if 0 came from kbd

/** @brief KBC interruption handler.
*/
void(kbc_ih)();

/** @brief Function to determine if the byte to read comes from mouse
 * @return 1 if comes from mouse, 0 if it doesn't, < 0 if there's any error
 */
int kbc_is_mouse_packet();

/** @brief Function that makes sure that the byte read from the kbc comes from mouse, checking the Bit 3 of the byte read
 * @return 1 if true, 0 if false
 */
int kbc_is_first_mouse_packet(uint8_t *st_byte);
