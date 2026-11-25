/*
 * Menu.h
 *
 *  Created on: Oct 22, 2024
 *      Author: joerg
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "Graphics.h"
#include "Game.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"


	


#define ARM_MATH_CM4
#define MPU6050_ADDR (0x68 << 1)      // MPU6050 I2C address
#define REGISTER_DATA 0x3B           
#define ACCEL_SCALE_2g 16384          
#define WHO_AM_I_REG 0x75             
#define WHO_AM_I_EXPECTED 0x68        
#define NUMBER_TRIALS 1								
#define MAX_TIMEOUT 100u							
#define PWR_MGMT_1_REG 0x6B    				
#define GYRO_CONFIG_REG 0x1B					
#define ACCEL_CONFIG_REG 0x1C					
#define CONFIG_REG 0x1A								
#define INT_ENABLE_REG 0x38						
#define INT_ENABLE			 0b0000001	
#define PWR_MGMT_1_0 		 	0b0000000			
#define GYRO_CONFIG_250  	0b0000000		
#define ACCEL_CONFIG_2g  	0b0000000			
#define CONFIG_94Hz			 	0b0000010			
#define INT_ENABLE			 	0b0000001		
#define ADRESSE_LECTURE 0xD1 

#define MAX_BUFFER_SIZE 6

#define TX_BUFFER_SIZE 8

void choosePlayer(ili9341_t *lcd, player_t *players);

void MPU6050_Configure();
void MPU6050_Init(void);

/**
  * @brief Read Accelerometer Data from MPU-6050
  */

void MPU6050_Process_Data(void);

HAL_StatusTypeDef MPU6050_CheckConnection(void);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

int change_dir(float ax);

#endif /* INC_MENU_H_ */
