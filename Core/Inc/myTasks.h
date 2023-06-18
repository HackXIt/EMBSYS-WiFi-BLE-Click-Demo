/*
 * myTasks.h
 *
 *  Created on: Jun 2, 2023
 *      Author: rini
 */

#ifndef INC_MYTASKS_H_
#define INC_MYTASKS_H_

#include "cmsis_os2.h"			// For CMSIS API functions
#include "FreeRTOS.h"			// For FreeRTOS API functions
#include "usart.h"				// For UART handles
#include "task.h"				// for FreeRTOS task control functions
#include "string.h"				// for string functions
#include "stdbool.h"			// for boolean values
#include "stm32l4xx_ll_usart.h" // For lowlevel usart abstraction access
#include "printf.h"				// For 3rd-party printf library specifically for embedded systems
#include "ATcommands.h"			// For AT-command-list and functions

// Define bit masks for the two types of events
#define UART1_IDLE_EVENT (1 << 0)
#define UART2_EVENT (1 << 1)

// Increase this if your output is cutoff
// Be aware that this will take up space in the stack, not the heap
// This number is also the maximum number of characters which can be received in the TCP server
#define BUFFER_SIZE 4096

void StartDefaultTask(void *argument); // Default & initial configuration task
void UartHandlerTask(void *argument); // Task which handles the intermediate communication between UART interfaces
void SendATCommand(char *command, int length); // Function which transmits a provided string to UART1


#endif /* INC_MYTASKS_H_ */
