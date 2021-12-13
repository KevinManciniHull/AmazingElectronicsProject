 /*
 * ACC_VARIABLES.h
 *
 *  Created on: Dec 12, 2021
 *      Author: Bernardas
 */

#ifndef INC_ACC_VARIABLES_H_
#define INC_ACC_VARIABLES_H_

extern uint8_t IODIR;
#define IPOL    0x01
#define GPINTEN 0x02
#define DEFVAL    0x03
#define INTCON    0x04
#define IOCON   0x05
extern uint8_t GPPU;
#define INTF    0x07
extern uint8_t INTCAP;
extern uint8_t GPIO;
extern uint8_t   OPCODEW;
extern uint8_t    OPCODER;
extern char UART1_rxBuffer[12];
#define SPI_TRANSFER_TIMEOUT    1000
extern int16_t proba2;//variable for two's complement function conversion, y;
extern char msg[50];
extern int accel_loop;
extern int mainmenuloop;
extern uint8_t leds_binary;
extern char msg_acc[35];
extern int global_variable;
int sides_acc;
extern uint8_t templsb;//temp register low
extern uint8_t tempmsb;//temp register high
extern uint8_t templ;//buffer for lsb temperature reading
extern uint8_t temph;//buffer for msb temperature reading
extern int16_t temp;//temp register value after twos complement
extern uint8_t x_axis_l;//out x l axis
extern uint8_t x_axis_h;// out x h axis
extern uint8_t y_axis_l;//out y l axis
extern uint8_t y_axis_h;// out y h axis
extern uint8_t z_axis_l;//out z l axis
extern uint8_t z_axis_h;// out z h axis
extern uint8_t WHO_AM_I;//who am i
extern uint8_t a_acc;//low power high performance control parameters
extern uint8_t b_acc;//control register
extern uint8_t dataxl;//x axis lsb
extern uint8_t dataxh;//x axis msb
extern uint8_t datayl;//y axis lsb
extern uint8_t datayh;//y axis msb
extern uint8_t datazl;//z axis lsb
extern uint8_t datazh;//z axis msb
extern uint8_t read_add;//I2C reading command register
extern uint8_t write_add;//I2C writing command register
extern int16_t proba1;//variable for two's complement function conversion, x;

extern int16_t proba3;//variable for two's complement function conversion, z;
//Character variable for UART purposes




#endif /* INC_ACC_VARIABLES_H_ */
