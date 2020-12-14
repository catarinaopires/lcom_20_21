#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */
#define TIMER1_IRQ 0 /**< @brief Timer 1 IRQ line */
#define TIMER2_IRQ 0 /**< @brief Timer 2 IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

/** 
 * @brief Enum with the available counters
 */
typedef enum timer_nr{
    timer0,
    timer1,
    timer2,
    timerCtrl
}timer_nr;

/**
 * @brief Enum with available modes 
*/
typedef enum timer_mode{
    mode0,
    mode1,
    mode2,
    mode3,
    mode4,
    mode5
}timer_mode;

/** @brief Function to parse the timer_nr to the correspondent port
 * @param timer Number of the timer 
 * @return The correspondent timer port, -1 if invalid
*/
int i8254_parse_port(timer_nr timer);

/** @brief Function to select the bitmask for use with controlwords
 * @param timer Timer to select
 * @return Bitmask fot the selected timer, -1 if invalid
*/
int8_t i8254_parse_selection(timer_nr timer);

/** @brief Function to parse the timer_nr to the correspondent irq line
 * @param timer Number of the timer 
 * @return The correspondent irq_line, -1 if invalid
*/
int i8254_parse_irq(timer_nr timer);

/**
 * @brief Changes the operating frequency of a timer
 * 
 * Must use the read-back command so that it does not change 
 *   the 4 LSBs (mode and BCD/binary) of the timer's control word.
 * 
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Timer operating frequency
 * @return Return 0 upon success and non-zero otherwise
 */
int i8254_set_frequency(timer_nr timer, uint32_t freq);

/** @brief Function to assemble control word with the desired mode
 * @param timer Timer to configure
 * @param mode Mode to configure the timer
 * @return Control word for the i8254
*/
uint8_t i8254_get_control_word(timer_nr timer, timer_mode mode, uint8_t rd_wrt, uint8_t bcd);

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
//void(timer_int_handler)();

/**
 * @brief Reads the input timer configuration (status) via read-back command
 *
 * @param timer Timer whose configuration to read (Ranges from 0 to 2)
 * @param st    Address of memory position to be filled with the timer config
 * @return Return 0 upon success and non-zero otherwise
 */
int i8042_get_conf(timer_nr timer, uint8_t *st);

/**
 * @brief Increments elapsed time count
 * 
 * @return Returns the current time count
 */
//uint32_t(timer_print_elapsed_time)();
