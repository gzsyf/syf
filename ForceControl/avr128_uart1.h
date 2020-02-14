
#ifndef AVR128_UART1_H_
#define AVR128_UART1_H_

extern void uart_init1(unsigned int baud);
extern void USART1_SendByte(unsigned char data);
extern unsigned char USART1_ReceiveByte( void );


#endif /* AVR128_UART1_H_ */
