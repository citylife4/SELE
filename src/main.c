
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "ee24c16.h"

uint8_t input[16];
uint8_t byte = 0;
uint16_t u16eaddress = 0x00001;
uint8_t logNumber = 0;

//set stream pointer
FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

int main(void)
{

	uint8_t u8ebyte;
	uint8_t u8erbyte;
	uint16_t u16eaddress = 0x00001;
/*
	uint8_t eewritepage1[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'h', 'i'};
	uint8_t eewritepage2[8] = {	'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q'};

	uint8_t eereadpage1[8];
	uint8_t eereadpage2[8];

	uint8_t i;
*/
	//Initialize USART0
	USART0Init();
	TWIInit();
	//assign our stream to standard I/O streams
	stdin=stdout=&usart0_str;
	sei();

	while(1) {

	}



}

ISR (USART_RX_vect)
{
	uint8_t u8ebyte;
	uint8_t u8erbyte;
	uint8_t i;
	uint16_t u16eaddress = 0x07F0;

    /* interrupt code here */

	input[byte] = getchar();
	printf("\n*%#04x : %d\n", input[byte], byte );
	byte++;

	/**/

	if(15 == byte) {

		// To Send..
		if (EEWriteByte(u16eaddress, logNumber) != ERROR) ;
		else printf_P(PSTR("\nStatus write fail!"));

		for(i=1 ; i < 16 ; i++)  {
			printf(" ");
			if (EEWriteByte(u16eaddress+i, input[i-1]) != ERROR)
			{
				printf_P(PSTR("\nRead byte From eeprom"));
				if (EEReadByte(u16eaddress+i, &u8ebyte) != ERROR)
				{
					printf("\n*%#04x = %#04x", u16eaddress+i, u8ebyte);
				}
				else printf_P(PSTR("\nStatus Read fail!"));

			}else printf_P(PSTR("\nStatus Write fail!"));
		}

		//To receive
		for(i=0 ; i < 16 ; i++)  {
			if (EEReadByte(u16eaddress+i, &u8ebyte) != ERROR)
			{
				printf("\n*%#04x = %#04x", u16eaddress+i, u8ebyte);
			}
			else printf_P(PSTR("\nStatus read fail!"));
		}

	}

}


