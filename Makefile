# Configuration
TOOLCHAIN=arm-none-eabi-
BUILDDIR=build
OPENOCD=/mnt/c/Users/nohous/opt/xpack-openocd-0.10.0-15/bin/openocd.exe

TARGET=template
CPU=-mcpu=cortex-m0
LDSCRIPT=STM32F071VBTx_FLASH.ld
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
OPT=-Os

FREERTOS_SRCS= \
	external/FreeRTOS/list.c \
	external/FreeRTOS/queue.c \
	external/FreeRTOS/tasks.c \
	external/FreeRTOS/portable/GCC/ARM_CM0/port.c
FREERTOS_INCLUDES= \
	-Iexternal/FreeRTOS/include \
	-Iexternal/FreeRTOS/portable/GCC/ARM_CM0/

APP_SOURCES= \
	external/STM32CubeF0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c \
	external/STM32CubeF0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_gpio.c \
	src/main.c
APP_INCLUDES= \
	-Iinclude/ \
	-Iexternal/STM32CubeF0/Drivers/CMSIS/Include \
	-Iexternal/STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Include/ \
	-Iexternal/STM32CubeF0/Drivers/STM32F0xx_HAL_Driver/Inc

AS_DEFS= \
	-DSTM32F071xB
C_DEFS= \
	-DSTM32F071xB \
	-DUSE_FULL_LL_DRIVER

C_SRCS= \
	$(FREERTOS_SRCS)\
	$(APP_SOURCES)
C_INCLUDES= \
	$(FREERTOS_INCLUDES) \
	$(APP_INCLUDES)

AS_SRCS= \
	external/STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f071xb.s
AS_INCLUDES=


LIBS=-lc -lm -lnosys
LIBDIR=

CFLAGS=$(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ASFLAGS=$(MCU) $(AS_DEFS) $(AS_INCLUDES) -Wall -fdata-sections -ffunction-sections

LDFLAGS=$(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILDDIR)/$(OUT).map,--cref -Wl,--gc-sections

# Checks
$(if $(strip $(BUILDDIR)),,$(error BUILDDIR not set))

# Internal
CC=$(TOOLCHAIN)gcc
AS=$(TOOLCHAIN)gcc -x assembler-with-cpp
OBJCOPY=$(TOOLCHAIN)objcopy
SIZE=$(TOOLCHAIN)size

AS_OBJS=$(addprefix $(BUILDDIR)/,$(patsubst %.s,%.o,$(AS_SRCS)))
C_OBJS=$(addprefix $(BUILDDIR)/,$(patsubst %.c,%.o,$(C_SRCS)))

all: $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex $(BUILDDIR)/$(TARGET).bin

.PRECIOUS: $(BUILDDIR)/%.o

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.elf: $(AS_OBJS) $(C_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(BUILDDIR)/%.hex: $(BUILDDIR)/%.elf
	$(OBJCOPY) -O ihex $< $@
	
$(BUILDDIR)/%.bin: $(BUILDDIR)/%.elf
	$(OBJCOPY) -O binary -S $< $@	

clean:
	rm -rf $(BUILDDIR)/*

flash:
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program $(BUILDDIR)/$(TARGET).elf verify reset exit"

reset:
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f0x.cfg -c "reset exit"

unlock:
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f0x.cfg -c "reset halt stm32f0x unlock 0 reset halt"

.PHONY: clean flash reset unlock