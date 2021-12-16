/*
 * temp_sensor.h
 *
 *  Created on: Dec 14, 2021
 *      Author: Inês
 */

#include <stdlib.h>
#include <stdio.h>

 uint8_t TSensor_Task(void);  //Temperature Sensor: Task (temperature level)
 uint8_t ReadADC_MCP3002(void);  //MCP3002 ADC: Task (record voice)

 void TSensor_Who_Am_I_Test(void);  //Temperature Sensor: Test 1
 void TSensor_Initiation_Test(void);  //Temperature Sensor: Test 2
 void TSensor_Admitted_Current_Temperature_Value_Test(void);  //Temperature Sensor: Test 3
