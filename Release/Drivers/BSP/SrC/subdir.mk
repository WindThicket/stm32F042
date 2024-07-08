################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/SrC/24cxx.c 

OBJS += \
./Drivers/BSP/SrC/24cxx.o 

C_DEPS += \
./Drivers/BSP/SrC/24cxx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/SrC/%.o Drivers/BSP/SrC/%.su Drivers/BSP/SrC/%.cyclo: ../Drivers/BSP/SrC/%.c Drivers/BSP/SrC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-SrC

clean-Drivers-2f-BSP-2f-SrC:
	-$(RM) ./Drivers/BSP/SrC/24cxx.cyclo ./Drivers/BSP/SrC/24cxx.d ./Drivers/BSP/SrC/24cxx.o ./Drivers/BSP/SrC/24cxx.su

.PHONY: clean-Drivers-2f-BSP-2f-SrC

