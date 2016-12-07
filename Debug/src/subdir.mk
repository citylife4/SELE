################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/24lc02b.c \
../src/i2ceeprom.c \
../src/main.c \
../src/twi.c \
../src/usart.c 

OBJS += \
./src/24lc02b.o \
./src/i2ceeprom.o \
./src/main.o \
./src/twi.o \
./src/usart.o 

C_DEPS += \
./src/24lc02b.d \
./src/i2ceeprom.d \
./src/main.d \
./src/twi.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


