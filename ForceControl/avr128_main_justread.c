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
unsigned char readCount;
unsigned char byteCount;
unsigned int crc;

// ---------- initialize the gpio --------------
void io_init(void)
{
	DDRD = 0xFF;
	PORTD = 0xFF;
}

// ------ main ---------
int main(void)
{
	unsigned char send_buf[7];
	io_init();
	uart_init1(9600);
	
    while (1) 
    {
    // collect 8 byte data
		if(count < 8)
		{
			receive_buf[count] = USART1_ReceiveByte();
			count ++;
		}
		
		if (count == 8)
		{
			
			count = 0;    //remember it 
			if(receive_buf[0] == 1)  // 校验设备地址为 力控里面设设备地址为01
			{
				crc = crc16(receive_buf,6);  
				if (crc == (receive_buf[6] << 8) + receive_buf[7]) // 校验crc码
				{
					if (receive_buf[1] == 3)  // 校验功能码 这里可以改一下
					{
						addr = receive_buf[3];
						readCount = receive_buf[5];
						byteCount = readCount*2;
						flag = 1;		
					}
					else tmp = USART1_ReceiveByte();		 //this is use to shift the modbus code
                                               //因为modbus一直在发, 错了也发, 移一下位, 让他纠正错误
				}
				else tmp = USART1_ReceiveByte();
			}
			else tmp = USART1_ReceiveByte();
		}
		
		send_buf[0] = 0x01;//站址
		send_buf[1] = 0x03;//读操作
		send_buf[2] = byteCount + 3;//字节数
		send_buf[3] = 0x00;//第一个寄存器数据高字节
		send_buf[4] = 0x01;//第一个寄存器数据低字节
		    
		/*计算crc*/
		crc_data = crc16(send_buf,byteCount + 3);
		    
		send_buf[5]=crc_data >> 8; //低位
		send_buf[6]=crc_data;	   //高位
		
		while(flag == 1)
		{
			for (i = 0; i < 7; i++)
			{
				USART1_SendByte(send_buf[i]); //回传数据 应答
				_delay_ms(10);
			}
			flag = 0 ;
			count = 0;
		}
	
    }
}
