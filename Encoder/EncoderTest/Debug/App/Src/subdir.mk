################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/Ease_of_life.c \
../App/Src/HybridVelocity.c \
../App/Src/PulseCountVelocity_object.c \
../App/Src/PulseCountVelocity_static.c \
../App/Src/TimeIntervalVelocity_static.c 

OBJS += \
./App/Src/Ease_of_life.o \
./App/Src/HybridVelocity.o \
./App/Src/PulseCountVelocity_object.o \
./App/Src/PulseCountVelocity_static.o \
./App/Src/TimeIntervalVelocity_static.o 

C_DEPS += \
./App/Src/Ease_of_life.d \
./App/Src/HybridVelocity.d \
./App/Src/PulseCountVelocity_object.d \
./App/Src/PulseCountVelocity_static.d \
./App/Src/TimeIntervalVelocity_static.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/%.o App/Src/%.su App/Src/%.cyclo: ../App/Src/%.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F439xx -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"X:/Projects/Encoder/Encoder_LIB/Encoder/EncoderTest/App/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Src

clean-App-2f-Src:
	-$(RM) ./App/Src/Ease_of_life.cyclo ./App/Src/Ease_of_life.d ./App/Src/Ease_of_life.o ./App/Src/Ease_of_life.su ./App/Src/HybridVelocity.cyclo ./App/Src/HybridVelocity.d ./App/Src/HybridVelocity.o ./App/Src/HybridVelocity.su ./App/Src/PulseCountVelocity_object.cyclo ./App/Src/PulseCountVelocity_object.d ./App/Src/PulseCountVelocity_object.o ./App/Src/PulseCountVelocity_object.su ./App/Src/PulseCountVelocity_static.cyclo ./App/Src/PulseCountVelocity_static.d ./App/Src/PulseCountVelocity_static.o ./App/Src/PulseCountVelocity_static.su ./App/Src/TimeIntervalVelocity_static.cyclo ./App/Src/TimeIntervalVelocity_static.d ./App/Src/TimeIntervalVelocity_static.o ./App/Src/TimeIntervalVelocity_static.su

.PHONY: clean-App-2f-Src

