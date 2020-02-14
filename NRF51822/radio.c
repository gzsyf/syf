//arm_startup_nrf51.s是启动文件，包含中断定义
//#include "nrf51.h"//包含51822各个寄存器结构地址。以上2文件就可以控制51822了。
//nrf51_bitfields.h包含各个寄存器设置值的作用
#include "project_nrf51822.c"//包含51822工程需要用到的头文件（晶振，延时，io脚什么的

#include "pca1000x.h"
#include "Radio_TandR_1.h"


#include <stdbool.h>//调用bool
#include "stdio.h"//标准输入输出
#include <stdint.h>//声明已知大小的整数或显示特征的整数


//QxC(Q开始，C结束，x是数据char)
uint8_t packet_T[3]={'Q','1','C'};  //Send 的数据包

uint8_t volatile packet_R[3];  //Receive 的数据包


//--------------------------------------------------
//radio收集一个字节的数据
//--------------------------------------------------
void rx()
{
				nrf_gpio_pin_set(LED_1);
				//------r and t transform-------
				NRF_RADIO->EVENTS_READY = 0U; //收发模式转换完成  标志位       
        NRF_RADIO->TASKS_RXEN   = 1U; //接收模式
        while(NRF_RADIO->EVENTS_READY == 0U) //等待收发模式转换完成(接收模式)标志位
        {
           // Do nothing.等待
        }
				
				//------Start transfer-------
        NRF_RADIO->EVENTS_END  = 0U;//传输完成  标志位     
        NRF_RADIO->TASKS_START = 1U; // 开始传输
        while(NRF_RADIO->EVENTS_END == 0U)//等待传输完成  标志位
        {
           nrf_gpio_pin_clear(LED_0);// blue亮  //传输不成功
						// Do nothing.等待
        }
				
				//--------Check the message-------
        if (NRF_RADIO->CRCSTATUS == 1U)//如果CRC校验正确
        {
					
          //nrf_gpio_port_write(NRF_GPIO_PORT_SELECT_PORT1, packet[0]);
					if(packet_R[0]==('Q'))//确定开始
					   {
              if(packet_R[2]==('C'))//结束正确
                {  
									
									
									
									nrf_gpio_pin_set(LED_0);// blue灭，数据成功了，灯越暗说明数据传输越快。
									
									                       //不亮说明,数据传输失败。
                }
								else
								{
									
									NRF_RADIO->EVENTS_DISABLED = 0U;//无线关闭   标志位
									return;
								}
             }
						 else
						 {
							 //nrf_gpio_pin_set(19);
							 NRF_RADIO->EVENTS_DISABLED = 0U;//无线关闭   标志位
							 return;
						 }
        }
				
				nrf_gpio_pin_clear(LED_1);
				//-------close transfer--------
        NRF_RADIO->EVENTS_DISABLED = 0U;//无线关闭   标志位
				NRF_RADIO->TASKS_DISABLE   = 1U;// 关闭无线设备
        while(NRF_RADIO->EVENTS_DISABLED == 0U)//等待设备关闭
        {
            // Do nothing.
        }
}


//--------------------------------------------------
//radio发送一个字节的数据
//--------------------------------------------------
void tx()
{
				nrf_gpio_pin_set(LED_2);//传输成功led0亮，越暗说明有干扰，需要改频率  输出高电平到pa.1
				//------r and t transform-------
				NRF_RADIO->EVENTS_READY = 0U;//收发模式转换完成标志位。复位
				NRF_RADIO->TASKS_TXEN = 1U;//启动无线电为发射模式
				while (NRF_RADIO->EVENTS_READY == 0U);//等待收发模式转换完成
				//nrf_gpio_pin_set(14);
	
				//------Start transfer-------
				NRF_RADIO->EVENTS_END  = 0U;//传输完成标志位，复位     
				NRF_RADIO->TASKS_START = 1U;//开始传输
				while(NRF_RADIO->EVENTS_END == 0U) //等待传输完成
        {
					  nrf_gpio_pin_clear(LED_3);//传输失败led0灭
            // Do nothing.
        }

						//-------传输完成的效果-------组1引脚（8~15）
						//nrf_gpio_port_write(P0, ~packet[0]);//数值显示在单片机P0口上 1000 1111
				
				//-------If transfer successful------------
				nrf_gpio_pin_set(LED_3);//传输成功led0亮，越暗说明有干扰，需要改频率  输出高电平到pa.1
				
				//-------close transfer--------
				NRF_RADIO->EVENTS_DISABLED = 0U;//无线关闭标志位  复位
				NRF_RADIO->TASKS_DISABLE   = 1U;// 关闭无线设备
        while(NRF_RADIO->EVENTS_DISABLED == 0U)//等待设备关闭
        {
            // Do nothing.
        }
				nrf_gpio_pin_clear(LED_2);//传输成功led0亮，越暗说明有干扰，需要改频率  输出高电平到pa.1
				
}



void tx_string(const uint8_t* str)
{
  uint_fast8_t i = 0;
  uint8_t ch = str[i++];
  while (ch != '\0')
  {
		packet_T[1] = ch;
    tx();
    ch = str[i++];
		nrf_delay_ms(1000);
  }
}

uint8_t* rx_string(uint8_t len,uint8_t start)
{
  uint8_t array[20];
	uint8_t* p;
  uint8_t i = 0;
	
	while(1)
	{
		rx();
		if(packet_R[1] == start)
		{
			while (i <= len-1)
			{
				rx();
				array[i] = packet_R[1];
				i++;
			}
			break;
		}
		else continue;
	}
	p = array;
	return p;
}
