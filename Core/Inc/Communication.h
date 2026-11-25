
#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#pragma once

#include "stm32f4xx_hal.h"
#define buffer_size 20
extern volatile int flag_message_complete;
extern volatile uint8_t message_buffer[buffer_size];
extern uint8_t rx_byte;

typedef enum {
    STATE_IDLE,
    STATE_NEW_TRANSMISSION,
    STATE_RECEIVE
} UART_State;

void UART_ProcessByte(uint8_t byte);

HAL_StatusTypeDef UART_SendMessage(UART_HandleTypeDef *huart, uint8_t *message, uint8_t message_size);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);