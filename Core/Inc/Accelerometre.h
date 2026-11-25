#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"

/* Private define ------------------------------------------------------------*/
#define MPU6050_ADDR (0x68 << 1)
#define ADRESSE_LECTURE 0xD1 
#define INT_ENABLE			 0b0000001	
#define PWR_MGMT_1_0 		 	0b0000000			
#define GYRO_CONFIG_250  	0b0000000		
#define ACCEL_CONFIG_2g  	0b0000000			
#define CONFIG_94Hz			 	0b0000010			
#define INT_ENABLE			 	0b0000001		
#define WAI_ADDR 0x75 
#define PWR_ADDR 0x6B 
#define GYRO_ADDR 0x1B 
#define ACCEL_ADDR 0x1C 
#define CONFIG_ADDR 0x1A 
#define INT_ADDR 0x38 
#define ACCEL_XOUT_H 0x3B 
#define ACCEL_SCALE_FACTOR 16384.0f 
#define ARM_MATH_CM4
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


//Acceleration variables
extern volatile int Data_flag;
extern int nx, ny;
extern float  x_accel;
extern float y_accel;
extern volatile int counter_var, time_var;
extern volatile int16_t accel_x;
extern volatile int16_t accel_y;


void Init_MPU6050();
void See_Acceleration();
void Direction(int* nx, int* ny);

