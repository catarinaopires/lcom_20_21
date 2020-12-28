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

// Data precessing from the ST register
#define KBC_OUTPUT_BUFF_FULL BIT(0) // Output buffer full flag, when bit 0 from ST is full
#define KBC_INPUT_BUFF_FULL BIT(1)  // Input buffer full flag, when bit 1 from ST is full
#define KBC_PAR_ERR BIT(7)          // Parity error flag, bit 7 goes high
#define KBC_TO_ERR BIT(6)           // Timeout error flag, bit 6 goes high
#define KBC_AUX BIT(5)              // If high OBF came from mouse, if 0 came from kbd

/** @brief Keyboard interruption handler.
*/
void (kbc_ih)();

int kbc_is_mouse_packet(uint8_t* byte);
