#pragma once
#include "../program_options.h"
#include <stdint.h>

/**
 * @brief Struct to keep hookId and IRQ of each interruption.
 */
typedef struct interrupt_info{
    int hook_id;
    int irq_line;
}interrupt_info;

/**
 * @brief Array of interruptions info.
 */
interrupt_info INTERRUPTS[INTERRUPTS_SIMULTANEOUSLY_SUBSCRIBED];

/** @brief Initialize array to of given interrupts.
 * @param arr Array of interrupts info
 */
void interrupt_arr_initializer(interrupt_info* arr);

/** @brief Creates interrupt_info object with given data.
 * @param irq IQR line of interruption
 * @param bit_no Bit No of interruption
 * @return Returns object of interrupt_info created with given data.
 */
interrupt_info interrupt_info_collector(const int* irq, const uint8_t* bit_no);

/** @brief Subscribes interrupt adding its information to the array.
 * @param irq IRQ line
 * @param policy Policy
 * @param bit_no Bit No
 * @return Returns 0 upon success, 1 otherwise.
 */
int interrupt_subscribe(int irq, int policy, uint8_t *bit_no);

/** @brief Unsubscribe interruption with the give IRQ line.
 * @param irq IRQ line
 * @return Returns 0 upon success, 1 otherwise.
 */
int interrupt_unsubscribe(int irq);

/** @brief Unsubscribe all interruptions of array.
 * @return Returns 0 upon success, 1 otherwise.
 */
int interrupt_unsubscribe_all();
