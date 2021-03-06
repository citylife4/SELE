/*
 * _24C16.c
 *
 * Created: 1/7/2012 19:11:24
 *  Author: embedds.com
 */ 

/*
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "usart.h"
#include "ee24c16.h"
//set stream pointer
FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);


int main(void)
{
	uint8_t u8ebyte;
	uint8_t u8erbyte;
	uint16_t u16eaddress = 0x07F0;
	uint8_t page = 0;
	uint8_t i;
	uint8_t eereadpage[16];
	uint8_t eewritepage[16] = { 'a', 'b', 'c', 'd', 'e', 'f', 'h', 'i',
								'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q'};
//Initialize USART0
USART0Init();
//
TWIInit();
//assign our stream to standard I/O streams
stdin=stdout=&usart0_str;
printf("\nWrite byte %#04x to eeprom address %#04x", 'a', u16eaddress);
if (EEWriteByte(u16eaddress, 'a') != ERROR)
{
	printf_P(PSTR("\nRead byte From eeprom"));
	if (EEReadByte(u16eaddress, &u8ebyte) != ERROR)
	{
		printf("\n*%#04x = %#04x", u16eaddress, u8ebyte);
	}
	else printf_P(PSTR("\nStatus fail!"));

}	
if(EEWritePage(page, eewritepage) != ERROR)
{
	printf_P(PSTR("\nReading 16 bytes from page 5 "));
	if (EEReadPage(page, eereadpage) != ERROR)
	{
		//compare send and read buffers
		for (i=0; i<8; i++)
		{
			if (eereadpage[i] != eewritepage[i])
			{
				printf("\n%#04x != %#04x", eereadpage[i], eewritepage[i]);
				break;
			}		
				else {
					printf("\n%#04x == %#04x", eereadpage[i], eewritepage[i]);
					continue;
				}
		}
		if (i==8)
			printf_P(PSTR("\nPage write and read success!"));
		else
			printf_P(PSTR("\nPage write and read fail!"));
	} else printf_P(PSTR("\nStatus fail!"));

}else printf_P(PSTR("\nStatus fail!"));

}*/
