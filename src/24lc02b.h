/*
 * 24c16.h
 *
 * Created: 1/7/2012 23:19:17
 *  Author: embedds.com
 */ 



#include <avr/io.h>
#include "twi.h"
#define EEDEVADR 0b10100000
#define EEPAGES 128
#define EEPAGESIZE 16
#define EEMAXADDR 0x0800
#define ERROR 1
#define SUCCESS (!ERROR)

uint8_t EEWriteByte(uint16_t u16address, uint8_t u8data);
uint8_t EEReadByte(uint16_t u16address, uint8_t *u8data);

