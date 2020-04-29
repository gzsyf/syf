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
  * note: this file must contain i2c.c
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PCF8574_H
#define __PCF8574_H
#ifdef __cplusplus
 extern "C" {
#endif

/* USER CODE BEGIN Includes --------------------------------------------------*/
#include "main.h"
/* USER CODE END Includes ----------------------------------------------------*/

/* USER CODE BEGIN Exported types --------------------------------------------*/
/* USER CODE END Exported types ----------------------------------------------*/


/* USER CODE BEGIN Private defines -------------------------------------------*/
#define	I2C_NOT_LAST	0x00
#define	I2C_LAST_BYTE	0x01
 
#define	I2C_READ	0x01
#define	I2C_WRITE	0x00
 
#define	PCF8574	0x7E
/* USER CODE END Private defines ---------------------------------------------*/


/* USER CODE BEGIN Prototypes ------------------------------------------------*/
u8 i2c_EIO_RD(u8* data);
u8 i2c_EIO_WD(u8 data);
/* USER CODE END Prototypes --------------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif /*__ XXX_H */

/********************************* REFERENCE ***********************************
 * [1] 
 ******************************************************************************/

/************************ (C) COPYRIGHT gzsyf *****END OF FILE*****************/
