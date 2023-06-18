/*
 * myTasks.c
 *
 *  Created on: Jun 2, 2023
 *      Author: rini
 */

#include "myTasks.h"

// Create a bunch of ugly static global data for demonstration purposes
// I DO NOT RECOMMEND to use global variables in your actual application!!
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

char at_cmd_buffer[AT_CMD_BUFFER_SIZE] = {0};

// NOTE: COPY & PASTING INTO SERIAL TERMINAL DOES NOT WORK!
// The serial terminal is meant for human input, since it is character by character via interrupts!

void StartDefaultTask(void *argument) {
	// Create tasks
	//xTaskCreate(uart2Task, "Uart1Task", 128, NULL, osPriorityLow, NULL);
	xTaskCreate(UartHandlerTask, xUartHandlerTaskName, 128, NULL, osPriorityNormal1, &xUartTaskHandle);

	// Activate UART interrupts and reception
	LL_USART_EnableIT_IDLE(USART1); // Enable idle line detection (interrupt) for uart1
	// NOTE: Please check stm32l4xx_it.c for the USER-CODE that handles the IDLE Line Interrupt!!
	HAL_UART_Receive_DMA(&huart1, uart1Buffer, BUFFER_SIZE);

	// Wifi-BLE Click configuration
	// The UART output works for the initial configuration, but I recommend to have the serial terminal open already
	// The demo application is intended for interactive usage over terminal.
	// The initial configuration is supposed to make re-occuring setup easier to quickly get into testing
	// Delays between commands are recommended, otherwise the ESP32 responds with "Busy p..."
	// A delay of 10 microseconds usually is enough, but it depends on the command!

	// ... add your initial configuration commands below ...

	// Example configuration (I recommend you try these commands over serial terminal first)

	// Configure Station+AP Mode
	at_set_command(at_cmd_buffer, SendATCommand, AT_WIFI_Set_Mode, "%u", AT_WIFI_Mixed_Mode);
	osDelay(10);
	// Allow multiple connections
	at_set_command(at_cmd_buffer, SendATCommand, AT_IP_Set_MultiConnectionMode, "%u", AT_IP_ConnectionMode_Multiple);
	osDelay(10);
	// Start TCP server on Port 80
	at_set_command(at_cmd_buffer, SendATCommand, AT_IP_Server, "%u,%u", AT_IP_Server_Create, 80);
	osDelay(10);
	// CONNECTs to YOUR-SSID with YOUR-WIFI-PWD
	at_set_command(at_cmd_buffer, SendATCommand, AT_WIFI_Connect, "\"%s\",\"%s\"", "YOUR-SSID", "YOUR-WIFI-PWD");
	osDelay(10);
	at_execute_command(at_cmd_buffer, SendATCommand, AT_Startup);
	osDelay(10);

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
					// The transmissions here are also in blocking mode, otherwise output is cutoff!
					// If received data is BIGGER than buffer size, then output will be cutoff! (=> Increase buffer size)
					if (new_pos > old_pos)  // If data does not wrap around the buffer
					{
						length = new_pos - old_pos;
						// Process your data => uart1Buffer[old_pos] TO uart1Buffer[old_pos+length] == Received DATA
						HAL_UART_Transmit(&huart2, &uart1Buffer[old_pos], length, HAL_MAX_DELAY);
					}
					else  // If data wraps around the buffer
					{
						// If you process data in here, you'll need to partially construct your data

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
// Interrupt callback routine for UART
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

void SendATCommand(char *command, int length) {
	/*
	// The command sent MUST NOT contain a newline, otherwise it might be error-prone
	uint8_t cmd_length = strlen(command); // If your string is not NULL-Terminated this will cause errors!
	uint8_t *buffer = (uint8_t*)pvPortMalloc(sizeof(char)*cmd_length+2);
	strncpy((char*)buffer, command, cmd_length);
	buffer[cmd_length] = '\r';
	buffer[cmd_length+1] = '\n';
	*/
	HAL_UART_Transmit(&huart1, (uint8_t*)command, length, HAL_MAX_DELAY);
	//vPortFree(buffer);
}
