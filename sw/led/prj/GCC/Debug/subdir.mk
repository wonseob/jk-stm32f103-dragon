################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/svn/google_code/jk-stm32f103-dragon/sw/led/hw_config.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/led/main.c 

OBJS += \
./hw_config.o \
./main.o 

C_DEPS += \
./hw_config.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
hw_config.o: D:/svn/google_code/jk-stm32f103-dragon/sw/led/hw_config.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../led" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: D:/svn/google_code/jk-stm32f103-dragon/sw/led/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../led" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


