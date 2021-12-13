/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "ACC_VARIABLES.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;

DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;

I2C_HandleTypeDef hi2c2;

QSPI_HandleTypeDef hqspi;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;
TIM_HandleTypeDef htim16;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
//MCP23s09 expander GPIO;



/*#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')
*/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DFSDM1_Init(void);
static void MX_I2C2_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM16_Init(void);
void writevalue (void);
void readregisters(void);
void twoscomplement (void);
void initialize_extender(void);
void led_colours(char led_colour[]);
void mainmenu(void);
void sendDataSPI(uint8_t reg, uint8_t value);
void led_config(int i);
void acc_reg(void);
void correctID (void);
void play_acc(void);
void youlost(void);
void youwon(void);
int seconds=5000;



char x_axis_charl[10];
char x_axis_char2[10];
char x_axis_char;
//  char TEST_LOGIN = 'T';
char ACC_LOGIN = 'A';
uint8_t led_counter=0x80;




/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


void correctID (void)
{
uint8_t WHO_AM_I = 0x0F;
uint8_t data3;
HAL_UART_Transmit(&huart1,"\033[2J\033[1;1H", strlen("\033[2J\033[1;1H"), 100);
HAL_I2C_Master_Transmit(&hi2c2, 0xD5, &WHO_AM_I, 1,100);
HAL_I2C_Master_Receive(&hi2c2, 0xD4, &data3,1, 100);
if (data3==106)
{
	sprintf(msg_acc,"it is an accelerometer %d",data3);
  HAL_UART_Transmit(&huart1, msg_acc, strlen(msg_acc), 1000);
}
else if (data3!=106)
{
	sprintf(msg_acc,"it is NOT an accelerometer %d",3);
HAL_UART_Transmit(&huart1, msg_acc, strlen(msg_acc), 1000);
}
}



void readregisters(void)
{
HAL_I2C_Mem_Read(&hi2c2, 0xD4, x_axis_l, 1, &dataxl, 1, 100);
HAL_I2C_Mem_Read(&hi2c2, 0xD4, x_axis_h, 1, &dataxh, 1, 100);
HAL_I2C_Mem_Read(&hi2c2, 0xD4, y_axis_l, 1, &datayl, 1, 100);
HAL_I2C_Mem_Read(&hi2c2, 0xD4, y_axis_h, 1, &datayh, 1, 100);
HAL_I2C_Mem_Read(&hi2c2, 0xD4, z_axis_l, 1, &datazl, 1, 100);
HAL_I2C_Mem_Read(&hi2c2, 0xD4, z_axis_h, 1, &datazh, 1, 100);
}


void acc_reg(void){
	  HAL_I2C_Mem_Write(&hi2c2, 0xD5, b_acc, 1, &a_acc, 1, 100);
	  readregisters();
	  twoscomplement();
	  writevalue();
}

void twoscomplement (void){
proba1=(int16_t)(dataxh<<8 | dataxl)*0.061;
proba2=(int16_t)(datayh<<8 | datayl)*0.061;
proba3=(int16_t)(datazh<<8 | datazl)*0.061;
}

void writevalue(void)
{
sprintf(msg_acc, "x, y, z acceleration %d %d %d           ", proba1, proba2, proba3);


}

void initialize_extender(void){

      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); // Take slave-select low
      HAL_SPI_Transmit(&hspi1,(uint8_t *)&OPCODER,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the MCP23S09 opcode, and read bit
      HAL_SPI_Transmit(&hspi1,(uint8_t *)&GPPU,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the MCP23S09 opcode, and read bit
            HAL_SPI_Receive(&hspi1,(uint8_t *)&leds_binary,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the register we want to write
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

}

void led_config(int i){
int t=1;
if (i==0){
	sides_acc=(int)proba1;
}
else if (i==1){
	sides_acc=(int)proba2;
}
else if (i==2){
	sides_acc=(int)proba3;
}



	mainmenuloop=1;
	sprintf(msg_acc,"\rlet's play a guessing game\n\rtilt the board's"
			" side by 45 degrees\n\ryou have %d ms to do it      \n\r",seconds);
	HAL_UART_Transmit(&huart1,msg_acc,strlen(msg_acc), 100);
	seconds=seconds-200;
	sendDataSPI(GPPU, led_counter);
	led_counter=led_counter/2;
	if (led_counter<0x01){
		led_counter=0x80;
	}
	play_acc();
	HAL_UART_Receive (&huart1, UART1_rxBuffer, 12, 100);
HAL_UART_Transmit(&huart1,"\033[F\033[F\033[F",strlen("\033[F\033[F\033[F"),100);
if (UART1_rxBuffer[0]=='t'){
	   UART1_rxBuffer[0]="";
	   seconds=5000;
	   main_acc();
}

else if (seconds==0 && (450>sides_acc || sides_acc>550)){
		  youlost();
}

else if (seconds==0 && (450<sides_acc && sides_acc<550)){

youwon();



	   }
}




void youlost(void){
	int t=1;
	UART1_rxBuffer[0]="";
			   seconds=5000;
			   HAL_UART_Transmit(&huart1,"\033[F"
			  	     	"╔═════════════════╗\n\r"
			  		    "║    YOU LOST     ║\n\r"
			  		    "╚═════════════════╝\n\r",
			  			strlen("\033[F"
			  			"╔═════════════════╗\n\r"
			  		    "║    YOU LOST     ║\n\r"
			  		    "╚═════════════════╝\n\r"),200);

while (t==1){
	HAL_UART_Receive (&huart1, UART1_rxBuffer, 12, 100);
			if 	(UART1_rxBuffer[0]=='c'){
				   UART1_rxBuffer[0]="";
				t=0;
			main_acc();
			}
	sendDataSPI(GPPU, 0xFF);
	HAL_Delay(500);
	sendDataSPI(GPPU, 0x00);
	HAL_Delay(500);



}
}



void youwon(void){
	int t=1;
	UART1_rxBuffer[0]="";
			   seconds=5000;
			   		   HAL_UART_Transmit(&huart1,"\033[F"
			   		  	     	"╔═════════════════╗\n\r"
			   		  		    "║    YOU WON!     ║\n\r"
			   		  		    "╚═════════════════╝\n\r",
			   		  			strlen("\033[F"
			   		  			"╔═════════════════╗\n\r"
			   		  		    "║    YOU WON!     ║\n\r"
			   		  		    "╚═════════════════╝\n\r"),100);

			   		while (t==1){

			   					   HAL_UART_Receive (&huart1, UART1_rxBuffer, 12, 100);
			   					if 	(UART1_rxBuffer[0]=='c'){
			   						   UART1_rxBuffer[0]="";
			   						t=0;
			   					main_acc();
			   					}
			   					sendDataSPI(GPPU, 0xFF);
			   					HAL_Delay(50);
			   					sendDataSPI(GPPU, 0x00);
			   					HAL_Delay(50);

			   		}
}




void mainmenu(void){
	int i = rand ()%(2-0+1);


HAL_UART_Transmit(&huart1,"\033[2J\033[1;1H", strlen("\033[2J\033[1;1H"), 100);
initialize_extender();

HAL_UART_Transmit(&huart1, msg_acc, strlen(msg_acc), 100);
  HAL_UART_Transmit(&huart1,"\n\rchecking accelerator. press c to play a game\n\r(press t to sTop the current game)\n\n\n\r",
    strlen("\n\rchecking accelerator. press c to play a game\n\r(press t to sTop the current game)\n\n\n\r"),100);
while (mainmenuloop){
   HAL_UART_Receive (&huart1, UART1_rxBuffer, 12, 100);
if (UART1_rxBuffer[0]=='c'){
mainmenuloop=0;
led_config(i);
}

}
}


void sendDataSPI(uint8_t reg, uint8_t value){
  uint8_t buf;
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); // Take slave-select low
            HAL_SPI_Transmit(&hspi1,(uint8_t *)&OPCODEW,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the MCP23S09 opcode, and write bit
            HAL_SPI_Transmit(&hspi1,(uint8_t *)&reg,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the register we want to write
            HAL_SPI_Transmit(&hspi1,(uint8_t *)&value,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the byte
      //      HAL_SPI_Receive(&hspi1,(uint8_t *)&OPCODER,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the register we want to write
        //    HAL_SPI_Receive(&hspi1,(uint8_t *)&buf,sizeof(uint8_t),SPI_TRANSFER_TIMEOUT); // Send the register we want to write
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); // Take slave-select high
    }
/**
  * @brief System Clock Configuration
  * @retval None
  */
void play_acc(void){
acc_reg();
   HAL_UART_Receive (&huart1, UART1_rxBuffer, 12, 100);
   HAL_UART_Transmit(&huart1, msg_acc, strlen(msg_acc), 100);

}

void led_colours(char colour[]){

//int i=strlen(colour)+strlen("\rrgb test %s\n\r%s          \033[F")+strlen(msg_acc);
  //char message[i];
// HAL_UART_Transmit(&huart1, "\33[2K\r", strlen("\33[2K\r"), 1);
  //initialize_extender();

//sprintf(message, "\rrgb test %s\n\r%s          \033[F",colour, msg_acc);
//HAL_UART_Transmit(&huart1, message, strlen(message), 200);

//memset(UART1_rxBuffer,0,strlen(UART1_rxBuffer));
}
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 80-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65535;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}
