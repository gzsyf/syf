/**
  ******************************************************************************
  * File Name          : XXXX.c
  * Description        : This file provides code for the configuration
  *                      of the XXXXXX instances.
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

/* USER INCLUDES CODE BEGIN --------------------------------------------------*/
#include "PCF8574.h"
/* USER INCLUDES CODE END ----------------------------------------------------*/

/* USER GLOBAL VARIABLE BEGIN ------------------------------------------------*/
/* USER GLOBAL VARIABLE END --------------------------------------------------*/



/* USER INIT CODE BEGIN ------------------------------------------------------*/
/* USER INIT CODE END --------------------------------------------------------*/


/* USER STATIC CODE BEGIN ----------------------------------------------------*/
/* USER STATIC CODE END ------------------------------------------------------*/

/* USER EXTERN CODE BEGIN ----------------------------------------------------*/
/**
  * @file   主机从I2C1总线上的PCF8574TS设备中读取一个字节数据
  * @brief  获取的数据为8个IO口状态
  * @calls  	xxxx:
  * @call by 	xxxx:
  * @param  None
  * @retval 0:失败  1:成功
  * @date 
  */
u8 i2c_EIO_RD(u8* data){
	u8 state,ret;
	I2C_Start();									//!<启动I2C总线
	state = I2C_SendByte(PCF8574 | I2C_READ);		//!<发送器件地址,主机读
	if(state == 1){
		*data = I2C_ReceiveByte(I2C_LAST_BYTE);	//!<读取数据
		ret = 1;
	}else{
		ret = 0;
	}
	I2C_Stop();
	return ret;
}
/**
  * @file   主机向I2C1总线上的PCF8574TS设备中写入一个字节数据
  * @brief  
  * @calls  	xxxx:
  * @call by 	xxxx:
  * @param  data：写入的数据
  * @retval u8: 0:失败  1:成功
  * @date 
  */
u8 i2c_EIO_WD(u8 data){
	u8 state,ret;
	I2C_Start();									//!<启动I2C总线
	state = I2C_SendByte(PCF8574|I2C_WRITE);	//!<发送器件地址,主机写
	if(state==1){
	  	state = I2C_SendByte(data);
		if(state==1){
			ret = 1;
		}else{
			ret = 0;
		}
	}else{
		ret = 0;
	}
	I2C_Stop();
	return ret;
} 

/* USER EXTERN CODE END ------------------------------------------------------*/

/* USER INTERRUPT CODE BEGIN -------------------------------------------------*/
/* USER INTERRUPT  CODE END --------------------------------------------------*/


/********************************* REFERENCE ***********************************
 * [1] code come from
 * https://blog.csdn.net/fch112702/article/details/77547815
 ******************************************************************************/

/************************ (C) COPYRIGHT gzsyf *****END OF FILE*****************/
