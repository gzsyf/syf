/**
  ******************************************************************************
  * File Name          : XXXXXX.h
  * Description        : This file provides code for the configuration
  *                      of the XXXXX instances.
  ******************************************************************************
  * @attention
  *
  * chip:  stm32c8t6 chip.
  * date:  2020.3.24
  * pin: PB6 -> SCL PB7 -> SDA 
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XXX_H
#define __XXX_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

#define SDA_IN()  		{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() 		{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

#define IIC_SCL_H       GPIO_SetBits(GPIOB , GPIO_Pin_6) 
#define IIC_SCL_L       GPIO_ResetBits(GPIOB , GPIO_Pin_6)

#define IIC_SDA_H         	GPIO_SetBits(GPIOB , GPIO_Pin_7)   
#define IIC_SDA_L         	GPIO_ResetBits(GPIOB , GPIO_Pin_7)

#define READ_SDA       	GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_7)

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */

void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);
void IIC_Init(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT gzsyf *****END OF FILE****/
