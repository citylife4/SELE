/*
 * usart.c
 *
 * Created: 1/7/2012 23:03:08
 *  Author: embedds.com
 */ 
#include "usart.h"

void USART0Init(void)
{
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission, reception and interupt
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
}

uint8_t USART0SendByte(char u8Data, FILE *stream)
{
	   if(u8Data == '\n')
	   {
		  USART0SendByte('\r', 0);
	   }
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = u8Data;
	return 0;
}
uint8_t USART0ReceiveByte(FILE *stream)
{
	uint8_t u8Data;

	// Wait for byte to be received
	while(!(UCSR0A&(1<<RXC0))){};

	u8Data=UDR0;
	//echo input data
	USART0SendByte(u8Data,stream);
	// Return received data

	return u8Data;
}
