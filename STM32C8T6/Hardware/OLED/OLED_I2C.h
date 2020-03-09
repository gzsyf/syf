/************************************************************************************
*************************************************************************************
*STM32C8T6 OLED_iic 用iic通信的迷你12864液晶屏 -- OLED_I2C.h
*io口的连接方式:
*PB6 -- SCL; PB7 -- SDA
*************************************************************************************
*************************************************************************************/

#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

//--------------------oled地址-----------------------------
//---------------------------------------------------------
#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78


//------------------------外部函数--------------------------------------
//----------------------------------------------------------------------
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

#endif
