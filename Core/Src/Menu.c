/**
 * @file Menu.c
 * @brief This file implements the game manu. The menu is used to choose a character (Pac-Man or Ghost)
 */

#include "Menu.h"
#include "main.h"
#include "i2c.h"
#include "Accelerometre.h"
#include "communication.h"

volatile int character_flag = 0;
volatile int flag = 0;
extern uint8_t data[6];	// Tableau dans lequel seront enregsitrees les valeur d'accelerations.
float y_acc;			// Valeur de l'acceleration en y.
float accelY_g;
int16_t accelY_menu;

int change_dir(float ax){
	if (ax > 0.4) return 1;
	else if (ax < -0.4) return -1;
	else return 0;
}


// Routine qui configure l'accelerometre de la facon 

void MPU6050_Configure();

//void MPU6050_Configure(void)
//{
//    uint8_t data;

//    // Reset 
//    data = 0x01;
//    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

//    // Configure accelerometer
//    data = 0x00; 
//    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

//    // Configure 
//    data = 0x01;
//    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, WHO_AM_I_REG, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
//}
// Routine du gestionnaire d'interruption
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

// Routine qui affiche les valeurs d'acceleration a l'ecran.
void display_acceleration(ili9341_t *screen, float ax, float ay, float az);

/**
* @brief The function draws the two possible players (Pac-Man or Ghost) on the screen. 
*        Additionally, it renders two hollow squares under each character and a filled 
*        square in the middle that serves as a cursor. The function is supposed to 
*        read the accelerometer value and use its values to move the cursor block
*        until it coincides with one of the hollow squares. At this point, the player 
*        is chosen and the function returns. 
* @param lcd  [in] pointer to the ili9341 handle structure used to access the tft screen
* @param players [out] array of player_t structures which contains the identities of the players when the
*        function returns
*/
void choosePlayer(ili9341_t *lcd, player_t* players){
	MPU6050_Configure();

	const uint16_t checkbox_size = 30;
	const uint16_t checkfill_size = 20;
	
	const uint8_t char_y_pos = 140;
	const character_t pacman 					= { .type = PACMAN, .color = PACMAN_COLOR };
	const position_t pacman_icon_pos 	= { .x = 180, .y = char_y_pos };
	const character_t ghost 					= { .type = GHOST,	.color = GHOST_COLOR };
	const position_t ghost_icon_pos 	= { .x = 30, .y = char_y_pos };
	
	position_t cursor_pos = { .x=(((pacman_icon_pos.x+35)-(ghost_icon_pos.x+55))/2)+15+(ghost_icon_pos.x+35), .y=185 };
	position_t old_cursor_pos = cursor_pos;
	
	character_t chosen_character = {0};
	
	ili9341_fill_screen(lcd, ILI9341_BLACK);
	
	drawGhost(lcd, ghost_icon_pos.x, ghost_icon_pos.y, ghost.color);
	drawPacMan(lcd, pacman_icon_pos.x, pacman_icon_pos.y, pacman.color);
	
	ili9341_draw_rect(lcd, ILI9341_WHITE, ghost_icon_pos.x + 35, 180, checkbox_size, checkbox_size);
	ili9341_draw_rect(lcd, ILI9341_WHITE, pacman_icon_pos.x + 35, 180, checkbox_size, checkbox_size);
	
	while(chosen_character.type == NONE) {
		HAL_Delay(20); // À remplacer avec un timer
		if (Data_flag != 0) {
				See_Acceleration(); 
				Data_flag = 0;
	}

		cursor_pos.x += nx * STEP_SIZE; // Mise à jour de la position du curseur

		char buffer_texte[40];
		ili9341_text_attr_t text_attr4 = {&ili9341_font_11x18, ILI9341_WHITE, ILI9341_BLACK,0,60};
		sprintf(buffer_texte, "Accel Y : %.6f ", accelY_g);
		ili9341_draw_string(lcd, text_attr4, buffer_texte);

		
		cursor_pos.x +=  change_dir(accelY_g) * 0.5 * STEP_SIZE; // Mise à jour de la position du curseur
		//(return_direction((int16_t)((data[2] << 8) | data[3])) / 16384.0)
		
		ili9341_fill_rect(lcd, ILI9341_BLACK, old_cursor_pos.x, old_cursor_pos.y, checkfill_size, checkfill_size);
		ili9341_fill_rect(lcd, ILI9341_WHITE, cursor_pos.x, cursor_pos.y, checkfill_size, checkfill_size);
		old_cursor_pos = cursor_pos;
		
		uint8_t pacman_chosen = cursor_pos.x >= pacman_icon_pos.x + checkbox_size + 10;
		if (pacman_chosen) {
			chosen_character = pacman;
			character_flag = 1;
			ili9341_fill_rect(lcd, ILI9341_BLACK, pacman_icon_pos.x + 35, 180, checkbox_size, checkbox_size);
			ili9341_draw_rect(lcd, pacman.color, pacman_icon_pos.x + 35, 180, checkbox_size, checkbox_size);
			ili9341_fill_rect(lcd, pacman.color, pacman_icon_pos.x + 40, cursor_pos.y, checkfill_size, checkfill_size);
		}
		uint8_t ghost_chosen = cursor_pos.x <= ghost_icon_pos.x + checkbox_size + 10;
		if (ghost_chosen) {
			chosen_character = ghost;
			character_flag = 2;
			ili9341_fill_rect(lcd, ILI9341_BLACK, ghost_icon_pos.x + 35, 180, checkbox_size, checkbox_size);
			ili9341_draw_rect(lcd, ghost.color, ghost_icon_pos.x + 35, 180, checkbox_size, checkbox_size);
			ili9341_fill_rect(lcd, ghost.color, ghost_icon_pos.x + 40, cursor_pos.y, checkfill_size, checkfill_size);
		}
		
	}
	
	players[LOCAL_PLAYER_ID].character = chosen_character;
	players[ENEMY_PLAYER_ID].character = chosen_character.type == PACMAN ? ghost : pacman;
	
	HAL_Delay(1000);
	ili9341_fill_screen(lcd, ILI9341_BLACK);
}

void MPU6050_Init(void);



void MPU6050_Process_Data(void);

HAL_StatusTypeDef MPU6050_CheckConnection(void);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

void MPU6050_Configure()
{
	/* Initialize text attributes for the display */
    ili9341_text_attr_t text_attr = {&ili9341_font_11x18, ILI9341_WHITE, ILI9341_BLACK,0,0};

    /* --------------------------- PWR_MGMT_1 --------------------------- */
    uint8_t powerConfigRegisterValue = PWR_MGMT_1_0;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, I2C_MEMADD_SIZE_8BIT, &powerConfigRegisterValue, 1, MAX_TIMEOUT);

    /* --------------------------- GYRO_CONFIG --------------------------- */
    uint8_t gyroRegisterValue = GYRO_CONFIG_250;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, I2C_MEMADD_SIZE_8BIT, &gyroRegisterValue, 1, MAX_TIMEOUT);

    /* --------------------------- ACCEL_CONFIG --------------------------- */
    uint8_t accelConfigRegisterValue = ACCEL_CONFIG_2g;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, I2C_MEMADD_SIZE_8BIT, &accelConfigRegisterValue, 1, MAX_TIMEOUT);

    /* --------------------------- CONFIG --------------------------- */
    uint8_t configRegisterValue = CONFIG_94Hz;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, CONFIG_REG, I2C_MEMADD_SIZE_8BIT, &configRegisterValue, 1, MAX_TIMEOUT);

    /* --------------------------- INT_ENABLE --------------------------- */
    uint8_t intEnableRegisterValue = INT_ENABLE;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, INT_ENABLE_REG, I2C_MEMADD_SIZE_8BIT, &intEnableRegisterValue, 1, MAX_TIMEOUT);
		
		
    
}


