################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/st7735/st7735.c 

OBJS += \
./Drivers/BSP/Components/st7735/st7735.o 

C_DEPS += \
./Drivers/BSP/Components/st7735/st7735.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/st7735/%.o Drivers/BSP/Components/st7735/%.su Drivers/BSP/Components/st7735/%.cyclo: ../Drivers/BSP/Components/st7735/%.c Drivers/BSP/Components/st7735/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F746xx -DSTM32 -DSTM32F746NGHx -DSTM32F746G_DISCO -DSTM32F7 -c -I../Inc -I"D:/Users/user/Documents/Template_TP_DSP/Template_TP_DSP/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"D:/Users/user/Documents/Template_TP_DSP/Template_TP_DSP/Drivers/CMSIS/Include" -I"D:/Users/user/Documents/Template_TP_DSP/Template_TP_DSP/Drivers/BSP/STM32746G-Discovery" -I"D:/Users/user/Documents/Template_TP_DSP/Template_TP_DSP/Drivers/BSP/Fonts" -I"D:/Users/user/Documents/Template_TP_DSP/Template_TP_DSP/Drivers/STM32F7xx_HAL_Driver" -I"D:/Users/user/Documents/Template_TP_DSP/Template_TP_DSP/Drivers/STM32F7xx_HAL_Driver/Inc" -I"D:/Users/user/Documents/Template_TP_DSP/Template_TP_DSP/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-st7735

clean-Drivers-2f-BSP-2f-Components-2f-st7735:
	-$(RM) ./Drivers/BSP/Components/st7735/st7735.cyclo ./Drivers/BSP/Components/st7735/st7735.d ./Drivers/BSP/Components/st7735/st7735.o ./Drivers/BSP/Components/st7735/st7735.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-st7735

