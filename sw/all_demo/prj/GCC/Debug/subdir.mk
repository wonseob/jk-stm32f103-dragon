################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/adc.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/at24c02.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/buzzer.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/can.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/dac.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/diskio.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/ff.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/font.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/hw_config.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/icon.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/iic.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/key.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd12864.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd1602.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd28.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd32.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd43.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd70.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/led.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/main.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/mmc_sd.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/newlib_stubs.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/nrf24l01.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/queue.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/rs485.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/rtc.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/spi.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/stm32f10x_it.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/timer.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/touch.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usart.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_desc.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_istr.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_prop.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_pwr.c \
D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/vs1003b.c 

OBJS += \
./adc.o \
./at24c02.o \
./buzzer.o \
./can.o \
./dac.o \
./diskio.o \
./ff.o \
./font.o \
./hw_config.o \
./icon.o \
./iic.o \
./key.o \
./lcd.o \
./lcd12864.o \
./lcd1602.o \
./lcd28.o \
./lcd32.o \
./lcd43.o \
./lcd70.o \
./led.o \
./main.o \
./mmc_sd.o \
./newlib_stubs.o \
./nrf24l01.o \
./queue.o \
./rs485.o \
./rtc.o \
./spi.o \
./stm32f10x_it.o \
./timer.o \
./touch.o \
./usart.o \
./usb_desc.o \
./usb_istr.o \
./usb_prop.o \
./usb_pwr.o \
./vs1003b.o 

C_DEPS += \
./adc.d \
./at24c02.d \
./buzzer.d \
./can.d \
./dac.d \
./diskio.d \
./ff.d \
./font.d \
./hw_config.d \
./icon.d \
./iic.d \
./key.d \
./lcd.d \
./lcd12864.d \
./lcd1602.d \
./lcd28.d \
./lcd32.d \
./lcd43.d \
./lcd70.d \
./led.d \
./main.d \
./mmc_sd.d \
./newlib_stubs.d \
./nrf24l01.d \
./queue.d \
./rs485.d \
./rtc.d \
./spi.d \
./stm32f10x_it.d \
./timer.d \
./touch.d \
./usart.d \
./usb_desc.d \
./usb_istr.d \
./usb_prop.d \
./usb_pwr.d \
./vs1003b.d 


# Each subdirectory must supply rules for building sources it contributes
adc.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

at24c02.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/at24c02.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

buzzer.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/buzzer.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

can.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/can.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

dac.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/dac.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

diskio.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/diskio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ff.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/ff.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

font.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/font.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hw_config.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/hw_config.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

icon.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/icon.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

iic.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/iic.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

key.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/key.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd12864.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd12864.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd1602.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd1602.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd28.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd28.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd32.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd32.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd43.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd43.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd70.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/lcd70.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

led.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/led.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mmc_sd.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/mmc_sd.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

newlib_stubs.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/newlib_stubs.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

nrf24l01.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/nrf24l01.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

queue.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/queue.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

rs485.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/rs485.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

rtc.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/rtc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

spi.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f10x_it.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/stm32f10x_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

timer.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

touch.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/touch.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

usart.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_desc.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_desc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_istr.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_istr.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_prop.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_prop.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_pwr.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/usb_pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vs1003b.o: D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/vs1003b.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\CMSIS\Device\ST\STM32F10x\Include" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32_USB-FS-Device_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib\STM32_USB-FS-Device_Lib_V3.4.0\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../all_demo" -I"D:/svn/google_code/jk-stm32f103-dragon/sw/all_demo/prj/GCC/../../../lib/STM32_USB-FS-Device_Lib_V3.4.0/Libraries/CMSIS/Include" -O0 -g3 -Wall -c -v -mthumb -mcpu=cortex-m3 -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


