#pragma once
#include <lcom/lab3.h>
#include <stdint.h>
// Define Keyboard Controller (KBC) IRQ
#define KBC_IRQ 1
#define KBC_MOUSE_IRQ 12

// Define some useful KBC ports
#define KBC_ST_REG 0x64   // Status register (ST)
#define KBC_CMD_REG 0x64  // Command register (CMDREG)
#define KBC_CMD_ARGS 0x60 // Argument register for some commands
#define KBC_OUT_BUF 0x60  // KBC's Output register

// KBC COMMANDS
#define KBC_CMD_READ 0x20
#define KBC_CMD_WRITE 0x60

// Mouse commands
#define KBC_WRITE_MOUSE 0xD4
#define KBC_STREAM_MODE 0xEA
#define KBC_REMOTE_MODE 0xF0
#define KBC_ENABLE_DATA_REP_STR 0xF4
#define KBC_DISABLE_DATA_REP_STR 0xF5
#define KBC_READ_DATA 0xEB
#define KBC_MOUSE_ACK 0xFA
#define KBC_MOUSE_NACK 0xFE
#define KBC_MOUSE_ERR 0xFC

// Define wait time for KBC poll reads
#define WAIT_KBC 20000

// Breakcode flag (Output buffer's bit 7 is high)
#define KBC_BREAKCODE_FLAG BIT(7)

// Data precessing from the ST register
#define KBC_OUTPUT_BUFF_FULL BIT(0) // Output buffer full flag, when bit 0 from ST is full
#define KBC_INPUT_BUFF_FULL BIT(1)  // Input buffer full flag, when bit 1 from ST is full
#define KBC_PAR_ERR BIT(7)          // Parity error flag, bit 7 goes high
#define KBC_TO_ERR BIT(6)           // Timeout error flag, bit 6 goes high
#define KBC_AUX BIT(5)              // If high OBF came from mouse, if 0 came from kbd

// Detect 2 byte scancodes
#define KBC_SCANCODE_LEN_2 0xE0

// Define 'Esc' breakcode for loop termination
#define KBC_ESC_BREAKCODE 0x81

// Mouse data processing
#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define MOUSE_MSB_XDELTA BIT(4)
#define MOUSE_MSB_YDELTA BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)


// Define for variable counter
//#define COUNT_SCAN
//#define COUNT_POLL

// Global variables needed
uint8_t OUTPUT_BUFF_DATA;
//int SCAN_COUNTER;
//int POLL_COUNTER;


int is_make_code();


void (kbc_ih)(void);

int is_make_code();

int (kbc_read_poll)();

int (mouse_write)(uint8_t cmd);

void (mouse_ih)(void);

struct packet (mouse_process_packet)(uint8_t bytes[]);

uint16_t (to16Cpl2)(uint8_t value, uint8_t msb);
