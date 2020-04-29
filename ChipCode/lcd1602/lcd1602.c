 /**
  ******************************************************************************
  * File Name          : lcd1602.c
  * Description        : This file provides code for the configuration
  *                      of the XXXX instances.
  ******************************************************************************
  * @attention
  *
  * chip:  		XXXXXXchip.  
  * date:  		2020.3.26
  * pin: 		RS -> XX  R/W -> XX  E -> XX  DB0~7 -> XX  VEE -> GND
  * frequency: 	 	
  *
  ******************************************************************************
  */

/* USER INCLUDES CODE BEGIN --------------------------------------------------*/
#include "lcd1602.h"
/* USER CODE END Includes ----------------------------------------------------*/

/* USER GLOBAL VARIABLE BEGIN ------------------------------------------------*/
/* USER GLOBAL VARIABLE END --------------------------------------------------*/

/* USER INIT CODE BEGIN ------------------------------------------------------*/
 /**
  * @file   LCD1602 pin configuration
  * @brief  this code is initiate 8 pin mode LCD1602  
  * @param  None
  * @retval None
  * @date 	2020.3.26
  */
void configLcd1602(void){
	
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB , ENABLE);//使能GPIOA时钟

	  GPIO_InitStructure.GPIO_Pin = 0xff;																//init PORTA 0-7 is data pin
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 									//output push pull mode
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									//high mode
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;		//init PORTB 2-4 is control pin
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;									//output push pull mode
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 								//high mode
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
	
 
 /**
  * @file   LCD1602 D7 tun into input mode 
  * @brief  because of the LCD1602_Wait_Ready function
  * @param  None
  * @retval None
  * @date 	2020.3.26
  */
static void setInputModeDB7(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//input pull up mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//high mode
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}	



 /**
  * @file   LCD1602 D7 tun into output mode 
  * @brief  because of the LCD1602_Wait_Ready function
  * @param  None
  * @retval None
  * @date 	2020.3.26
  */

static void setOutputModeDB7(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 	//output push pull mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			//high mode
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}	
	
	
/* USER INIT CODE END --------------------------------------------------------*/


/* USER STATIC CODE BEGIN ----------------------------------------------------*/

 /**
  * @file   check the lcd1602 buzy or not
  * @brief  if DB7 equal 1 that lcd1602 is buzy then it will wait it ready
  * @param  None:
  * @retval None:
  * @date 	2020.3.27
  */
static void waitReadyLcd1602(void){
	u8 sta;
	DATAOUT(0xff);   				//PA set all
	LCD_RS_CLR();     			//RS 0
	LCD_RW_SET();     			//RW 1
	do
	{
		LCD_EN_SET();   			//EN 1
		delayMs(5);						//delay 5ms
		setInputModeDB7();
 		sta = READ_DB7();			//get DB7 state
		setOutputModeDB7();
		LCD_EN_CLR();  				//EN  0
	}while(sta & 0x80);			//DB7=1 -> buzy =0 -> ready
}

 /**
  * @file   write command to lcd1602
  * @brief  before writing RS RW reset
  * @param  u8 cmd: lcd 1602 command
	*				@arg xx:
  *				@arg xx:
  *				@arg xx: 
  *				@arg xx:
  * @retval None
  * @date 	2020.3.27
  */
static void writeComLcd1602(u8 cmd){
	waitReadyLcd1602();  //check it whether buzy
	LCD_RS_CLR();   
	LCD_RW_CLR();
	DATAOUT(cmd); 				//send the command
	LCD_EN_SET();
	LCD_EN_CLR();
}

 /**
  * @file 	write one byte to lcd1602  
  * @brief  write one byte to lcd1602
  * @param  u8 dat:
  * @retval None:
  * @date 	2020.3.27
  */
static void writeDatLcd1602(u8 dat){
	waitReadyLcd1602(); //check it whether buzy
	LCD_RS_SET();   
	LCD_RW_CLR();   
	DATAOUT(dat);   			//send the data
	LCD_EN_SET();  
	LCD_EN_CLR();
		
}

/* USER STATIC CODE END ------------------------------------------------------*/


/* USER EXTERN CODE BEGIN ----------------------------------------------------*/
 /**
  * @file   clean the screen
  * @brief  command is 0x01
  * @param  None:
  * @retval None:
  * @date 	2020.3.27
  */
void cleanScreenLcd1602(void){
	writeComLcd1602(0x01);
}

 /**
  * @file   set the cursor position
  * @brief  first you should confirm the address
  * @param  u8 x u8 y: x-axis 0~15  y-axis 0~1
  * @retval None:
  * @date 	2020.3.27
  */
void setCursorLcd1602(u8 x, u8 y){
	u8 addr;
	if (y == 0) addr = 0x80 + x;
	else addr = 0xC0 + x;
	writeComLcd1602(addr | 0x80);
}

 /**
  * @file   show a string on lcd1602
  * @brief  str ceiling is 15 letter ; num ; symbol
  * @param  u8 x u8 y u8 *str: x-axis 0~15  y-axis 0~1 stringpoint
  * @retval None:
  * @date 	2020.3.27
  */
void showStrLcd1602(u8 x, u8 y, u8 *str){
	setCursorLcd1602(x, y);
	while(*str != '\0'){
		writeDatLcd1602(*str++);
	}
}

 /**
  * @file   lcd1602 initialization 
  * @brief  
  * @param  None:
  * @retval None:
  * @date 	2020.3.27
  */
void initLcd1602(void){
	writeComLcd1602(0x38);	//16*2 screen, 5*7matrix
	writeComLcd1602(0x0C);	//show mode，close cursor
	writeComLcd1602(0x06);	//string static，adress ++
	writeComLcd1602(0x01);	//clean screen
}

/* USER EXTERN CODE END ------------------------------------------------------*/

/* USER INTERRUPT CODE BEGIN -------------------------------------------------*/
/* USER INTERRUPT  CODE END --------------------------------------------------*/

/********************************* REFERENCE ***********************************
 * [1] 
 ******************************************************************************/
/************************ (C) COPYRIGHT gzsyf *****END OF FILE*****************/




