/*
 * myTasks.h
 *
 *  Created on: Jun 2, 2023
 *      Author: rini
 */

#ifndef INC_MYTASKS_H_
#define INC_MYTASKS_H_

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "usart.h"
#include "task.h"
#include "string.h"
#include "stdbool.h"
#include "stm32l4xx_ll_usart.h"

// Define bit masks for the two types of events
#define UART1_IDLE_EVENT (1 << 0)
#define UART2_EVENT (1 << 1)

#define BUFFER_SIZE 4096
#define QUEUE_ITEM_SIZE sizeof(uint8_t*)

void StartDefaultTask(void *argument);
void WatchdogTask(void *argument);
void UartHandlerTask(void *argument);


#endif /* INC_MYTASKS_H_ */
