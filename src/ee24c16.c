/*
 * 24c16.c
 *
 * Created: 1/7/2012 23:19:00
 *  Author: embedds.com
 */ 
#include "ee24c16.h"
//write byte to 24C16
uint8_t EEWriteByte(uint16_t u16addr, uint8_t u8data)
{
	TWIStart();
	if (TWIGetStatus() != 0x08)
		return ERROR;
	//select devise and send A2 A1 A0 address bits
	TWIWrite((EEDEVADR)|(uint8_t)((u16addr & 0x0700)>>7));
	if (TWIGetStatus() != 0x18)
		return ERROR;	
	//send the rest of address
	TWIWrite((uint8_t)(u16addr));
	if (TWIGetStatus() != 0x28)
		return ERROR;
	//write byte to eeprom
	TWIWrite(u8data);
	if (TWIGetStatus() != 0x28)
		return ERROR;
	TWIStop();
	return SUCCESS;
}

// Write a full page (8 bytes)
uint8_t EEReadByte(uint16_t u16addr, uint8_t *u8data)
{
	//uint8_t databyte;
	TWIStart();
	if (TWIGetStatus() != 0x08)
		return ERROR;
	//select devise and send A2 A1 A0 address bits
	TWIWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7)));
	if (TWIGetStatus() != 0x18)
		return ERROR;
	//send the rest of address
	TWIWrite((uint8_t)(u16addr));
	if (TWIGetStatus() != 0x28)
		return ERROR;
	//send start
	TWIStart();
	if (TWIGetStatus() != 0x10)
		return ERROR;
	//select devise and send read bit
	TWIWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7))|1);
	if (TWIGetStatus() != 0x40)
		return ERROR;
	*u8data = TWIReadNACK();
	if (TWIGetStatus() != 0x58)
		return ERROR;
	TWIStop();
	return SUCCESS;
}
