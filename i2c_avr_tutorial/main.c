#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define BLINK_DELAY_MS 400
#define EEDEVADR 0b111
#define EEMAXADDR 0x0800
#define ERROR 1
#define SUCCESS (!ERROR)
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
/*
uint8_t TWIReadACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}
//read byte with NACK
uint8_t TWIReadNACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

void TWIInit(void)
{
    //set SCL to 400kHz
    TWSR = 0x00;
    TWBR = 0x0C;
    //enable TWI
    TWCR = (1<<TWEN);
}

uint8_t TWIGetStatus(void)
{
    uint8_t status;
    //mask status
    status = TWSR & 0xF8;
    return status;
}

void TWIStart(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}
//send stop signal

void TWIStop(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWIWrite(uint8_t u8data)
{
    TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

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

uint8_t EEWritePage(uint8_t page, uint8_t *u8data)
{
    //calculate page address
    uint8_t u8paddr = 0;
    uint8_t i;
    u8paddr = page<<4;
    TWIStart();
    if (TWIGetStatus() != 0x08)
        return ERROR;
    //select page start address and send A2 A1 A0 bits send write command
    TWIWrite(((EEDEVADR)|(u8paddr>>3))&(~1));
    if (TWIGetStatus() != 0x18)
        return ERROR;
    //send the rest of address
    TWIWrite((u8paddr<<4));
    if (TWIGetStatus() != 0x28)
        return ERROR;
    //write page to eeprom
    for (i=0; i<16; i++)
    {
        TWIWrite(*u8data++);
            if (TWIGetStatus() != 0x28)
                return ERROR;
    }
    TWIStop();
    return SUCCESS;
}
uint8_t EEReadPage(uint8_t page, uint8_t *u8data)
{
    //calculate page address
    uint8_t u8paddr = 0;
    uint8_t i;
    u8paddr = page<<4;
    TWIStart();
    if (TWIGetStatus() != 0x08)
        return ERROR;
    //select page start address and send A2 A1 A0 bits send write command
    TWIWrite(((EEDEVADR)|(u8paddr>>3))&(~1));
    if (TWIGetStatus() != 0x18)
        return ERROR;
    //send the rest of address
    TWIWrite((u8paddr<<4));
    if (TWIGetStatus() != 0x28)
        return ERROR;
    //send start
    TWIStart();
    if (TWIGetStatus() != 0x10)
        return ERROR;
    //select devise and send read bit
    TWIWrite(((EEDEVADR)|(u8paddr>>3))|1);
    if (TWIGetStatus() != 0x40)
        return ERROR;
    for (i=0; i<15; i++)
    {
        *u8data++ = TWIReadACK();
            if (TWIGetStatus() != 0x50)
                return ERROR;
    }
    *u8data = TWIReadNACK();
    if (TWIGetStatus() != 0x58)
        return ERROR;
    TWIStop();
    return SUCCESS;
}

int main()
{
    uint8_t u8ebyte;
    uint8_t u8erbyte;
    uint16_t u16eaddress = 0x07F0;
    uint8_t page = 5;
    uint8_t i;
    uint8_t eereadpage[16];
    uint8_t eewritepage[16] = { 10, 44, 255, 46, 80, 87, 43, 130,
                                210, 23, 1, 58, 46, 150, 12, 46 };
//Initialize USART0
USART0Init();
//
TWIInit();
//assign our stream to standard I/O streams
stdin=stdout=&usart0_str;
printf("\nWrite byte %#04x to eeprom address %#04x", 0x58, u16eaddress);
if (EEWriteByte(u16eaddress, 0x58) != ERROR)
{
    printf_P(PSTR("\nRead byte From eeprom"));
    if (EEReadByte(u16eaddress, &u8ebyte) != ERROR)
    {
        printf("\n*%#04x = %#04x", u16eaddress, u8ebyte);
    }
    else printf_P(PSTR("\nStatus fail!"));

}
else printf_P(PSTR("\nStatus fail!"));

printf_P(PSTR("\nWriting 16 bytes to page 5 "));
if(EEWritePage(page, eewritepage) != ERROR)
{
    printf_P(PSTR("\nReading 16 bytes from page 5 "));
    if (EEReadPage(page, eereadpage) != ERROR)
    {
        //compare send and read buffers
        for (i=0; i<16; i++)
        {
            if (eereadpage[i] != eewritepage[i])
            {
                break;
            }
                else continue;
        }
        if (i==16)
            printf_P(PSTR("\nPage write and read success!"));
        else
            printf_P(PSTR("\nPage write and read fail!"));
    } else printf_P(PSTR("\nStatus fail!"));

}else printf_P(PSTR("\nStatus fail!"));

printf_P(PSTR("\nContinue testing EEPROM from terminal!"));
    while(1)
    {
        printf("\nEnter EEPROM address to write (MAX = %u): ", EEMAXADDR);
        scanf("%u",&u16eaddress);
        printf("Enter data to write to EEPROM at address %u: ", u16eaddress);
        scanf("%u",&u8ebyte);
        printf_P(PSTR("\nWriting..."));
        EEWriteByte(u16eaddress, u8ebyte);
        printf_P(PSTR("\nTesting..."));
        if (EEReadByte(u16eaddress, &u8erbyte) !=ERROR)
            {
                if (u8ebyte==u8erbyte)
                    printf_P(PSTR("\nSuccess!"));
                else
                    printf_P(PSTR("\nFail!"));
            }
            else printf_P(PSTR("\nStatus fail!"));

        //TODO:: Please write your application code
    }
}*/
