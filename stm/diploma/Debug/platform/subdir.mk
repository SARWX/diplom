################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/deca_mutex.c \
../platform/deca_sleep.c \
../platform/deca_spi.c \
../platform/lcd.c \
../platform/port.c \
../platform/stm32f10x_it.c \
../platform/syscalls.c 

OBJS += \
./platform/deca_mutex.o \
./platform/deca_sleep.o \
./platform/deca_spi.o \
./platform/lcd.o \
./platform/port.o \
./platform/stm32f10x_it.o \
./platform/syscalls.o 

C_DEPS += \
./platform/deca_mutex.d \
./platform/deca_sleep.d \
./platform/deca_spi.d \
./platform/lcd.d \
./platform/port.d \
./platform/stm32f10x_it.d \
./platform/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
platform/%.o platform/%.su: ../platform/%.c platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"D:/programming/diplom/stm/diploma/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/CoreSupport" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x" -I../Inc -I"D:/programming/diplom/stm/diploma/platform" -I"D:/programming/diplom/stm/diploma/decadriver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-platform

clean-platform:
	-$(RM) ./platform/deca_mutex.d ./platform/deca_mutex.o ./platform/deca_mutex.su ./platform/deca_sleep.d ./platform/deca_sleep.o ./platform/deca_sleep.su ./platform/deca_spi.d ./platform/deca_spi.o ./platform/deca_spi.su ./platform/lcd.d ./platform/lcd.o ./platform/lcd.su ./platform/port.d ./platform/port.o ./platform/port.su ./platform/stm32f10x_it.d ./platform/stm32f10x_it.o ./platform/stm32f10x_it.su ./platform/syscalls.d ./platform/syscalls.o ./platform/syscalls.su

.PHONY: clean-platform

