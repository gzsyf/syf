/**
  ******************************************************************************
  * File Name          : delay.c
  * Description        : This file provides code for the configuration
  *                      of the delay instances.
  ******************************************************************************
  * @attention
  *
  * chip:  				AT89C51 chip.  
  * date:  				2020.3.27
  * pin: 	 				None
  * frequency: 	 	12MHz
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"

/* USER INIT CODE BEGIN */

/* USER INIT CODE END */


/* USER STATIC CODE BEGIN */

/* USER STATIC CODE END */


/* USER EXTERN CODE BEGIN */

 /**
  * @file   AT89C51 delay micro seconds
  * @brief  system clock frequency is 12Mhz
  * @param  u16 num: micro seconds
  * @retval None
  * @date 	2020.3.27
  */

void delayMs(u16 num){

    unsigned int i=0,j=0;
    for(i=0;i<num;i++)
        for(j=0;j<21738;j++);
}

 /**
  * @file   AT89C51 delay milli second
  * @brief  system clock frequency is 12Mhz
  * @param  u16 num: milli seconds
  * @retval None
  * @date 	2020.3.27
  */

void delayUs(u16 num){

    unsigned int i=0,j=0;
    for(i=0;i<num;i++)
        for(j=0;j<123;j++);
}

  

/* USER EXTERN CODE END */




/************************ (C) COPYRIGHT gzsyf *****END OF FILE****/
