
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "24lc02b.h"
#include "usart.h"

#define RED 1
#define GREEN 2


uint8_t input[16];
uint8_t input2[5]={'T','E','S','T','E'};
uint8_t byte = 0;
uint8_t logNumber[3] = {'0','0','1'} ;
uint8_t zeros[256] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
//Melhorar este codigo
uint8_t address = 0x00;
uint8_t lightStatus = 0;

//tosend
uint8_t greentored[4] = {'g', '-', 'r', '\n'};
uint8_t redtogreen[4]=  {'r', '-', 'g','\n'};

//set stream pointer
FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

void sendLog(uint8_t address);
uint8_t mysend(uint8_t* string, uint8_t num, uint8_t memstart);
uint8_t myread(uint8_t memstart, uint8_t size);
uint8_t myclean();


int main(void)
{

	//Initialize USART0
	USART0Init();
	TWIInit();

	DDRD = 0xFC;         //PB as output
	DDRB = 0xF8;  		// SET PB0 e PB1 as Input

	//assign our stream to standard I/O streams
	stdin=stdout=&usart0_str;

	//Set Interrupts Enabled
	sei();

	while(1) {

	}

}

void sendLog(uint8_t logaddress) {		//Writes log number to memory

	mysend(logNumber, 3, logaddress);

	if(logNumber[2] == '9')
	{
		if(logNumber[1] == 9)
		{
			logNumber[2] = '0';
			logNumber[1] = '0';
			logNumber[0] += 1;
		}
		else
			logNumber[2] = '0';
			logNumber[1] += 1;
	}
	else
	{
		logNumber[2] += 1;
	}


}

uint8_t mysend(uint8_t* string, uint8_t num, uint8_t memstart)
{
	int i;
	byte=0;

	for(i=0;i< num;i++)
	{
		//printf("String: %c to Address: %d\n",string[byte], address+i);
		if (EEWriteByte(memstart, string[byte]) == ERROR)
			return 0;
		else
			printf("Successful Write of char %c to address %#04x\n",string[byte],memstart);
			_delay_ms(10);
		byte++;
		memstart++;
	}

	return 1;
}

uint8_t myclean()
{
	int i;
	byte=0;

	for(i=0;i< 32;i++)
	{
		if (EEWriteByte(i, '0') == ERROR)
			return 0;
		else
			_delay_ms(10);
		byte++;
	}

	return 1;
}


uint8_t myread(uint8_t memstart, uint8_t size)
{

	uint8_t i;
	uint8_t aux;
	uint8_t buff[256];

	//printf("memstart: %d, size: %d\n",memstart, size);

	for(i=0 ; i<size ; i++){
		if (EEReadByte(memstart, &aux) != ERROR){
			//printf("Read string: %c from %d(END)\n",aux, memstart);
			//_delay_ms(10);
			buff[i] = aux;
		}
		else {
			printf("\nERROR\n");
			return 0;
		}


		byte++;
		memstart++;
	}


	buff[i]='\0';
	_delay_ms(10);
	printf("Successfully read string:\n %s (END)\n",buff);
	return 1;
}






ISR (USART_RX_vect)		//Interrupt on reception
{

	input[byte] = getchar();

	if('g' == input[byte]){ //Red to green
		PORTD=0x04;
		printf("\nGREEN TURNED ON\n");
		//printf("\nPORTB %04x\n", PINB);
		sendLog(address);
		address+=3;

		mysend(redtogreen, 4, address);

		myread(address-3, 7);
		address+=4;

		if(PINB!=0x06)
			printf("Green LED failure\n");

	}
	else if('r' == input[byte]){ //green to red
		PORTD=0x08;
		printf("\nRED TURNED ON\n");
		sendLog(address);
		address+=3;

		mysend(greentored, 4, address);

		myread(address-3, 7);

		address+=4;

		if(PINB!=0x05)
			printf("Red LED failure\n");
	}
	else if('l' == input[byte]) // read log
	{
		myread(0x00, address);
	}
	else if('b'== input[byte]) //Test backup red light
	{
		PORTD=0x10;
		printf("\nBackup RED TURNED ON\n");

		if(PINB!=0x03)
			printf("Backup Red LED failure\n");

	}
	else if('t' == input[byte])	//Memory test
	{
			//Implement march tests
		if(mysend(zeros,32,0))
			printf("Zeros success\n");
		else
			printf("Zeros failed\n");

	}
	else if('c' == input[byte]) //clean memory
	{
		address=0x00;
		logNumber[0] = '0';
		logNumber[1] = '0';
		logNumber[2] = '1';

		if(!myclean())
			printf("Clean failed\n");
		else
			printf("Memory successfully cleaned\n");
	}
	else if('u' == input[byte]) //clean memory
	{
		while(1){
			printf("teste\n");
			_delay_ms(1000);
		}
	}

	//Pode fazer 32 escritas!!

	//A implementar:
	//Se vermelho falhar -> backup
	//Deteçao de falha +/-
	//Ligar e desligar as luzes progressivamente
	//Circuito pronto para 12V?


	/*

	if(15 == byte){
		if(mysend(input,0))
			printf("Write of string to memory successfull\n");
		else
			printf("Write of string to memory unsuccessful\n");

		if(myread(&input,0,15))
			printf("Read of string from memory successfull\n");
		else
			printf("Read of string from memory unsuccessfull\n");

	}

	/************************************************************
	//Testes de leitura e escrita na memoria
	//Primeiro escreve e le 1 byte, depois escreve e le 4 bytes
	/***********************************************************


	if (EEWriteByte(address, input[byte]) == ERROR)
		printf("Write failed!\n");
	else
		printf("Successfull Write of char %c to address %#04x\n",input[byte],address);


	if (EEReadByte(address, &u8ebyte) != ERROR)
		printf("Successfull Read of char %c from address %#04x\n",u8ebyte,address);
	else
		printf("Read failed!");

	address++;
	byte++;

	//Escrita de 4 bytes e leitura de 4 bytes


	address = 0;
	byte = 0;

	for(i=0 ; i<4 ; i++){
		if (EEWriteByte(address, input2[byte]) == ERROR)
			printf("Write failed!\n");
		else
			printf("Successfull Write of char %c to address %#04x\n",input2[byte],address);
		byte++;
		address++;
	}

	address=0;
	byte=0;

	for(i=0 ; i<4 ; i++){
		if (EEReadByte(address, &u8ebyte) != ERROR)
			printf("Successfull Read of char %c from address %#04x\n",u8ebyte,address);
		else
			printf("Read failed\n");
		byte++;
		address++;
	}
*/
}


