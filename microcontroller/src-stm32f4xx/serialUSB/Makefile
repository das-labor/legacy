
LIB_PATH=./Libraries

EXECUTABLE=STM32F4-Discovery_Demo.elf
BIN_IMAGE=STM32F4-Discovery_Demo.bin

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-g -O2 -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib

#usb_conf.h
CFLAGS+=-DUSE_USB_OTG_FS=1

#assert
CFLAGS+=-DUSE_FULL_ASSERT=1

# to run from FLASH
CFLAGS+=-Wl,-T,stm32_flash.ld

#Current directory
CFLAGS+=-I.

#CMSIS Lib
CFLAGS+=-I$(LIB_PATH)/CMSIS/ST/STM32F4xx/Include
CFLAGS+=-I$(LIB_PATH)/CMSIS/Include

# stm32f4_discovery lib
CFLAGS+=-I$(LIB_PATH)/STM32F4xx_StdPeriph_Driver/inc
CFLAGS+=-I$(LIB_PATH)/STM32F4xx_StdPeriph_Driver/inc/device_support
CFLAGS+=-I$(LIB_PATH)/STM32F4xx_StdPeriph_Driver/inc/core_support

#STM32_USB_Device_Library
CFLAGS+=-I$(LIB_PATH)/STM32_USB_Device_Library/Class/cdc/inc
CFLAGS+=-I$(LIB_PATH)/STM32_USB_Device_Library/Core/inc

#STM32_USB_OTG_Driver
CFLAGS+=-I$(LIB_PATH)/STM32_USB_OTG_Driver/inc

#Utilities
CFLAGS+=-I$(STM_ROOT)/Utilities/STM32F4-Discovery

# Include standard periph driver
CFLAGS+=-DUSE_STDPERIPH_DRIVER

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

#stm32f4xx_it.c
$(EXECUTABLE): main.c system_stm32f4xx.c startup_stm32f4xx.s stm32fxxx_it.c \
	usb_bsp.c usbd_desc.c usbd_usr.c usb_core.c usbd_cdc_vcp.c\
	stm32f4_discovery.c \
	$(LIB_PATH)/STM32_USB_OTG_Driver/src/usb_dcd_int.c \
	$(LIB_PATH)/STM32_USB_OTG_Driver/src/usb_dcd.c \
	$(LIB_PATH)/STM32_USB_Device_Library/Core/src/usbd_core.c \
	$(LIB_PATH)/STM32_USB_Device_Library/Core/src/usbd_req.c \
	$(LIB_PATH)/STM32_USB_Device_Library/Core/src/usbd_ioreq.c \
	$(LIB_PATH)/STM32_USB_Device_Library/Class/cdc/src/usbd_cdc_core.c
	$(MAKE) -C $(LIB_PATH)/STM32F4xx_StdPeriph_Driver
	$(CC) $(CFLAGS) $^ -o $@ -L$(LIB_PATH)/STM32F4xx_StdPeriph_Driver -lSTM32F4xx_StdPeriph_Driver


clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

.PHONY: all clean

flash: STM32F4-Discovery_Demo.elf
	arm-none-eabi-gdb --command=gdbscript STM32F4-Discovery_Demo.elf
