# define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "avr128_uart1.h"

unsigned char buff[8] = {0,0,0,0,0,0,0,0};
unsigned char flag = 0 ;
unsigned char count = 0;
unsigned char i = 0;

void io_init(void) 
{							
	DDRD = 0xFF;
	PORTD = 0xFF;
}

int main(void)
{
    /* Replace with your application code */
	io_init();
	uart_init1(9600);
    while (1) 
    {
		if(count < 8)
		{
			buff[count] = USART1_ReceiveByte();
			count ++;
		}
		if (count == 8)
		{
			flag = 1;
		}
		while(flag == 1)
		{
			for (i = 0; i < 8; i++)
			{
				USART1_SendByte(buff[i]);
			}
		}
    }
}
