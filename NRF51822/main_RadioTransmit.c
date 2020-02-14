#include "project_nrf51822.c"//包含51822工程需要用到的头文件（晶振，延时，io脚什么的

#include "pca1000x.h"
#include "Radio_TandR_1.h"

#include <stdbool.h>//调用bool
#include "stdio.h"//标准输入输出
#include <stdint.h>//声明已知大小的整数或显示特征的整数



int main(void)
{
	uint8_t cr;
	
	init();//时钟初始化
  ioinit();//引脚初始化
	radio_configure();//无线电配置（也可用于和nrf2401通讯）
	KEY_Init();
	//nrf_gpio_range_cfg_output(LED_START,LED_STOP);
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
	
	NRF_RADIO->PACKETPTR = (uint32_t)packet_T;//指针指向数据缓冲packet
	
	
  while(1)
  {	
		nrf_delay_ms(300);
		nrf_gpio_pin_toggle(LED_2);
		nrf_delay_ms(300);
		
		cr = simple_uart_get();
		packet_T[1] = cr;
		//Key_Scan() == 3
		tx();
  }
	
}
