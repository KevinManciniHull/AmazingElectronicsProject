#include "stdio.h"
acc_game_timer = 5000;
IODIR =  0x00;
GPPU =   0x06;
INTCAP  =  0x08;
GPIO =   0x09;
OPCODEW = 0x40;
OPCODER = 0x41;
accel_loop = 1;
mainmenuloop=1;
leds_binary=0x00;
global_variable=37;
UART1_rxBuffer[12]={0};
msg_acc[35]={0};
proba1=0;
proba2=0;
proba3=0;
sides_acc=0;
read_add = 0xD5;
write_add = 0xD4;
a_acc = 0xB0;
b_acc= 0x10;
 x_axis_l = 0x28;//out x l axis
 x_axis_h = 0x29;// out x h axis
y_axis_l = 0x2A;//out y l axis
y_axis_h = 0x2B;// out y h axis
z_axis_l = 0x2C;//out z l axis
z_axis_h = 0x2D;// out z h axis
templsb=0x20;
tempmsb=0x21;
dataxl=0;//x axis lsb
dataxh=0;//x axis msb
datayl=0;//y axis lsb
datayh=0;//y axis msb
datazl=0;//z axis lsb
datazh=0;//z axis msb



