#include <avr/io.h>
#include "avr128_uart1.h"

//-----------------------------------------
//UART1初始化函数
//------------------------------------------
void uart_init1(unsigned int baud) 
{

	UCSR1A = (1 << U2X1);								// 倍速								
	UCSR1B = (1 << TXEN1) | (1 << RXEN1);				// 允许接收和发送					
	UCSR1B |= (1 << RXCIE1);							// 允许接收中断						
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);				// 8位数据、1 位停止 异步 无校验
	
	switch(baud)
	{
		case 2400:	UBRR1H = 0x01;UBRR1L = 0xa0;break;
		case 4800: UBRR1H = 0x00;UBRR1L = 0xcf;break;
		case 9600: UBRR1H = 0x00;UBRR1L = 0x67;break;
		case 14400: UBRR1H = 0x00;UBRR1L = 0x44;break;
		case 19200: UBRR1H = 0x00;UBRR1L = 0x33;break;
		case 28800: UBRR1H = 0x00;UBRR1L = 0x22;break;
		case 38400: UBRR1H = 0x00;UBRR1L = 0x19;break;
		default: UBRR1H = 0x00;UBRR1L = 0x67;break;
	}						
}

//-----------------------------------------
//查询方式发送 一个字节
//-----------------------------------------
void USART1_SendByte(unsigned char data)
{
	while(!(UCSR1A&(1<<UDRE1)));
	UDR1=data; 
}

//-----------------------------------------
//查询方式发送 一个字符串
//-----------------------------------------
void UART1_SendArray(unsigned char *pArray)
{
   unsigned char *p = pArray;			   // p指向字符串头

   while (*p != '\0')		   // 循环直到字符串末尾
   {
	   USART1_SendByte(*p);	   // 发送1字节
	   p++;				   // p++指向下一字节
   }
}

//-----------------------------------------
//查询方式接收 
//------------------------------------------
unsigned char USART1_ReceiveByte( void )
{
	while ( !(UCSR1A & (1<<RXC1)) );
	return UDR1;
}
