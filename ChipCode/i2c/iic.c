/**
  ******************************************************************************
  * File Name          : i2c.c
  * Description        : This file provides code for the configuration
  *                      of the XXXXXX instances.
  ******************************************************************************
  * @attention
  *
  * chip:  XXXXXXchip.  
  * date:  XX.XX.XX
  * pin: 
  * frequency:
  *
  ******************************************************************************
  */

/* USER INCLUDES CODE BEGIN --------------------------------------------------*/
#include "iic.h"
/* USER INCLUDES CODE END ----------------------------------------------------*/

/* USER GLOBAL VARIABLE BEGIN ------------------------------------------------*/
/* USER GLOBAL VARIABLE END --------------------------------------------------*/


/* USER INIT CODE BEGIN ------------------------------------------------------*/
/**
  * @file   configurate iic pins
  * @brief  stm32 : PB6->SCL and PB7->SDA
  * @calls  	xxxx:
  * @call by 	xxxx:
  * @param  None
  * @retval None
  * @date  2020.4.28
  */
void I2C_Configuration(void){
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;			/* config SCL */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;			/* config SDA */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 /**
  * @file   iic SDA turn into input mode 
  * @brief  iic SDA not only read and write
  * @calls  	xxxx:
  * @call by 	xxxx:
  * @param  None
  * @retval None
  * @date 	2020.4.28
  */
static void setInputModeSDA(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//input pull up mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//high mode
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}	
 /**
  * @file   iic SDA turn into output mode 
  * @brief  iic SDA not only read and write
  * @calls  	xxxx:
  * @call by 	xxxx:
  * @param  None
  * @retval None
  * @date 	2020.4.28
  */
static void setOutputModeSDA(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//output push pull mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	//high mode
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
	
/* USER INIT CODE END --------------------------------------------------------*/



/* USER STATIC CODE BEGIN ----------------------------------------------------*/
/**
  * @file   iic delay
  * @brief  because of proteus ,stm32 can't use delay
  *			if use other chip this must be changed 
  * @calls  	xxxx:
  * @call by 	i2c.c: every function in the i2c.c
  * @param  None
  * @retval None
  * @date 	2020.4.28
  */
void I2C_delay(void){	
   u8 i=50; /* 这里可以优化速度,经测试最低到5还能写入 */
   while(i){ 
     i--; 
   } 
}
/**
  * @file   iic start flag
  * @brief  do at the first
  * @calls  	xxxx:
  * @call by 	I2C_ReceiveByte(), I2C_SendByte()
  * @param  None
  * @retval None
  * @date 	2020.4.28
  */
u8 I2C_Start(void){
	I2C_delay();
	SDA_H;
	SCL_H;
	I2C_delay();
	
//	setInputModeSDA();
//	if(!SDA_read)return 0;	/* SDA线为低电平则总线忙,退出 */
//	setOutputModeSDA();
	
	SDA_L;
	I2C_delay();
	
//	setInputModeSDA();
//	if(SDA_read) return 0;	/* SDA线为高电平则总线出错,退出 */
//	setOutputModeSDA();
	
	SDA_L;
	I2C_delay();
	SCL_L;
	return 1;
}
/**
  * @file   iic stop flag
  * @brief  do at the end
  * @calls  	xxxx:
  * @call by 	i2c.c: I2C_ReceiveByte(), I2C_SendByte()
  * @param  None
  * @retval None
  * @date 	2020.4.28
  */
void I2C_Stop(void){
	SCL_L;
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	I2C_delay();
	I2C_delay();
	SDA_H;
	I2C_delay();
}
/**
  * @file   iic master answer slave
  * @brief  when slave send msg to master,master wanna receive continue
  * @calls  	xxxx:
  * @call by 	i2c.c: I2C_ReceiveByte()
  * @param  None
  * @retval None
  * @date 	2020.4.28
  */
void I2C_Ack(void){	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}
/**
  * @file   iic master no-answer slave
  * @brief  when slave send msg to master,master stop receive 
  * @calls  	xxxx:
  * @call by 	I2C_ReceiveByte()
  * @param  None
  * @retval None
  * @date 	2020.4.28
  */
void I2C_NoAck(void){	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}
/**
  * @file   iic write slave answer
  * @brief  if ask it is a good news,if nask it might can't find device
  * @calls  	xxxx:
  * @call by 	i2c.c: I2C_SendByte()
  * @param  None
  * @retval return = 1 ACK, =0 NOACK
  * @date 	2020.4.28
  */
u8 I2C_WaitAck(void){
	u8 k;
//	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	
	setInputModeSDA();
//	if(SDA_read){
//      SCL_L;
//      return 0;
//	}
	k = SDA_read;
	setOutputModeSDA();
	I2C_delay();
	SCL_L;
	
	if(k)return 0;
	return 1;
}

/* USER STATIC CODE END ------------------------------------------------------*/



/* USER EXTERN CODE BEGIN ----------------------------------------------------*/
/**
  * @file   iic send Byte
  * @brief  
  * @calls  	xxxx:
  * @call by 	xxxx:
  * @param  u8 SendByte: the data you wanna send
  * @retval u8: whether ok or not
  * @date 	2020.4.28
  */
u8 I2C_SendByte(u8 SendByte) {
    u8 i=8;
	u8 ask = 0;
    while(i--){
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
	    I2C_delay();
    }
    SCL_L;

	ask = I2C_WaitAck();
	
	return ask;
}
/**
  * @file   iic receive Byte
  * @brief  
  * @calls  	xxxx:
  * @call by 	xxxx:
  * @param  u8 flag: whether is last receive Byte or not
  * @retval u8: whether ok or not
  * @date 	2020.4.28
  */
u8 I2C_ReceiveByte(u8 flag){ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;
	setInputModeSDA();	
    while(i--){
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();
      if(SDA_read){
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
	setOutputModeSDA();
	
	if(flag){
		I2C_NoAck();
	}
	else
	I2C_Ack();
	
    return ReceiveByte;
}
/* USER EXTERN CODE END ------------------------------------------------------*/

/* USER INTERRUPT CODE BEGIN -------------------------------------------------*/
/* USER INTERRUPT  CODE END --------------------------------------------------*/


/********************************* REFERENCE ***********************************
 * [1] code come from
 * https://blog.csdn.net/fch112702/article/details/77547815
 ******************************************************************************/

/************************ (C) COPYRIGHT gzsyf *****END OF FILE*****************/


