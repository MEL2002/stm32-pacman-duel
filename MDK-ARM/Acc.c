#include "Accelerometre.h"
#include "i2c.h"
#include "stdio.h"

/* Variables globales --------------------------------------------------------*/
volatile int data_ready_flag = 0;
volatile int16_t accel_x, accel_y, accel_z;
volatile int counter_var = 0, time_var = 0;
float x_accel, y_accel;
int dir_x, dir_y;
/*----------------------------------------------------------------------------*/

// Initialisation du capteur MPU6050
void Sensor_Init() {
    uint8_t power_mgmt = 0x00;
    uint8_t gyro_config = 0x00;
    uint8_t accel_config = 0x00;
    uint8_t filter_config = 0x02;
    uint8_t interrupt_enable = 0x01;


HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x6B, 1, &power_mgmt, 1, 50); // Power Management
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1B, 1, &gyro_config, 1, 50); // Gyroscope Config
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1C, 1, &accel_config, 1, 50); // Accelerometer Config
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1A, 1, &filter_config, 1, 50); // Digital Low Pass Filter
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x38, 1, &interrupt_enable, 1, 50); // Interrupt Enable

}

// Lecture des données d'accélération
void Read_Acceleration() {
	
    uint8_t accel_data[6];

    // Lecture des registres de données d'accélération
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR | 0x01, ACCEL_XOUT_H, 1, accel_data, 6, 100);

    accel_y = ((int16_t)accel_data[0] << 8) | accel_data[1]; // pour avoir l'ecran dans le bon sens
    accel_x = ((int16_t)accel_data[2] << 8) | accel_data[3]; // pour avoir l'ecran dans le bon sens

    x_accel = (float)(accel_x / ACCEL_SCALE_FACTOR);
    y_accel = (float)(accel_y / ACCEL_SCALE_FACTOR);
		Choose_direction(&dir_x, &dir_y);
//		printf("X : %d, Y : %d\r\n", dir_x,dir_y);
}

void Choose_direction(int* dir_x, int* dir_y) {
			*dir_x = 0;
			*dir_y = 0;
			if (x_accel > 0.2) {
				*dir_x = 1;
			}
			else if (x_accel < -0.2) {
				*dir_x = -1;
			}
			if (y_accel > 0.2) {
				*dir_y = 1;
			}
			else if (y_accel < -0.2) {
				*dir_y = -1;
			}
}

// Callback pour l'interruption GPIO
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    // Vérifiez que l'interruption provient de la broche PC11 (EXTI11)
    if (GPIO_Pin == GPIO_PIN_11) {
        data_ready_flag = 1;
    }
}


