/* USER CODE BEGIN Header */
/** 
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program for the ELE3312 project
	* Authors         :
	* Group           :
	*
  ******************************************************************************
  * Project: Projet Final_Version Final
	* Partie : 1
  * Author1: Moira Lamarre 2070819
	* Author2: Kamer Ammour 2061731
  * Groupe: 5
	*
	* Partie : 2
	* Author3: Rina Aladad 2185593
	* Author4:Mahdi Moussa 2176885
  * Groupe: 5
  * Date: 2024-11-27
  * This file contains the main logic of the game.
  * 
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "ili9341_gfx.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "Graphics.h"
#include "Menu.h"
#include "Game.h"
#include "Battle.h"
#include "stdio.h"
#include "Accelerometre.h"
#include "Communication.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


#define TABLE_LENGTH 1000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ili9341_t *_screen;
player_t players[NUM_PLAYERS] = {0};
uint8_t gameFlag = 0;
uint8_t RxDatsa[1];

uint32_t tab_value0[TABLE_LENGTH];
uint32_t * pointer_tab0 = tab_value0; // Initialise un pointeur vers le tableau 1
uint8_t data[6];
int16_t accelY;
int16_t accelX;


float temps_min = 116.0;
float temps_max = 3480.0;
float v_son = 0.000343;
volatile float temps_echo = 0.0;
volatile float echo_start = 0.0;
volatile float echo_end = 0.0;

volatile int etat_rising_Egde=0;
volatile int in_bounds = 0;
volatile int position_current = 0;

//Partie 2 initiation signal carée, etc:


uint32_t tab_value[TABLE_LENGTH];
//uint32_t tab_value2[TABLE_LENGTH];
const float fs = 40000.0;
volatile float frequence;
const float M_PI = 3.14159265;
const float distance_un_carre = (60.0-2.0)/15.0;
const float distance_min = 2.0;
volatile float distance;
float freq[15] = {246.94,261.63,293.66,311.13,329.63,369.99,392.00,440,493.88,523.25,587.33,622.25,659.26,698.46,783.99};
float distance_carre[16] = 
{distance_min,
1.0*distance_un_carre+distance_min,// distance maximale de la premiere frequence freq[0]
2.0*distance_un_carre+distance_min,
3.0*distance_un_carre+distance_min,
4.0*distance_un_carre+distance_min,
5.0*distance_un_carre+distance_min,
6.0*distance_un_carre+distance_min,
7.0*distance_un_carre+distance_min, //  distance maximale de la 7eme frequence freq[6]
8.0*distance_un_carre+distance_min, // distance maximale a 440Hz
9.0*distance_un_carre+distance_min,
10.0*distance_un_carre+distance_min,
11.0*distance_un_carre+distance_min,
12.0*distance_un_carre+distance_min,
13.0*distance_un_carre+distance_min,
14.0*distance_un_carre+distance_min,
15.0*distance_un_carre+distance_min};// distance maximale de la derniere frequence freq[14]


const int nb_pixel_un_carre = 240/15;
int nb_pixel_y [16]=
{0,
1*nb_pixel_un_carre,// derniere position du premier cube freq[0]
2*nb_pixel_un_carre,
3*nb_pixel_un_carre,
4*nb_pixel_un_carre,
5*nb_pixel_un_carre,
6*nb_pixel_un_carre,
7*nb_pixel_un_carre,
8*nb_pixel_un_carre, // derniere position du 8eme cube freq[7]
9*nb_pixel_un_carre,
10*nb_pixel_un_carre,
11*nb_pixel_un_carre,
12*nb_pixel_un_carre,
13*nb_pixel_un_carre,
14*nb_pixel_un_carre,
15*nb_pixel_un_carre};// derniere position du dernier cube freq[14]
ili9341_t *_screen;
extern volatile int counter;
extern volatile int nb_etoiles;
extern volatile int position_x[10];
extern volatile int position_y[10];
extern volatile int z;
extern volatile int pouvoir_Pac; 
extern volatile int pouvoir_Ghost;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        if (etat_rising_Egde==0) {
            echo_start = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
					etat_rising_Egde=1;
        } else if(etat_rising_Egde==1) {
            echo_end = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
					etat_rising_Egde=0;
					temps_echo=fabsf(echo_end-echo_start);
					
					if(temps_echo>=temps_min && temps_echo<= temps_max)
					{
					in_bounds = 1;
					}else{in_bounds = 0;}
                 }
				
    }
}

void fill_table_onde_carre(uint32_t *tableau, float frequence) {

int periode = fs/frequence;

		for (int i = 0; i < TABLE_LENGTH; i++) {
			tableau[i] = (uint32_t)((sinf(2.0*M_PI*frequence*i/fs)+1)*2000.00);  
 
}
		}
void partie_B(float v_son, float temps_echo, int in_bounds,ili9341_t *_screen,float *distance_carre,float *freq,float frequence,uint32_t *tab_value,int *nb_pixel_y ){
	    /* USER CODE END WHILE */
	
	 /* USER CODE END WHILE */
 distance = ((v_son *temps_echo)/2.0)*100.0;
	if(distance>=2&& distance<=60)
	{
		distance = distance;
	}else if (distance>=60)
	{
	distance = 60;
	} else if (distance <=2.8)
	{
	distance = 2;
	}

if(distance<=distance_carre[1] && distance>=distance_carre[0]){frequence = freq[0];
fill_table_onde_carre(tab_value, frequence);// premiere frequence
	ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
	position_current = nb_pixel_y [0];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [0], nb_pixel_un_carre, nb_pixel_un_carre);
}

if(distance<=distance_carre[2] && distance>=distance_carre[1]){frequence = freq[1];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [1];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [1], nb_pixel_un_carre, nb_pixel_un_carre);
}

if(distance<=distance_carre[3] && distance>=distance_carre[2]){frequence = freq[2];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [2];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [2], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[4] && distance>=distance_carre[3]){frequence = freq[3];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [3];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [3], nb_pixel_un_carre, nb_pixel_un_carre);
}
	
if(distance<=distance_carre[5] && distance>=distance_carre[4]){frequence = freq[4];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [4];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [4], nb_pixel_un_carre, nb_pixel_un_carre);
}

if(distance<=distance_carre[6] && distance>=distance_carre[5]){frequence = freq[5];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [5];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [5], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[7] && distance>=distance_carre[6]){frequence = freq[6];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [6];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [6], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[8] && distance>=distance_carre[7]){frequence = freq[7];
fill_table_onde_carre(tab_value, frequence);//440 HZ
	ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [7];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [7], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[9] && distance>=distance_carre[8]){frequence = freq[8];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [8];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [8], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[10] && distance>=distance_carre[9]){frequence = freq[9];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [9];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [9], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[11] && distance>=distance_carre[10]){frequence = freq[10];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [10];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [10], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[12] && distance>=distance_carre[11]){frequence = freq[11];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [11];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [11], nb_pixel_un_carre, nb_pixel_un_carre);
}
	
	
if(distance<=distance_carre[13] && distance>=distance_carre[12]){frequence = freq[12];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [12];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [12], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[14] && distance>=distance_carre[13]){frequence = freq[13];
fill_table_onde_carre(tab_value, frequence);
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [13];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [13], nb_pixel_un_carre, nb_pixel_un_carre);
}

	
if(distance<=distance_carre[15] && distance>=distance_carre[14]){frequence = freq[14];
fill_table_onde_carre(tab_value, frequence);// derniere frequence
ili9341_fill_rect(_screen, ILI9341_BLACK, 272, 0, 16, 240);
		position_current = nb_pixel_y [14];
	ili9341_fill_rect(_screen, ILI9341_CYAN, 272,nb_pixel_y [14], nb_pixel_un_carre, nb_pixel_un_carre);
}

frequence = 0.0;
	temps_echo = 0.0;
	distance = 0.0;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	/* Default players setting */
	static game_state_t game_state = CHOOSE_PLAYER;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */


	// Initialize the screen
	_screen = ili9341_new(
		  &hspi1,
		  Void_Display_Reset_GPIO_Port, Void_Display_Reset_Pin,
		  TFT_CS_GPIO_Port, TFT_CS_Pin,
		  TFT_DC_GPIO_Port, TFT_DC_Pin,
		  isoLandscapeFlip,
		  NULL, NULL,
		  NULL, NULL,
		  itsNotSupported,
		  itnNormalized);
	char text[40];
	ili9341_text_attr_t text_attr = {&ili9341_font_11x18,  ILI9341_WHITE, ILI9341_BLACK, 10, 0};
	ili9341_fill_screen(_screen, ILI9341_BLACK);
	ili9341_text_attr_t text_attr2 = {&ili9341_font_11x18, ILI9341_WHITE, ILI9341_BLACK,0,18};
	HAL_TIM_Base_Start_IT(&htim2); 
	HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim5); 
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, tab_value, TABLE_LENGTH, DAC_ALIGN_12B_R);

		player_t* player = &players[LOCAL_PLAYER_ID];
	player_t* enemy = &players[ENEMY_PLAYER_ID];
	


	int16_t x = 0, y = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_Delay(20); // � remplacer avec un timer
		
		switch(game_state) {
		case CHOOSE_PLAYER:
      choosePlayer(_screen, players);
			game_state = INIT_MAZE;
			break;
			break;
		
		case INIT_MAZE:
				counter = 0;
				nb_etoiles = 0;
				z = 0;
				pouvoir_Pac = 0; 
		  	player->current_pos = player->start_pos;
		  	enemy->current_pos = enemy->start_pos;
		  	drawMaze(_screen, players);			
		  	game_state = WANDER_MAZE;

			break;
			
		case WANDER_MAZE:

		//player
		if (Data_flag != 0) {
					See_Acceleration(); 
					Data_flag = 0;
			}
			x = player->current_pos.x + nx * STEP_SIZE;
			y = player->current_pos.y + ny * STEP_SIZE;
		
		
			
 if (updatePosition(_screen, (position_t){x, y}, players)) {
        game_state = BATTLE; 
        continue;
    }

    // Communicate 
    uint8_t message[4] = {
        (uint8_t)(player->current_pos.x >> 8),  
        (uint8_t)(player->current_pos.x & 0xFF),
        (uint8_t)(player->current_pos.y >> 8),  
        (uint8_t)(player->current_pos.y & 0xFF) 
    };
    UART_SendMessage(&huart5, message, sizeof(message)); 

    // Check for position updates from the other board
    if (flag_message_complete) {
        flag_message_complete = 0;

        //Remote player's position
        enemy->previous_pos = enemy->current_pos;
        enemy->current_pos.x = (message_buffer[0] << 8) | message_buffer[1];
        enemy->current_pos.y = (message_buffer[2] << 8) | message_buffer[3];

        // Draw the remote player at the updated position
        drawRemotePlayer(_screen, enemy);
    }

  	if(updatePosition(_screen, (position_t){x, y}, players)){
				game_state = BATTLE;
			continue;
		}

			break;
			
		case BATTLE:
			HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, tab_value, TABLE_LENGTH, DAC_ALIGN_12B_R);
			battle(_screen, players);
			game_state = INIT_MAZE;
			break;
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac) {

            HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, tab_value, TABLE_LENGTH, DAC_ALIGN_12B_R);
	
 }

void HAL_UART_RxCpltCallbacks(UART_HandleTypeDef *huart) {
    if (huart == &huart5) { // Check if the callback is for UART5
        UART_ProcessByte(RxDatsa[0]); // Process received byte
        HAL_UART_Receive_IT(&huart5, RxDatsa, 1); // Re-enable interrupt for the next byte
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
