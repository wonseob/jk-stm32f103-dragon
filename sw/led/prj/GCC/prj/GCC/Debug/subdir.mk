################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/svn/google_code/jk-stm32f4-dragon/sw/led/main.c \
D:/svn/google_code/jk-stm32f4-dragon/sw/led/stm32f4xx_it.c 

OBJS += \
./main.o \
./stm32f4xx_it.o 

C_DEPS += \
./main.d \
./stm32f4xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
main.o: D:/svn/google_code/jk-stm32f4-dragon/sw/led/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F4XX -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\STM32_USB_Device_Library\Core\inc" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\STM32_USB_Device_Library\Class\hid\inc" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../led" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m4 -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4xx_it.o: D:/svn/google_code/jk-stm32f4-dragon/sw/led/stm32f4xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F4XX -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\STM32_USB_Device_Library\Core\inc" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\STM32_USB_Device_Library\Class\hid\inc" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib\STM32_USB-Host-Device_Lib_V2.1.0\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../led" -I"D:/svn/google_code/jk-stm32f4-dragon/sw/led/prj/GCC/../../../lib/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m4 -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


