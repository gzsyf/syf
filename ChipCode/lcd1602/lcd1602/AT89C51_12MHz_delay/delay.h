/**
  ******************************************************************************
  * File Name          : delay.h
  * Description        : This file provides code for the configuration
  *                      of the XXXXX instances.
  ******************************************************************************
  * @attention
  *
  * chip:   			stm32f103c8t6 chip.  
  * date:   			2020.3.27
  * pin: 					None
  * frequency: 	 	72MHz
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
 #include "main.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void delayUs(u16 num);
void delayMs(u16 num);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ XXX_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT gzsyf *****END OF FILE****/
