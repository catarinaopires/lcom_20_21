#pragma once

// Define Keyboard Controller (KBC) IRQ
#define KBC_IRQ 1


// Breakcode flag (Output buffer's bit 7 is high)
#define KBC_BREAKCODE_FLAG BIT(7)


// Define 'Esc' breakcode for loop termination
#define KBC_ESC_BREAKCODE 0x81

