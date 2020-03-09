//---------------------------------------------------
//STM32C8T6 LED灯控制头文件 -- led.h
//最小系统led灯在PC13口
//----------------------------------------------------

#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//---------外部函数-----------------
//----------------------------------
void LED_Configuration(void);
void LED_OFF(void);
void LED_ON(void);
void LED_Flash(void);
	
#endif
