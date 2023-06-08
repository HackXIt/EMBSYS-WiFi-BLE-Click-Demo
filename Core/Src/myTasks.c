/*
 * myTasks.c
 *
 *  Created on: Jun 2, 2023
 *      Author: rini
 */

#include "myTasks.h"

// Create a bunch of ugly static global data for demonstration purposes

const char *xUartHandlerTaskName = "UartHandler";
const char *newLine = "\r\n";

// Create circular buffers
uint8_t uart1Buffer[BUFFER_SIZE];
uint8_t uart2Buffer[BUFFER_SIZE];

// Buffer indices
uint16_t uart1BufferIndex = 0;
uint16_t uart2BufferIndex = 0;

// temporaray data buffers
uint8_t uart2_rx_char;

// Task handles
TaskHandle_t xUartTaskHandle = NULL;

void StartDefaultTask(void *argument) {
	// Create tasks
	//xTaskCreate(uart2Task, "Uart1Task", 128, NULL, osPriorityLow, NULL);
	xTaskCreate(UartHandlerTask, xUartHandlerTaskName, 128, NULL, osPriorityLow, &xUartTaskHandle);

	// Wifi-BLE Click configuration
	// ... your initial configuration goes here ...

	// Activate UART interrupts and reception
	LL_USART_EnableIT_IDLE(USART1); // Enable idle line detection (interrupt) for uart1
	HAL_UART_Receive_DMA(&huart1, uart1Buffer, BUFFER_SIZE);
	HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);


	osThreadExit();
}

void UartHandlerTask(void *argument) {
	uint32_t ulNotificationValue;
	static size_t old_pos = 0;  // Track the position of last character processed

	while(1)
	{
		// Wait for a task notification indicating an uart event
		if(xTaskNotifyWait(0x00, UINT32_MAX, &ulNotificationValue, portMAX_DELAY) == pdPASS) {

			if(ulNotificationValue & UART1_IDLE_EVENT) // Received message from UART1
			{
				size_t new_pos = BUFFER_SIZE - huart1.hdmarx->Instance->CNDTR;  // Compute the new position in the buffer
				size_t length;

				if(new_pos != old_pos)  // Check if any new data is received
				{
					if (new_pos > old_pos)  // If data does not wrap around the buffer
					{
						length = new_pos - old_pos;
						HAL_UART_Transmit(&huart2, &uart1Buffer[old_pos], length, HAL_MAX_DELAY);
					}
					else  // If data wraps around the buffer
					{
						// First transmit the data until the end of the buffer
						length = BUFFER_SIZE - old_pos;
						HAL_UART_Transmit(&huart2, &uart1Buffer[old_pos], length, HAL_MAX_DELAY);

						// Then transmit the remaining data from the beginning of the buffer
						length = new_pos;
						HAL_UART_Transmit(&huart2, uart1Buffer, length, HAL_MAX_DELAY);
					}

					old_pos = new_pos;  // Update the position of the last character processed
				}
			}
			if(ulNotificationValue & UART2_EVENT) // Received CHARACTER from UART2
			{
				char c = (char)uart2_rx_char;

				// Echo back the character to the terminal
				HAL_UART_Transmit_IT(&huart2, (uint8_t*)&c, 1);

				// If newline, forward the buffer to UART1 and reset buffer
				if(c == '\r')
				{
					uart2Buffer[uart2BufferIndex++] = '\r';
					uart2Buffer[uart2BufferIndex++] = '\n';
					HAL_UART_Transmit_IT(&huart2, (uint8_t*)newLine, 2);
					// For now I'll ignore that there's no newline in the echo feedback
					// This call will be made in blocking mode, because we'll clear the buffer!
					HAL_UART_Transmit(&huart1, uart2Buffer, uart2BufferIndex, HAL_MAX_DELAY);
					memset(uart2Buffer, 0, BUFFER_SIZE);
					uart2BufferIndex = 0;
				}
				else
				{
					// Add character to buffer
					uart2Buffer[uart2BufferIndex++] = c;
				}
			}
		}
	}
}
// Interrupt callbacks
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2) {
	  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	  xTaskNotifyFromISR(xUartTaskHandle, UART2_EVENT, eSetBits, &xHigherPriorityTaskWoken);
	  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	  // Restart reception with interrupt
	  HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);
  }
}
