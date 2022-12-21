################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../25AA1024/Src/eeprom_25aa1024.c 

OBJS += \
./25AA1024/Src/eeprom_25aa1024.o 

C_DEPS += \
./25AA1024/Src/eeprom_25aa1024.d 


# Each subdirectory must supply rules for building sources it contributes
25AA1024/Src/%.o: ../25AA1024/Src/%.c 25AA1024/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../rda5807m -I../esp8266 -I../nrf24l01p -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/esp8266/Inc" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/esp8266/Src" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/25AA1024/Inc" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/25AA1024/Src" -I../25AA1024 -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/rda5807m/Inc" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/rda5807m/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-25AA1024-2f-Src

clean-25AA1024-2f-Src:
	-$(RM) ./25AA1024/Src/eeprom_25aa1024.d ./25AA1024/Src/eeprom_25aa1024.o

.PHONY: clean-25AA1024-2f-Src

