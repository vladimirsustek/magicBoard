################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nrf24l01p/nrf24l01p_driver.c \
../nrf24l01p/nrf24l01p_driver_B.c \
../nrf24l01p/nrf24l01p_func.c \
../nrf24l01p/nrf24l01p_port.c \
../nrf24l01p/nrf24l01p_port_B.c 

OBJS += \
./nrf24l01p/nrf24l01p_driver.o \
./nrf24l01p/nrf24l01p_driver_B.o \
./nrf24l01p/nrf24l01p_func.o \
./nrf24l01p/nrf24l01p_port.o \
./nrf24l01p/nrf24l01p_port_B.o 

C_DEPS += \
./nrf24l01p/nrf24l01p_driver.d \
./nrf24l01p/nrf24l01p_driver_B.d \
./nrf24l01p/nrf24l01p_func.d \
./nrf24l01p/nrf24l01p_port.d \
./nrf24l01p/nrf24l01p_port_B.d 


# Each subdirectory must supply rules for building sources it contributes
nrf24l01p/%.o: ../nrf24l01p/%.c nrf24l01p/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../nrf24l01p -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-nrf24l01p

clean-nrf24l01p:
	-$(RM) ./nrf24l01p/nrf24l01p_driver.d ./nrf24l01p/nrf24l01p_driver.o ./nrf24l01p/nrf24l01p_driver_B.d ./nrf24l01p/nrf24l01p_driver_B.o ./nrf24l01p/nrf24l01p_func.d ./nrf24l01p/nrf24l01p_func.o ./nrf24l01p/nrf24l01p_port.d ./nrf24l01p/nrf24l01p_port.o ./nrf24l01p/nrf24l01p_port_B.d ./nrf24l01p/nrf24l01p_port_B.o

.PHONY: clean-nrf24l01p

