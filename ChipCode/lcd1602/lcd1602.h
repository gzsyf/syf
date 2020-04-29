 /**
  ******************************************************************************
  * File Name          : Lcd1602.h
  * Description        : This file provides code for the configuration
  *                      of the Lcd1602 instances.
  ******************************************************************************
  * @attention
  *
  * chip:  XXXXXXchip.  
  * date:  2020.3.26
  * pin: 	 RS -> XX  R/W -> XX  E -> XX  DB0~7 -> XX VEE -> GND
  * frequency: 	 	
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD1602_H
#define __LCD1602_H
#ifdef __cplusplus
 extern "C" {
#endif

/* USER CODE BEGIN Includes --------------------------------------------------*/
#include "main.h"

/* USER CODE END Includes ----------------------------------------------------*/


/* USER CODE BEGIN Exported types --------------------------------------------*/
/* USER CODE END Exported types ----------------------------------------------*/


/* USER CODE BEGIN Private defines -------------------------------------------*/
#define RS_GPIO_PORT GPIOB
#define RS_PIN GPIO_Pin_2
	 
#define RW_GPIO_PORT GPIOB
#define RW_PIN GPIO_Pin_3
	 
#define EN_GPIO_PORT GPIOB
#define EN_PIN GPIO_Pin_4

#define DB7_GPIO_PORT GPIOA
#define DB7_PIN GPIO_Pin_7
	 
#define DATA_GPIO_PORT GPIOA

	 
#define	LCD_RS_SET()	GPIO_SetBits(RS_GPIO_PORT, RS_PIN)
#define	LCD_RS_CLR()	GPIO_ResetBits(RS_GPIO_PORT, RS_PIN)
 
#define	LCD_RW_SET()	GPIO_SetBits(RW_GPIO_PORT, RW_PIN)
#define	LCD_RW_CLR()	GPIO_ResetBits(RW_GPIO_PORT, RW_PIN)
 
#define	LCD_EN_SET()	GPIO_SetBits(EN_GPIO_PORT, EN_PIN)
#define	LCD_EN_CLR()	GPIO_ResetBits(EN_GPIO_PORT, EN_PIN)

#define READ_DB7() 		GPIO_ReadInputDataBit(DB7_GPIO_PORT, DB7_PIN)
 
#define	DATAOUT(x)		GPIO_Write(DATA_GPIO_PORT, x)
/* USER CODE END Private defines ---------------------------------------------*/


/* USER CODE BEGIN Prototypes ------------------------------------------------*/
void configLcd1602(void);
void initLcd1602(void);
void cleanScreenLcd1602(void);
void setCursorLcd1602(u8 x, u8 y);
void showStrLcd1602(u8 x, u8 y, u8 *str);
/* USER CODE END Prototypes --------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /*__ LCD1602_H */

/********************************* REFERENCE ***********************************
 * [1] 
 ******************************************************************************/

/************************ (C) COPYRIGHT gzsyf *****END OF FILE*****************/
















//#ifndef __LCD_H
//#define __LCD_H

//#include "stm32f10x.h"
//#include "delay.h"

// 
////1602指令/数据 引脚
//#define	LCD_RS_SET()	GPIO_SetBits(GPIOB, GPIO_Pin_2)
//#define	LCD_RS_CLR()	GPIO_ResetBits(GPIOB, GPIO_Pin_2)
// 
////1602读写引脚
//#define	LCD_RW_SET()	GPIO_SetBits(GPIOB, GPIO_Pin_3)
//#define	LCD_RW_CLR()	GPIO_ResetBits(GPIOB, GPIO_Pin_3)
// 
////1602使能引脚
//#define	LCD_EN_SET()	GPIO_SetBits(GPIOB, GPIO_Pin_4)
//#define	LCD_EN_CLR()	GPIO_ResetBits(GPIOB, GPIO_Pin_4)

//#define READ_DB7() 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)
// 
////1602数据端口	PD4~7
//#define	DATAOUT(x)	GPIO_Write(GPIOA, x)


//void LCD_INIT(void);
//void LCD1602_Wait_Ready(void);
//void LCD1602_Write_Cmd(u8 cmd);
//void LCD1602_Write_Dat(u8 dat);
//void LCD1602_ClearScreen(void);
//void LCD1602_Set_Cursor(u8 x, u8 y);
//void LCD1602_Show_Str(u8 x, u8 y, u8 *str);
//void LCD1602_Init(void);

//#endif