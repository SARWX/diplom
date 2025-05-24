################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/platform/deca_mutex.c \
../Core/Src/platform/deca_range_tables.c \
../Core/Src/platform/deca_sleep.c \
../Core/Src/platform/deca_spi.c \
../Core/Src/platform/port.c 

OBJS += \
./Core/Src/platform/deca_mutex.o \
./Core/Src/platform/deca_range_tables.o \
./Core/Src/platform/deca_sleep.o \
./Core/Src/platform/deca_spi.o \
./Core/Src/platform/port.o 

C_DEPS += \
./Core/Src/platform/deca_mutex.d \
./Core/Src/platform/deca_range_tables.d \
./Core/Src/platform/deca_sleep.d \
./Core/Src/platform/deca_spi.d \
./Core/Src/platform/port.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/platform/%.o Core/Src/platform/%.su: ../Core/Src/platform/%.c Core/Src/platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-platform

clean-Core-2f-Src-2f-platform:
	-$(RM) ./Core/Src/platform/deca_mutex.d ./Core/Src/platform/deca_mutex.o ./Core/Src/platform/deca_mutex.su ./Core/Src/platform/deca_range_tables.d ./Core/Src/platform/deca_range_tables.o ./Core/Src/platform/deca_range_tables.su ./Core/Src/platform/deca_sleep.d ./Core/Src/platform/deca_sleep.o ./Core/Src/platform/deca_sleep.su ./Core/Src/platform/deca_spi.d ./Core/Src/platform/deca_spi.o ./Core/Src/platform/deca_spi.su ./Core/Src/platform/port.d ./Core/Src/platform/port.o ./Core/Src/platform/port.su

.PHONY: clean-Core-2f-Src-2f-platform

