
#include "Communication.h"
#include "stdio.h"

UART_State current_state = STATE_IDLE;
uint8_t start_byte = 0xFF;
uint8_t rx_byte;
uint8_t message_size = 0;
volatile uint8_t message_buffer[buffer_size]; // Adjust size as needed
uint8_t received_bytes = 0;

volatile uint8_t flag_start_received = 0;        
volatile uint8_t flag_size_received = 0;        
volatile int flag_message_complete = 0;     
volatile uint8_t flag_error = 0;  

HAL_StatusTypeDef UART_SendMessage(UART_HandleTypeDef *huart, uint8_t *message, uint8_t message_size) {
    if (message == NULL || message_size == 0) {
        return HAL_ERROR; 
    }

    uint8_t start_byte = 0xFF; 
    uint8_t tx_buffer[256] = {0};   
    uint16_t total_size = 2 + message_size; 

 
    tx_buffer[0] = start_byte;
    tx_buffer[1] = message_size; 
    for (uint8_t i = 0; i < message_size; i++) {
        tx_buffer[2 + i] = message[i];
    }


    return HAL_UART_Transmit(huart, tx_buffer, total_size, HAL_MAX_DELAY);
}



void UART_ProcessByte(uint8_t byte) {
    switch (current_state) {
        case STATE_IDLE:
            if (byte == start_byte) {
                current_state = STATE_NEW_TRANSMISSION; 
                flag_start_received = 1; 
            }
            break;

        case STATE_NEW_TRANSMISSION:
            message_size = byte; 
            received_bytes = 0;  
            flag_size_received = 1; 
            if (message_size > 0 && message_size < buffer_size) {
                current_state = STATE_RECEIVE;
            } else {
                current_state = STATE_IDLE; 
								printf("invalid size");
            }
            break;

        case STATE_RECEIVE:
            if (received_bytes < message_size) {
                message_buffer[received_bytes++] = byte; 
                if (received_bytes == message_size) {
                   
                    current_state = STATE_IDLE;
                    flag_message_complete = 1;
                }
            } else {
                
                flag_error = 1;
                current_state = STATE_IDLE;
							printf("erreur biz je sais pas");
            }
            break;

        default:
            current_state = STATE_IDLE;
            flag_error = 1;
            break;
    }
}

