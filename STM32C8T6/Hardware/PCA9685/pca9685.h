//---------------------------------------------------
//STM32C8T6 pca9685舵机驱动板头文件 -- pca9685.c
//io口连接：SCL --PB6   SDA -- PB7  vcc -- 3.3V  V+ -- 5V
//----------------------------------------------------

#ifndef __STM32PCA9685_H
#define __STM32PCA9685_H	

#include "stm32f10x.h"

//--------------------pca9685地址-----------------------------
//---------------------------------------------------------
#define pca_adrr 0x80

#define pca_mode1 0x0
#define pca_pre 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

//------------------计算值-------------------------------
//-------------------------------------------------------
#define jdMIN  115 // minimum
#define jdMAX  590 // maximum
#define jd000  130 //0度对应4096的脉宽计数值
#define jd180  520 //180度对应4096的脉宽计算值


//------------------------外部函数--------------------------------------
//----------------------------------------------------------------------
void pca_write(u8 adrr,u8 data);
u8 pca_read(u8 adrr);
void PCA_MG9XX_Init(float hz,u8 angle);
void pca_setfreq(float freq);
void pca_setpwm(u8 num, u32 on, u32 off);
void PCA_MG9XX(u8 num,u8 start_angle,u8 end_angle,u8 mode,u8 speed);


#endif









