################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c 

OBJS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.o 

C_DEPS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/%.o Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/%.su: ../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/%.c Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"D:/programming/diplom/stm/diploma/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/CoreSupport" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x" -I../Inc -I"D:/programming/diplom/stm/diploma/platform" -I"D:/programming/diplom/stm/diploma/decadriver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libraries-2f-CMSIS-2f-CM3-2f-DeviceSupport-2f-ST-2f-STM32F10x

clean-Libraries-2f-CMSIS-2f-CM3-2f-DeviceSupport-2f-ST-2f-STM32F10x:
	-$(RM) ./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.d ./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.o ./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.su

.PHONY: clean-Libraries-2f-CMSIS-2f-CM3-2f-DeviceSupport-2f-ST-2f-STM32F10x

