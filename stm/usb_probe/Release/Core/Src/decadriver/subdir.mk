################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/decadriver/deca_device.c \
../Core/Src/decadriver/deca_params_init.c 

OBJS += \
./Core/Src/decadriver/deca_device.o \
./Core/Src/decadriver/deca_params_init.o 

C_DEPS += \
./Core/Src/decadriver/deca_device.d \
./Core/Src/decadriver/deca_params_init.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/decadriver/%.o Core/Src/decadriver/%.su: ../Core/Src/decadriver/%.c Core/Src/decadriver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-decadriver

clean-Core-2f-Src-2f-decadriver:
	-$(RM) ./Core/Src/decadriver/deca_device.d ./Core/Src/decadriver/deca_device.o ./Core/Src/decadriver/deca_device.su ./Core/Src/decadriver/deca_params_init.d ./Core/Src/decadriver/deca_params_init.o ./Core/Src/decadriver/deca_params_init.su

.PHONY: clean-Core-2f-Src-2f-decadriver

