/**
  ******************************************************************************
  * File Name          : i2c.c
  * Description        : This file provides code for the configuration
  *                      of the i2c instances.
  ******************************************************************************
  * @attention
  *
  * chip:  stm32c8t6 chip.
  * date:  2020.3.24
  * pin: PB6 -> SCL PB7 -> SDA 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER INIT CODE BEGIN */

 /**
  * @file   init io
  * @brief  PB6 -> SCL PB7 -> SDA
  * @param  None
  * @retval None
  * @date   2020.3.24.11:49
  */
void IIC_Init(void){					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
}

/* USER INIT CODE END */


/* USER EXTERN CODE BEGIN */

 /**
  * @file   generate IIC start signal
  * @brief  IIC start： when CLK is high DATA change form high to low
  * @param  None
  * @retval None
  * @date   2020.3.24.11:49
  */
void IIC_Start(void){
	SDA_OUT();     	// sda change to output
	IIC_SDA=1;	  	// 
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;		// START: when CLK is high,DATA change form high to low 
	delay_us(4);	// must be 4us
	IIC_SCL=0;		// 钳住I2C总线，准备发送或接收数据 
}

 /**
  * @file   generate IIC stop signal
  * @brief  IIC stop： when CLK is high DATA change form low to high
  * @param  None
  * @retval None
  * @date   2020.3.24.11:49
  */
void IIC_Stop(void){
	SDA_OUT();		// sda change to output
	IIC_SCL=0;
	IIC_SDA=0;		// STOP: when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1;
	delay_us(4);	// template!!
	IIC_SDA=1;		//发送I2C总线结束信号
	delay_us(4);
}

 /**
  * @file   wait slave ASK signal
  * @brief  IIC wait ASk： when CLK is high read DATA whether it is ask or unask 
  * @param  None
  * @retval u8 : 
  *         	@ arg 0:
  *				@ arg 1:
  */
u8 IIC_Wait_Ack(void){
	u8 ucErrTime=0;
	SDA_IN();      			//SDA set as input
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA){		// if read slave is unask  stop iic
		ucErrTime++;		// timeout
		if(ucErrTime>250){
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;				// CLOCK down	   
	return 0;  
} 

 /**
  * @file   master generate IIC ask signal
  * @brief  master IIC ask： when DATA is low CLKCLK change form low to high to low
  *         why to ask : everytime master receive a message master should give slave a ask
  * @param  None
  * @retval None
  * @date   2020.3.24.12:02
  */
void IIC_Ack(void){
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;		// when DATA is low
	delay_us(2);
	IIC_SCL=1;		// CLKCLK change form low to high
	delay_us(2);	// must more than 4us
	IIC_SCL=0;		// to low
}

 /**
  * @file   master generate IIC unask signal
  * @brief  master IIC unask： when DATA is high CLOCK change form low to high to low
  *         why to unask : when master unwant to receive message master should give a unask 
  * @param  None
  * @retval None
  * @date   2020.3.24.12:02
  */	    
void IIC_NAck(void){
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;		// when DATA is high
	delay_us(2);	
	IIC_SCL=1;		// CLKCLK change form low to high
	delay_us(2);	// must more than 4us
	IIC_SCL=0;		// to low
}		

 /**
  * @file   master send one byte message
  * @brief  when CLOCK is high DATA can send its bit
  * @param  u8 txd : 
  * @retval None
  * @date   2020.3.24.12:21
  */		  
void IIC_Send_Byte(u8 txd){                       
    u8 t;   
	SDA_OUT(); 				// set DATA as output	    
    IIC_SCL=0;				// CLOCK pull down first
    for(t=0;t<8;t++){		// loop 8 times      
		if((txd&0x80)>>7)	// get the top bit
			IIC_SDA=1;		// it is 1 bit
		else
			IIC_SDA=0;		// it is 0 bit
		txd<<=1; 			// to get the next bit
		delay_us(2);   		// 对TEA5767这三个延时都是必须的 ！！！！
		IIC_SCL=1;			// put up CLOCK to get the message
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	

 /**
  * @file   master read one byte message
  * @brief  when CLOCK is high DATA can read its bit
  * @param  u8 ask : after get 8 bit master whether need to get next 
  *				@arg 0: donot get next message 
  *             @arg 1: get next message
  * @retval u8 receive: the byte you read 
  * @date   2020.3.24.12:51
  */	
u8 IIC_Read_Byte(unsigned char ack){
	u8 i,receive=0;
	SDA_IN();					 // set DATA as input
    for(i=0;i<8;i++ ){
        IIC_SCL=0; 				 // CLOCK pull down first
        delay_us(2);
		IIC_SCL=1;				 // ready to read
        receive<<=1;		
        if(READ_SDA)receive++;   // if read a 1 bit
		delay_us(1); 
    }					 
    if (!ack)					 // after get 8 bit master whether need to get next 	
        IIC_NAck();				
    else
        IIC_Ack();    
    return receive;
}

/* USER EXTERN CODE END */


/* USER STATIC CODE BEGIN */

/* USER STATIC CODE END */

/************************ (C) COPYRIGHT gzsyf *****END OF FILE****/
