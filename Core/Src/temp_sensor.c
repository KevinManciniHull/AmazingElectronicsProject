/*
 * temp_sensor.c
 *
 *  Created on: Dec 14, 2021
 *      Author: Inês
 */

#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l4xx_hal_i2c.h"
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include "temp_sensor.h"


/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;


/* Global Variables */

uint8_t WhoAmI_Address = 0x0F;  // Who_Am_I Address
uint8_t TN1218_Temperature_WriteAddress = 0xBE; // Write Address
uint8_t TN1218_Temperature_ReadAddress = 0xBF;  // Read Address

uint8_t value_temperature_level = 23;  // Value the player must reach on the temperature level
uint8_t seconds_temperature_level = 10;  // Seconds the player has to reach the temperature value

float temperature_value = 0;   // Aux variable to the measured temperature value
float current_temperature_value = 0;   // Aux variable to the measured current temperature value


/**
  * @brief  Read Who_Am_I register.
  * @retval Temperature Sensor Device Identifier
  */
 uint8_t TSensor_Who_Am_I(void){

	uint8_t Device_ID;

	HAL_I2C_Master_Transmit(&hi2c2, TN1218_Temperature_WriteAddress, &WhoAmI_Address, 1, 100);
	HAL_I2C_Master_Receive(&hi2c2, TN1218_Temperature_ReadAddress, &Device_ID, 1, 100);
	HAL_UART_Transmit(&huart1, &Device_ID, 1, 100);

	return Device_ID;
 }

 /**
   * @brief  Periodically read the temperature value from the register, checking if the temperature level was passed or failed.
   * @retval 0 if the level was passed
   */
 uint8_t TSensor_Task(void){

	 	int i;  // Aux variable for the cycles
		uint8_t temperature1;  // Integer part of the temperature value
		uint8_t temperature2;  // Fractional part of the temperature value
		float temperature_fraction;  // Aux variable for the preparation of the fractional part of the temperature value

		//Formatted Messages
		char str_temperature[100] = ""; // Formatted message to display the temperature value

		//Messages
		uint8_t message3[] = "***** Temperature Level Initialized ***** \r\n\n ";
		uint8_t message4[] = " Level Completed! :) \r\n ";
		uint8_t message5[] = "Press the Blue Button to Play the Same Level Again \r\n\n ";
		uint8_t message6[] = " Level Failed! :( \r\n ";
		uint8_t message7[] = "Press the Blue Button to Try Again \r\n\n ";

	while(1){
	 for(i=0; i<10; i++) {  // For cycle to count how many times the temperature value is read. This can be translated in seconds (an approximation)

	 	  temperature_value = BSP_TSENSOR_ReadTemp();  // Get the value from the register
	 	  temperature1 = temperature_value;  // Get the integer part of the temperature value
	 	  temperature_fraction = temperature_value - temperature1;  // Prepare the fraction part
	 	  temperature2 = trunc(temperature_fraction * 100);  // Get the fraction part of the temperature value

	 	  snprintf(str_temperature,100," Temperature = %d.%02d\n\r", temperature1, temperature2);  // Print the temperature value
	 	  HAL_UART_Transmit(&huart1,( uint8_t * )str_temperature,sizeof(str_temperature),1000);
	 	  HAL_Delay(1000);  // Wait 1 second

	 	  if(i == seconds_temperature_level - 1) { // If the player has not reached the temperature value during the given time

	 	  HAL_UART_Transmit(&huart1,message6,sizeof(message6),1000);  // Message: level failed

	 	  HAL_UART_Transmit(&huart1,message7,sizeof(message7),1000);  // Message: press the blue button to try again
	 	  while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET);  // Function to press the set button
	 	  while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET);  // Function to press the reset button (good practice)
	 	  }

	 	  if(temperature1 == value_temperature_level) {  // If the player has reached the temperature value in time
	 		  i = seconds_temperature_level - 1 ;

	 		  HAL_UART_Transmit(&huart1,message4,sizeof(message4),1000);  // Message: level passed

	 		  return 0;
	 	  }
	 	}
	}
 }

 /**
   * @brief Read the ADC voltage during approximately 10 seconds, for a future playback of the recorded voice.
   * @retval 0 when the recording is over
   */
uint8_t ReadADC_MCP3002(){

	//Hardware Variables
	uint8_t voltage_ref_ADC = 5;

	//Sound In
	uint8_t k;  // Aux varibled for the cycle
	uint8_t TxData_ADC[2];
	uint8_t RxData_ADC[2];
	uint16_t RxData_ADC_shifted;
	char buf[30];
	float voltage_ADC;

	 // Message
    uint8_t message15[] = " \r\n ***** MCP3002 ADC Voltage  ***** \r\n ";
	 uint8_t message16[] = "Press the Button to Record Voice \r\n\n ";
	 uint8_t message17[] = "Recording is Over \r\n\n ";

	// Construct the message according to the Sound In Hardware and the MCP3002 datasheet
	 TxData_ADC[0] = 0b11;
	 TxData_ADC[0] = ((TxData_ADC[0] << 1) + 0) << 5;
	 TxData_ADC[1] = 0;

	  HAL_UART_Transmit(&huart1,message15,sizeof(message15),1000);  // Message: name of the task

	  HAL_UART_Transmit(&huart1,message16,sizeof(message16),1000);  // Message: press the blue button to start recording
	  while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET);  // Function to press the set button
	  while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET);  // Function to press the reset button (good practice)

	 for(k=0; k<50; k++) { // For cycle to read the ADC voltage 50 times at 200 ms intervals, i.e. for approximately 10 seconds

	 //Get the reply
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);  //Chip Selector (CS): 6 Digital Pin
	 HAL_SPI_TransmitReceive(&hspi1, TxData_ADC, RxData_ADC, 2, 1000);  //Get the data
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);  //Chip Selector (CS): 6 Digital Pin

	 // Combining 2 bytes into 1 byte - Get ADC value by shifting right once and masking out all but last 10 bits
	 RxData_ADC_shifted = ((uint16_t)RxData_ADC[0] << 8) + RxData_ADC[1];
	 RxData_ADC_shifted >>= 1;
	 RxData_ADC_shifted &= 0b0000001111111111;

	 // Calculate the ADC voltage
	 voltage_ADC = (5 * RxData_ADC_shifted) / 1024; //Multiply by 5 because it is the reference voltage and Divided by 1024 because MCP3002 is a 10-bit ADC

	 /* Convert voltage to integer and decimal format */
	 voltage_ADC = voltage_ADC * 100;
	 sprintf(buf, "Voltage = %u.%02u V\r\n",((unsigned int)voltage_ADC / 100),((unsigned int)voltage_ADC % 100)); //Recast the float into two parts (2 integers) and print decimal figure
	 HAL_UART_Transmit(&huart1, (uint8_t *)buf, strlen(buf), 1000);

	 HAL_Delay(200);  // Wait 200 ms
	 }

	 HAL_UART_Transmit(&huart1,message17,sizeof(message17),1000);  // Message: recording is over
	 return 0;
}

/**
  * @brief  Who_Am_I Test
  * @retval -
  */
void TSensor_Who_Am_I_Test(){

	uint8_t Device_ID_aux;

	//Formatted Messages
	char str[100] = ""; // Formatted message to display

	//Debug Messages
	uint8_t message8[] = "\n\n\n\n\n ***** Who_Am_I TEST ***** \r\n ";
	uint8_t message9[] = " ***** Who_Am_I TEST Successfully Performed ***** \r\n ";
	uint8_t message10[] = " ***** ERROR - Who_Am_I TEST ***** \r\n ";


	  /* Who_Am_I TEST*/
	  HAL_UART_Transmit(&huart1,message8,sizeof(message8),1000);  // Message: name of the test
	  Device_ID_aux = TSensor_Who_Am_I();   // Get the Device Identifier

	  if(Device_ID_aux == 188){  //If the device identifier is what it's supposed to be (188)
		  snprintf(str,100,"Device ID = %d\n\r", Device_ID_aux);  //Print the device identifier obtained from the register
		  HAL_UART_Transmit(&huart1,( uint8_t * )str,sizeof(str),1000);
		  HAL_UART_Transmit(&huart1,message9,sizeof(message9),1000);  // Message: test was successful
	  }

	  else{   // If the device identifier is different from 188
		  snprintf(str,100,"Device ID = %d\n\r", Device_ID_aux);  // Print the number obtained from the register
		  HAL_UART_Transmit(&huart1,message10,sizeof(message10),1000);  // Message: test failed
	  }
}

/**
  * @brief  Sensor Initiation and Test
  * @retval -
  */
void TSensor_Initiation_Test(){

	uint8_t Sensor_Initiation_aux;

	//Debug Messages
	uint8_t message1[] = "\n***** Initialize Temperature Sensor and TEST ***** \r\n ";
	uint8_t message2[] = "***** Temperature Sensor Initialized ***** \r\n\n ";
	uint8_t message11[] = "***** ERROR - Sensor Initiation TEST ***** \r\n\n ";

	  /* Sensor Initiation and TEST */
	  HAL_UART_Transmit(&huart1,message1,sizeof(message1),1000);  // Message: name of the test
	  Sensor_Initiation_aux = BSP_TSENSOR_Init();  // Sensor initialisation

	  if(Sensor_Initiation_aux == 0) {  // If the variable returned by the initialization function is 0
	  HAL_UART_Transmit(&huart1,message2,sizeof(message2),1000);  // Message: test was successful
	  }

	  if(Sensor_Initiation_aux == 1) { // If the variable returned by the initialization function is 1
		  HAL_UART_Transmit(&huart1,message11,sizeof(message11),1000);  // Message: test failed
	  }
}

/**
  * @brief  Admitted Current Temperature Value Test
  * @retval -
  */
void TSensor_Admitted_Current_Temperature_Value_Test(){ //Admitted Current Temperature Value TEST

	uint8_t current_temperature1;
	uint8_t current_temperature2;
	float current_temperature_fraction;

	//Formatted Messages
	char str_current_temperature[100] = ""; // Formatted message to display the current temperature value

	//Debug Messages
	uint8_t message12[] = "***** Admitted Current Temperature Value TEST ***** \r\n ";
	uint8_t message13[] = " ***** Admitted Current Temperature Value TEST Successfully Performed ***** \r\n\n ";
	uint8_t message14[] = " ***** ERROR - Admitted Current Temperature Value TEST ***** \r\n\n ";

	  HAL_UART_Transmit(&huart1,message12,sizeof(message12),1000);  // Message: name of the test

	  current_temperature_value = BSP_TSENSOR_ReadTemp();  // Get the value from the register
	  current_temperature1 = current_temperature_value;  // Get the integer part of the temperature value
	  current_temperature_fraction = current_temperature_value - current_temperature1;   // Prepare the fraction part
	  current_temperature2 = trunc(current_temperature_fraction * 100);    // Get the fraction part of the temperature value

	  snprintf(str_current_temperature,100," Current Temperature = %d.%02d\n\r", current_temperature1, current_temperature2);  // Print the current temperature value
	  HAL_UART_Transmit(&huart1,( uint8_t * )str_current_temperature,sizeof(str_current_temperature),1000);

	  if(current_temperature1>-40 && current_temperature1<120){  // If the temperature value is within an admissible range
		  HAL_UART_Transmit(&huart1,message13,sizeof(message13),1000);  // Message: test was successful
	  }

	  else{  // If the temperature value is not within an admissible range
		  HAL_UART_Transmit(&huart1,message14,sizeof(message14),1000);  // Message: test failed
	  }

}
