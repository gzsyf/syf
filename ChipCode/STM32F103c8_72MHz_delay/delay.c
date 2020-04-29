/**
  ******************************************************************************
  * File Name          : delay.c
  * Description        : This file provides code for the configuration
  *                      of the XXXXXX instances.
  ******************************************************************************
  * @attention
  *
  * chip:  				stm32f103c8t6 chip.  
  * date:  				2020.3.27
  * pin: 	 				None
  * frequency: 	 	72MHz
  *
  ******************************************************************************
  */

/* USER INCLUDES CODE BEGIN --------------------------------------------------*/
#include "delay.h"
/* USER INCLUDES CODE END ----------------------------------------------------*/


/* USER INIT CODE BEGIN ------------------------------------------------------*/
/* USER INIT CODE END --------------------------------------------------------*/


/* USER STATIC CODE BEGIN ----------------------------------------------------*/
/* USER STATIC CODE END ------------------------------------------------------*/


/* USER EXTERN CODE BEGIN ----------------------------------------------------*/
/**
 * @file   stm32f103c8t6 delay micro seconds
 * @brief  system clock frequency is 72Mhz
 * @param  u32 num: micro seconds
 * @retval None
 * @date 	2020.3.27
 */
void delayUs(u32 num){
    u32 temp;
    SysTick->LOAD=9*num;         //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;         //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;                //清零计数器
    do{
        temp=SysTick->CTRL;           //读取当前倒计数值
    }
     while((temp&0x01)&&(!(temp&(1<<16))));     //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}
/**
 * @file   stm32f103c8t6 delay milli second
 * @brief  system clock frequency is 72Mhz
 * @param  u32 num: milli seconds
 * @retval None
 * @date 	2020.3.27
 */
void delayMs(u32 num){
    u32 temp;
    SysTick->LOAD=9000*num;      //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;        //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;            //清零计数器
    do{

        temp=SysTick->CTRL;       //读取当前倒计数值
    }while((temp&0x01)&&(!(temp&(1<<16)))); //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}
/* USER EXTERN CODE END ------------------------------------------------------*/


/* USER INTERRUPT CODE BEGIN -------------------------------------------------*/
/* USER INTERRUPT  CODE END --------------------------------------------------*/
  

/********************************* REFERENCE ***********************************
 * [1] code come from
 * https://blog.csdn.net/qq_35874273/article/details/78508927?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-8&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-8
 ******************************************************************************/

/************************ (C) COPYRIGHT gzsyf *****END OF FILE*****************/
