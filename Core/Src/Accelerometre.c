#include "Accelerometre.h"
#include "i2c.h"
#include "stdio.h"


volatile int Data_flag = 0;
volatile int counter_var = 0, time_var = 0;
float x_accel, y_accel;
int nx, ny;
volatile int16_t accel_x, accel_y;


// Initialisation MPU6050
void Init_MPU6050() {
    uint8_t power_mgmt = 0x00;
    uint8_t gyro_config = 0x00;
    uint8_t accel_config = 0x00;
    uint8_t filter_config = 0x02;
    uint8_t interrupt_enable = 0x01;


HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x6B, 1, &power_mgmt, 1, 50); 
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1B, 1, &gyro_config, 1, 50); 
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1C, 1, &accel_config, 1, 50); 
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1A, 1, &filter_config, 1, 50); 
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x38, 1, &interrupt_enable, 1, 50); 

}

// Lecture des données d'accélération
void See_Acceleration() {
	
    uint8_t accel_data[6];
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR | 0x01, ACCEL_XOUT_H, 1, accel_data, 6, 100);
    accel_y = ((int16_t)accel_data[0] << 8) | accel_data[1]; 
    accel_x = ((int16_t)accel_data[2] << 8) | accel_data[3];
    x_accel = (float)(accel_x / ACCEL_SCALE_FACTOR);
    y_accel = (float)(accel_y / ACCEL_SCALE_FACTOR);
		Direction(&nx, &ny);

}

void Direction(int* nx, int* ny) {
			*nx = 0;
			if (x_accel > 0.2) {
				*nx = 1;
			}
			else if (x_accel < -0.2) {
				*nx = -1;
			}
			*ny = 0;
			if (y_accel > 0.2) {
				*ny = 1;
			}
			else if (y_accel < -0.2) {
				*ny = -1;
			}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_11) {
        Data_flag = 1;
    }
}


