//---------------------------------------------------
//STM32C8T6 LED灯控制文件 -- led.c
//STM32C8T6最小系统的灯在PC13口
//----------------------------------------------------
#include "stm32f10x.h"
#include "led.h"

//--------------------led初始化函数 --------------------
//用PC13口控制led灯
//------------------------------------------------------
void LED_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}


//--------------------led开函数 --------------------
//用PC13口控制led灯
//------------------------------------------------------
void LED_ON(void)
{
	PCout(13) = 0;
}


//--------------------led关函数 --------------------
//用PC13口控制led灯
//------------------------------------------------------
void LED_OFF(void)
{
	PCout(13) = 1;
}

//--------------------led闪烁函数 --------------------
//用PC13口控制led灯
//------------------------------------------------------
void LED_Flash(void)
{
	PCout(13) ^= 1;	
}
