################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include prj/MDK-ARMv4/subdir.mk
-include prj/GCC/Debug/subdir.mk
-include prj/GCC/Debug/Libraries/STM32_USB-FS-Device_Driver/src/subdir.mk
-include prj/GCC/Debug/Libraries/STM32F10x_StdPeriph_Driver/src/subdir.mk
-include prj/GCC/Debug/Libraries/CMSIS/Device/ST/STM32F10x/Source/Templates/subdir.mk
-include prj/GCC/Debug/Libraries/CMSIS/Device/ST/STM32F10x/Source/Templates/TrueSTUDIO/subdir.mk
-include STM32F10x_StdPeriph_Driver/src/subdir.mk
-include CMSIS/Device/ST/STM32F10x/Source/Templates/subdir.mk
-include CMSIS/Device/ST/STM32F10x/Source/Templates/TrueSTUDIO/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: stm32f103_dragon_led.elf

# Tool invocations
stm32f103_dragon_led.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-none-eabi-gcc -T "D:/svn/google_code/jk-stm32f103-dragon/sw/led/prj/GCC\stm32f103_hd_flash.ld" -mthumb -mcpu=cortex-m3 -Wl,-Map=linker.map -Wl,-cref -Wl,--gc-sections -o"stm32f103_dragon_led.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) stm32f103_dragon_led.elf
	-@echo ' '

post-build:
	-@echo 'Bin Create'
	-arm-none-eabi-objcopy -S  -O binary  "stm32f103_dragon_led.elf" "stm32f103_dragon_led.bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
