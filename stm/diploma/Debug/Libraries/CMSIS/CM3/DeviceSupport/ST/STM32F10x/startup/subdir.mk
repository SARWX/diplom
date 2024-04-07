################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/startup_stm32f10x_cl.S 

OBJS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/startup_stm32f10x_cl.o 

S_UPPER_DEPS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/startup_stm32f10x_cl.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/%.o: ../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/%.S Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -DSTM32F10X_MD -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Libraries-2f-CMSIS-2f-CM3-2f-DeviceSupport-2f-ST-2f-STM32F10x-2f-startup

clean-Libraries-2f-CMSIS-2f-CM3-2f-DeviceSupport-2f-ST-2f-STM32F10x-2f-startup:
	-$(RM) ./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/startup_stm32f10x_cl.d ./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/startup_stm32f10x_cl.o

.PHONY: clean-Libraries-2f-CMSIS-2f-CM3-2f-DeviceSupport-2f-ST-2f-STM32F10x-2f-startup

