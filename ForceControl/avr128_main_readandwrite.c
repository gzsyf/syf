/*
 * testModbus.c
 *
 * Created: 2019/8/13 21:39:43
 * Author : as
 */ 
# define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "avr128_uart1.h"
#include "modbus.h"

unsigned char tmp;

unsigned char receive_buf[8];
unsigned int crc_data;
unsigned char flag = 0 ;
unsigned char count = 0;
unsigned char i = 0;

unsigned int addr;
unsigned int crc;

unsigned char send_buf[7];
unsigned char parameter = 1;


void io_init(void)
{
	DDRD = 0xFF;
	PORTD = 0xFF;
}


int main(void)
{

	io_init();
	uart_init1(9600);
	
    while (1) 
    {
		if(count < 8)
		{
			
			receive_buf[count] = USART1_ReceiveByte();
			count ++;
		}
		
		if (count == 8)
		{
			
			count = 0;
			if(receive_buf[0] == 1) 
			{
				crc = crc16(receive_buf,6);
				if (crc == (receive_buf[6] << 8) + receive_buf[7])
				{
					// 如果功能码是3 就是读取单个寄存器
					if (receive_buf[1] == 3)
					{
						flag = 1;
								
					}
					// 如果功能码是6 就是写入单个寄存器
					else if (receive_buf[1] == 6)
					{
						flag = 1;
						parameter = receive_buf[5];
						// 写寄存器modbus是不用应答的
					}
					else tmp = USART1_ReceiveByte();			
				}
				else tmp = USART1_ReceiveByte();

			}
			else tmp = USART1_ReceiveByte();
		}


		send_buf[0] = 0x01;//站址
		send_buf[1] = 0x03;//读操作
		send_buf[2] = 5;//字节数
		send_buf[3] = 0x00;//第一个寄存器数据高字节
		send_buf[4] = parameter + 1;//第一个寄存器数据低字节
								
		/*计算crc*/
		crc_data = crc16(send_buf,5);
								
		send_buf[5] = crc_data >> 8; //低位
		send_buf[6] = crc_data;	   //高位
		
		
		while(flag == 1)
		{
			for (i = 0; i < 7; i++)
			{
				USART1_SendByte(send_buf[i]);
				_delay_ms(10);
			}
			flag = 0 ;
			count = 0;
		}
	
    }
}
