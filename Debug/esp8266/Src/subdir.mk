################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../esp8266/Src/esp8266_functions.c \
../esp8266/Src/esp8266_http_data.c \
../esp8266/Src/esp8266_http_server.c \
../esp8266/Src/esp8266_port.c \
../esp8266/Src/esp8266_utils.c 

OBJS += \
./esp8266/Src/esp8266_functions.o \
./esp8266/Src/esp8266_http_data.o \
./esp8266/Src/esp8266_http_server.o \
./esp8266/Src/esp8266_port.o \
./esp8266/Src/esp8266_utils.o 

C_DEPS += \
./esp8266/Src/esp8266_functions.d \
./esp8266/Src/esp8266_http_data.d \
./esp8266/Src/esp8266_http_server.d \
./esp8266/Src/esp8266_port.d \
./esp8266/Src/esp8266_utils.d 


# Each subdirectory must supply rules for building sources it contributes
esp8266/Src/%.o: ../esp8266/Src/%.c esp8266/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../rda5807m -I../esp8266 -I../nrf24l01p -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/esp8266/Inc" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/esp8266/Src" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/25AA1024/Inc" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/25AA1024/Src" -I../25AA1024 -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/rda5807m/Inc" -I"C:/Users/42077/STM32CubeIDE/workspace_1.8.0/magicBoard_v0/rda5807m/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-esp8266-2f-Src

clean-esp8266-2f-Src:
	-$(RM) ./esp8266/Src/esp8266_functions.d ./esp8266/Src/esp8266_functions.o ./esp8266/Src/esp8266_http_data.d ./esp8266/Src/esp8266_http_data.o ./esp8266/Src/esp8266_http_server.d ./esp8266/Src/esp8266_http_server.o ./esp8266/Src/esp8266_port.d ./esp8266/Src/esp8266_port.o ./esp8266/Src/esp8266_utils.d ./esp8266/Src/esp8266_utils.o

.PHONY: clean-esp8266-2f-Src

