################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../decadriver/deca_device.c \
../decadriver/deca_params_init.c 

OBJS += \
./decadriver/deca_device.o \
./decadriver/deca_params_init.o 

C_DEPS += \
./decadriver/deca_device.d \
./decadriver/deca_params_init.d 


# Each subdirectory must supply rules for building sources it contributes
decadriver/%.o decadriver/%.su: ../decadriver/%.c decadriver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DUSE_STDPERIPH_DRIVER -DDEBUG -DSTM32F1 -DSTM32F103C8Tx -c -I"D:/programming/diplom/stm/diploma/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"C:/OTHER/programming/diplom/stm/diploma/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/CoreSupport" -I"C:/OTHER/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/CoreSupport" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x" -I"C:/OTHER/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x" -I../Inc -I"D:/programming/diplom/stm/diploma/platform" -I"C:/OTHER/programming/diplom/stm/diploma/platform" -I"D:/programming/diplom/stm/diploma/decadriver" -I"C:/OTHER/programming/diplom/stm/diploma/decadriver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-decadriver

clean-decadriver:
	-$(RM) ./decadriver/deca_device.d ./decadriver/deca_device.o ./decadriver/deca_device.su ./decadriver/deca_params_init.d ./decadriver/deca_params_init.o ./decadriver/deca_params_init.su

.PHONY: clean-decadriver

