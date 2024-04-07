################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DUSE_STDPERIPH_DRIVER -DDEBUG -DSTM32F1 -DSTM32F103C8Tx -c -I"D:/programming/diplom/stm/diploma/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"C:/OTHER/programming/diplom/stm/diploma/Libraries/STM32F10x_StdPeriph_Driver/inc" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/CoreSupport" -I"C:/OTHER/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/CoreSupport" -I"D:/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x" -I"C:/OTHER/programming/diplom/stm/diploma/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x" -I../Inc -I"D:/programming/diplom/stm/diploma/platform" -I"C:/OTHER/programming/diplom/stm/diploma/platform" -I"D:/programming/diplom/stm/diploma/decadriver" -I"C:/OTHER/programming/diplom/stm/diploma/decadriver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

