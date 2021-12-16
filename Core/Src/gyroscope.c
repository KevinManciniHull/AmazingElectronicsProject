/*
 * gyroscope.c
 *
 *  Created on: Dec 6, 2021
 *      Author: kevinmancini
 */

#define GYRO_NUMBER_OF_ATTEMPTS 10
#define GYRO_NUMBER_OF_TASKS 6
#define GYRO_WHO_AM_I_ADDRESS 0x0F
#define GYRO_OUT_TEMP_L_ADDRESS 0x20
#define GYRO_OUT_TEMP_H_ADDRESS 0x21
#define GYRO_MAX_TEMP 85
#define GYRO_MIN_TEMP -30
#define GYRO_OUT_TEMP_H_ADDRESS 0x21
#define GYRO_WHO_AM_I_VALUE 0x6A
#define GYRO_WRITE_ADDRESS 0xD5
#define GYRO_READ_ADDRESS 0xD4
#define GYRO_TEST_OK 0
#define GYRO_TEST_ERROR 1
#define GYRO_X_MAX 40000
#define GYRO_X_MIN -40000
#define GYRO_Y_MAX 40000
#define GYRO_Y_MIN -40000
#define GYRO_Z_MAX 40000
#define GYRO_Z_MIN -40000



#include "gyroscope.h"
#include "lsm6dsl.h"
#include "st7789.h"
#include "fonts.h"
#include "stm32l475e_iot01_gyro.h"


uint8_t GYRO_WhoAmI_Address = 0x0F;
uint8_t GYRO_OutTempL_Address = 0x20;
uint8_t GYRO_OutTempH_Address = 0x21;
I2C_HandleTypeDef GYRO_hi2c2;
UART_HandleTypeDef huart1;
int16_t GYRO_current_temp;

enum state {
	TEST_STATE,
	OUT_OF_SERVICE_STATE,
	IDLE_STATE,
	TASK_STATE };
typedef enum state state_t;

/**
  * @brief  Compare the user's values  with the criteria of the task
  * @param  n indicates which criteria should be used, pfData user's values
  * @retval 1 CORRECT
  */
uint8_t GYRO_Compare_Values(uint8_t n, float *pfData){
	if (n==0){
		// X check
		return ( pfData[0]<GYRO_X_MIN || pfData[0]>GYRO_X_MAX ) && pfData[1]>GYRO_Y_MIN && pfData[1]<GYRO_X_MAX && pfData[2]>GYRO_X_MIN && pfData[2]<GYRO_X_MAX;
	} else if (n==1){
		// Y check
		return pfData[0]>GYRO_X_MIN && pfData[0]<GYRO_X_MAX && ( pfData[1]<GYRO_Y_MIN || pfData[1]>GYRO_X_MAX ) && pfData[2]>GYRO_X_MIN && pfData[2]<GYRO_X_MAX;
	} else if (n==2){
		// Z check
		return pfData[0]>GYRO_X_MIN && pfData[0]<GYRO_X_MAX && pfData[1]>GYRO_Y_MIN && pfData[1]<GYRO_X_MAX && ( pfData[2]<GYRO_X_MIN || pfData[2]>GYRO_X_MAX );
	} else {
		return 1;
	}
}

/**
  * @brief  Get the value of the WHO_AM_I register
  * @param  NONE
  * @retval WHO_AM_I register's value
  */
uint16_t GYRO_Get_Temp(void){
	uint8_t cur_temp_l;
	uint8_t cur_temp_h;

	HAL_I2C_Master_Transmit(&GYRO_hi2c2, GYRO_WRITE_ADDRESS, &GYRO_OutTempL_Address, 1, 100);
	HAL_I2C_Master_Receive(&GYRO_hi2c2, GYRO_READ_ADDRESS, &cur_temp_l, 1, 100);

	HAL_I2C_Master_Transmit(&GYRO_hi2c2, GYRO_WRITE_ADDRESS, &GYRO_OutTempL_Address, 1, 100);
	HAL_I2C_Master_Receive(&GYRO_hi2c2, GYRO_READ_ADDRESS, &cur_temp_h, 1, 100);

	return ((cur_temp_h << 8) | cur_temp_l);
}

/**
  * @brief  Check if the current temperature is between the admitted boundaries
  * @retval 1 if correct temperature, otherwise 0
  */
uint8_t GYRO_Check_Temp(void){
	GYRO_current_temp = GYRO_Get_Temp();
	return GYRO_current_temp>GYRO_MIN_TEMP && GYRO_current_temp<GYRO_MAX_TEMP;
}

/**
  * @brief  Print on the terminal messages, used for debug only
  * @param  char array of the message
  */
void GYRO_PrintTerminal(char* msg){
	HAL_UART_Transmit(&huart1,( uint8_t * )msg,sizeof(msg),1000);
}

/**
  * @brief  Run the gyroscope task
  * @retval 0 if PASSED, otherwise 1 FAILED
  */
uint8_t GYRO_Task(void){
	// START TASK
	char points0[] = "X X X X X ";
	char points1[] = "O X X X X ";
	char points2[] = "O O X X X ";
	char points3[] = "O O O X X ";
	char points4[] = "O O O O X ";
	char points5[] = "O O O O O ";
	// Init
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	ST7789_Init();
	uint8_t GYRO_init = BSP_GYRO_Init();
	float *pfData = malloc(3*sizeof(float));
	float *gyro_value = malloc(3*sizeof(float));
	HAL_Delay(1000);
	BSP_GYRO_GetXYZ(gyro_value);
	int p = 0;
	ST7789_Fill_Color(WHITE);
	if (GYRO_init == 0)
	while(1){
		// MINI-TASK
		for(int n = 0; n<3; n++){
			if(n==0)
				ST7789_WriteString(10, 50, "Tilt left and right quickly for 5 seconds", Font_16x26, 100, WHITE);
			else if(n==1)
				ST7789_WriteString(10, 50, "Tilt front and back quickly for 5 seconds", Font_16x26, 100, WHITE);
			else if(n==2)
				ST7789_WriteString(10, 50, "Tilt clockwise and opposite quickly for 5 seconds", Font_16x26, 100, WHITE);
			while(p<5){
				if (p==0)
					ST7789_WriteString(10, 190, points0, Font_11x18, 100, WHITE);
				if (p==1)
					ST7789_WriteString(10, 190, points1, Font_11x18, 100, WHITE);
				if (p==2)
					ST7789_WriteString(10, 190, points2, Font_11x18, 100, WHITE);
				if (p==3)
					ST7789_WriteString(10, 190, points3, Font_11x18, 100, WHITE);
				if (p==4)
					ST7789_WriteString(10, 190, points4, Font_11x18, 100, WHITE);
				if (p==5)
					ST7789_WriteString(10, 190, points5, Font_11x18, 100, WHITE);
				HAL_Delay(1000);
				//if (GYRO_Check_Temp())
					BSP_GYRO_GetXYZ(pfData);
				if (GYRO_Compare_Values(n,pfData))
					p++;
			}
			ST7789_Fill_Color(WHITE);
			ST7789_WriteString(10, 160, "DONE", Font_16x26, 100, WHITE);
			HAL_Delay(1000);
			ST7789_Fill_Color(WHITE);
			p = 0;
		}
		ST7789_Fill_Color(WHITE);
		ST7789_WriteString(10, 160, "COMPLIMENTS", Font_16x26, RED, WHITE);
		HAL_Delay(1000);
		ST7789_Fill_Color(WHITE);
		return 0;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	return 1;
}


/**
  * @brief  Get the value of the WHO_AM_I register
  * @param  NONE
  * @retval WHO_AM_I register's value
  */


/**
  * @brief  Get the value of the WHO_AM_I register of the gyroscope sensor
  * @retval WHO_AM_I register's value
  */
uint8_t GYRO_Get_WHOAMI(void){
	uint8_t ID;
	HAL_I2C_Master_Transmit(&GYRO_hi2c2, GYRO_WRITE_ADDRESS, &GYRO_WhoAmI_Address, 1, 100);
	HAL_I2C_Master_Receive(&GYRO_hi2c2, GYRO_READ_ADDRESS, &ID, 1, 100);
	return ID;
}



/* TEST: Gyroscope WHOAMI check
 * RETURN: TEST_OK ( 0 ) or TEST_ERROR ( 1 )
 * */
uint8_t WHO_AM_I_TEST(void){
	return GYRO_Get_WHOAMI()==GYRO_WHO_AM_I_VALUE?GYRO_TEST_OK:GYRO_TEST_ERROR;
}

/* TEST: Gyroscope Initialization test
 * RETURN: TEST_OK ( 0 ) or TEST_ERROR ( 1 )
 * */
uint8_t GYRO_INIT_TEST(void){
	return BSP_GYRO_Init();
}

/* TEST: READ-WRITE registers test
 * RETURN: TEST_OK ( 0 ) or TEST_ERROR ( 1 )
 * */
//uint8_t READ_WRITE_TEST(void){
//	uint8_t res1,res2,res3;
//
//	HAL_I2C_Master_Transmit(&hi2c2, GYRO_WRITE_ADDRESS, &Test_Address1, 1, 100);
//	HAL_I2C_Master_Receive(&hi2c2, GYRO_READ_ADDRESS, &res1, 1, 100);
//
//	HAL_I2C_Master_Transmit(&hi2c2, GYRO_WRITE_ADDRESS, &Test_Address2, 1, 100);
//	HAL_I2C_Master_Receive(&hi2c2, GYRO_READ_ADDRESS, &res2, 1, 100);
//
//	HAL_I2C_Master_Transmit(&hi2c2, GYRO_WRITE_ADDRESS, &Test_Address3, 1, 100);
//	HAL_I2C_Master_Receive(&hi2c2, GYRO_READ_ADDRESS, &res3, 1, 100);
//	return ( res1==GYRO_TEST_EXPECTED_1 && res2==GYRO_TEST_EXPECTED_2 && res3==GYRO_TEST_EXPECTED_3 )?TEST_OK:TEST_ERROR;
//}

/* TEST: Task test
 * RETURN: TEST_OK ( 0 ) or TEST_ERROR ( 1 )
 * */
uint8_t GYRO_TASK_TEST(void){
	uint8_t res = 0;

	float test_pfData_1[] = {0.0,0.0,0.0};
	float test_pfData_2[] = {40001.0,0.0,0.0};
	float test_pfData_3[] = {0.0,40001.0,0.0};
	float test_pfData_4[] = {0.0,0.0,40001.0};
	float test_pfData_5[] = {40001.0,40001.0,0.0};
	float test_pfData_6[] = {0.0,40001.0,40001.0};
	float test_pfData_7[] = {40001.0,0.0,40001.0};
	float test_pfData_8[] = {40001.0,40001.0,40001.0};
	float test_pfData_9[] = {-40001.0,0.0,0.0};
	float test_pfData_10[] = {0.0,-40001.0,0.0};
	float test_pfData_11[] = {0.0,0.0,-40001.0};
	float test_pfData_12[] = {-40001.0,-40001.0,0.0};
	float test_pfData_13[] = {0.0,-40001.0,-40001.0};
	float test_pfData_14[] = {-40001.0,0.0,-40001.0};
	float test_pfData_15[] = {-40001.0,-40001.0,-40001.0};

	res += GYRO_Compare_Values(1,test_pfData_1);
	res += !GYRO_Compare_Values(1,test_pfData_2);
	res += GYRO_Compare_Values(1,test_pfData_3);
	res += GYRO_Compare_Values(1,test_pfData_4);
	res += GYRO_Compare_Values(1,test_pfData_5);
	res += GYRO_Compare_Values(1,test_pfData_6);
	res += GYRO_Compare_Values(1,test_pfData_7);
	res += GYRO_Compare_Values(1,test_pfData_8);
	res += !GYRO_Compare_Values(1,test_pfData_9);
	res += GYRO_Compare_Values(1,test_pfData_10);
	res += GYRO_Compare_Values(1,test_pfData_11);
	res += GYRO_Compare_Values(1,test_pfData_12);
	res += GYRO_Compare_Values(1,test_pfData_13);
	res += GYRO_Compare_Values(1,test_pfData_14);
	res += GYRO_Compare_Values(1,test_pfData_15);

	res += GYRO_Compare_Values(2,test_pfData_1);
	res += GYRO_Compare_Values(2,test_pfData_2);
	res += !GYRO_Compare_Values(2,test_pfData_3);
	res += GYRO_Compare_Values(2,test_pfData_4);
	res += GYRO_Compare_Values(2,test_pfData_5);
	res += GYRO_Compare_Values(2,test_pfData_6);
	res += GYRO_Compare_Values(2,test_pfData_7);
	res += GYRO_Compare_Values(2,test_pfData_8);
	res += GYRO_Compare_Values(2,test_pfData_9);
	res += !GYRO_Compare_Values(2,test_pfData_10);
	res += GYRO_Compare_Values(2,test_pfData_11);
	res += GYRO_Compare_Values(2,test_pfData_12);
	res += GYRO_Compare_Values(2,test_pfData_13);
	res += GYRO_Compare_Values(2,test_pfData_14);
	res += GYRO_Compare_Values(2,test_pfData_15);

	res += GYRO_Compare_Values(3,test_pfData_1);
	res += GYRO_Compare_Values(3,test_pfData_2);
	res += GYRO_Compare_Values(3,test_pfData_3);
	res += !GYRO_Compare_Values(3,test_pfData_4);
	res += GYRO_Compare_Values(3,test_pfData_5);
	res += GYRO_Compare_Values(3,test_pfData_6);
	res += GYRO_Compare_Values(3,test_pfData_7);
	res += GYRO_Compare_Values(3,test_pfData_8);
	res += GYRO_Compare_Values(3,test_pfData_9);
	res += GYRO_Compare_Values(3,test_pfData_10);
	res += GYRO_Compare_Values(3,test_pfData_11);
	res += GYRO_Compare_Values(3,test_pfData_12);
	res += GYRO_Compare_Values(3,test_pfData_13);
	res += GYRO_Compare_Values(3,test_pfData_14);
	res += GYRO_Compare_Values(3,test_pfData_15);

	return res;
}

