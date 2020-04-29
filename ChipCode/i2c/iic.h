/**
  ******************************************************************************
  * File Name          : XXXXXX.h
  * Description        : This file provides code for the configuration
  *                      of the XXXXX instances.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H	
#ifdef __cplusplus
 extern "C" {
#endif

/* USER CODE BEGIN Includes --------------------------------------------------*/
#include "main.h"
/* USER CODE END Includes ----------------------------------------------------*/

/* USER CODE BEGIN Exported types --------------------------------------------*/
/* USER CODE END Exported types ----------------------------------------------*/


/* USER CODE BEGIN Private defines -------------------------------------------*/
#define I2C_SCL_PIN      				GPIO_Pin_6   
#define I2C_SCL_GPIO_PORT   	       	GPIOB  
 
#define I2C_SDA_PIN      				GPIO_Pin_7   
#define I2C_SDA_GPIO_PORT   	       	GPIOB  
 

#define SCL_H         	 GPIO_SetBits(I2C_SCL_GPIO_PORT, I2C_SCL_PIN)   /*GPIOB->BSRR = GPIO_Pin_6*/
#define SCL_L            GPIO_ResetBits(I2C_SCL_GPIO_PORT, I2C_SCL_PIN) /*GPIOB->BRR  = GPIO_Pin_6 */
   
#define SDA_H         	 GPIO_SetBits(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)   /*GPIOB->BSRR = GPIO_Pin_7*/
#define SDA_L         	 GPIO_ResetBits(I2C_SDA_GPIO_PORT, I2C_SDA_PIN) /*GPIOB->BRR  = GPIO_Pin_7*/

#define SCL_read       	GPIO_ReadInputDataBit(I2C_SCL_GPIO_PORT, I2C_SCL_PIN)/* GPIOB->IDR  & GPIO_Pin_6   */
#define SDA_read       	GPIO_ReadInputDataBit(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)/*GPIOB->IDR  & GPIO_Pin_7	  */
/* USER CODE END Private defines ---------------------------------------------*/


/* USER CODE BEGIN Prototypes ------------------------------------------------*/
void I2C_Configuration(void);
u8 I2C_SendByte(u8 SendByte);
u8 I2C_ReceiveByte(u8 flag);
/* USER CODE END Prototypes --------------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif /*__ XXX_H */

/********************************* REFERENCE ***********************************
 * [1] code come from
 * https://blog.csdn.net/fch112702/article/details/77547815
 ******************************************************************************/

/************************ (C) COPYRIGHT gzsyf *****END OF FILE*****************/



