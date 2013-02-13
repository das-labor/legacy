# source files.
SRC =  	./src/stm32f4xx_adc.c ./src/stm32f4xx_can.c ./src/stm32f4xx_crc.c ./src/stm32f4xx_cryp_aes.c ./src/stm32f4xx_cryp.c ./src/stm32f4xx_cryp_des.c ./src/stm32f4xx_cryp_tdes.c ./src/stm32f4xx_dac.c ./src/stm32f4xx_dbgmcu.c ./src/stm32f4xx_dcmi.c ./src/stm32f4xx_dma.c \
        ./src/stm32f4xx_exti.c ./src/stm32f4xx_flash.c ./src/stm32f4xx_fsmc.c ./src/stm32f4xx_gpio.c ./src/stm32f4xx_hash.c ./src/stm32f4xx_hash_md5.c ./src/stm32f4xx_hash_sha1.c ./src/stm32f4xx_i2c.c ./src/stm32f4xx_iwdg.c \
        ./src/stm32f4xx_pwr.c ./src/stm32f4xx_rcc.c ./src/stm32f4xx_rng.c ./src/stm32f4xx_rtc.c ./src/stm32f4xx_sdio.c ./src/stm32f4xx_spi.c ./src/stm32f4xx_syscfg.c ./src/stm32f4xx_tim.c ./src/stm32f4xx_usart.c ./src/stm32f4xx_wwdg.c \
        ./src/misc.c
OBJ = $(SRC:.c=.o)

OUT = ./libSTM32F4xx_StdPeriph_Driver.a

# include directories
INCLUDES = -I. -I./inc

# C+ compiler flags (-g -O2 -Wall)
CFLAGS=-g -O2 -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib
CFLAGS+=-I../CMSIS/ST/STM32F4xx/Include
CFLAGS+=-I../CMSIS/Include

# compiler
CC = arm-none-eabi-gcc

# library paths
LIBS = -L../ -lm

# compile flags
LDFLAGS = -g

.SUFFIXES: .c

default: dep $(OUT)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJ) $(OUT) Makefile.bak 
# DO NOT DELETE

./src/stm32f4xx_adc.o: ./inc/stm32f4xx_adc.h stm32f4xx.h
./src/stm32f4xx_adc.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_adc.o: /usr/include/features.h
./src/stm32f4xx_adc.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_adc.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_adc.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_adc.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_adc.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_can.o: ./inc/stm32f4xx_can.h stm32f4xx.h
./src/stm32f4xx_can.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_can.o: /usr/include/features.h
./src/stm32f4xx_can.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_can.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_can.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_can.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_can.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_crc.o: ./inc/stm32f4xx_crc.h stm32f4xx.h
./src/stm32f4xx_crc.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_crc.o: /usr/include/features.h
./src/stm32f4xx_crc.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_crc.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_crc.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_crc.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_cryp_aes.o: ./inc/stm32f4xx_cryp.h stm32f4xx.h
./src/stm32f4xx_cryp_aes.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_cryp_aes.o: /usr/include/features.h
./src/stm32f4xx_cryp_aes.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_cryp_aes.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_cryp_aes.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_cryp_aes.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_cryp.o: ./inc/stm32f4xx_cryp.h stm32f4xx.h
./src/stm32f4xx_cryp.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_cryp.o: /usr/include/features.h
./src/stm32f4xx_cryp.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_cryp.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_cryp.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_cryp.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_cryp.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_cryp_des.o: ./inc/stm32f4xx_cryp.h stm32f4xx.h
./src/stm32f4xx_cryp_des.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_cryp_des.o: /usr/include/features.h
./src/stm32f4xx_cryp_des.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_cryp_des.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_cryp_des.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_cryp_des.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_cryp_tdes.o: ./inc/stm32f4xx_cryp.h stm32f4xx.h
./src/stm32f4xx_cryp_tdes.o: ../CMSIS/Include/core_cm4.h
./src/stm32f4xx_cryp_tdes.o: /usr/include/stdint.h /usr/include/features.h
./src/stm32f4xx_cryp_tdes.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_cryp_tdes.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_cryp_tdes.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_cryp_tdes.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_dac.o: ./inc/stm32f4xx_dac.h stm32f4xx.h
./src/stm32f4xx_dac.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_dac.o: /usr/include/features.h
./src/stm32f4xx_dac.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_dac.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_dac.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_dac.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_dac.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_dbgmcu.o: ./inc/stm32f4xx_dbgmcu.h stm32f4xx.h
./src/stm32f4xx_dbgmcu.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_dbgmcu.o: /usr/include/features.h
./src/stm32f4xx_dbgmcu.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_dbgmcu.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_dbgmcu.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_dbgmcu.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_dcmi.o: ./inc/stm32f4xx_dcmi.h stm32f4xx.h
./src/stm32f4xx_dcmi.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_dcmi.o: /usr/include/features.h
./src/stm32f4xx_dcmi.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_dcmi.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_dcmi.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_dcmi.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_dcmi.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_dma.o: ./inc/stm32f4xx_dma.h stm32f4xx.h
./src/stm32f4xx_dma.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_dma.o: /usr/include/features.h
./src/stm32f4xx_dma.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_dma.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_dma.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_dma.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_dma.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_exti.o: ./inc/stm32f4xx_exti.h stm32f4xx.h
./src/stm32f4xx_exti.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_exti.o: /usr/include/features.h
./src/stm32f4xx_exti.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_exti.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_exti.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_exti.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_flash.o: ./inc/stm32f4xx_flash.h stm32f4xx.h
./src/stm32f4xx_flash.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_flash.o: /usr/include/features.h
./src/stm32f4xx_flash.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_flash.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_flash.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_flash.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_fsmc.o: ./inc/stm32f4xx_fsmc.h stm32f4xx.h
./src/stm32f4xx_fsmc.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_fsmc.o: /usr/include/features.h
./src/stm32f4xx_fsmc.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_fsmc.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_fsmc.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_fsmc.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_fsmc.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_gpio.o: ./inc/stm32f4xx_gpio.h stm32f4xx.h
./src/stm32f4xx_gpio.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_gpio.o: /usr/include/features.h
./src/stm32f4xx_gpio.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_gpio.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_gpio.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_gpio.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_gpio.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_hash.o: ./inc/stm32f4xx_hash.h stm32f4xx.h
./src/stm32f4xx_hash.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_hash.o: /usr/include/features.h
./src/stm32f4xx_hash.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_hash.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_hash.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_hash.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_hash.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_hash_md5.o: ./inc/stm32f4xx_hash.h stm32f4xx.h
./src/stm32f4xx_hash_md5.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_hash_md5.o: /usr/include/features.h
./src/stm32f4xx_hash_md5.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_hash_md5.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_hash_md5.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_hash_md5.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_hash_sha1.o: ./inc/stm32f4xx_hash.h stm32f4xx.h
./src/stm32f4xx_hash_sha1.o: ../CMSIS/Include/core_cm4.h
./src/stm32f4xx_hash_sha1.o: /usr/include/stdint.h /usr/include/features.h
./src/stm32f4xx_hash_sha1.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_hash_sha1.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_hash_sha1.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_hash_sha1.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_i2c.o: ./inc/stm32f4xx_i2c.h stm32f4xx.h
./src/stm32f4xx_i2c.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_i2c.o: /usr/include/features.h
./src/stm32f4xx_i2c.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_i2c.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_i2c.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_i2c.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_i2c.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_iwdg.o: ./inc/stm32f4xx_iwdg.h stm32f4xx.h
./src/stm32f4xx_iwdg.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_iwdg.o: /usr/include/features.h
./src/stm32f4xx_iwdg.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_iwdg.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_iwdg.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_iwdg.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_pwr.o: ./inc/stm32f4xx_pwr.h stm32f4xx.h
./src/stm32f4xx_pwr.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_pwr.o: /usr/include/features.h
./src/stm32f4xx_pwr.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_pwr.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_pwr.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_pwr.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_pwr.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_rcc.o: ./inc/stm32f4xx_rcc.h stm32f4xx.h
./src/stm32f4xx_rcc.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_rcc.o: /usr/include/features.h
./src/stm32f4xx_rcc.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_rcc.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_rcc.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_rcc.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_rng.o: ./inc/stm32f4xx_rng.h stm32f4xx.h
./src/stm32f4xx_rng.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_rng.o: /usr/include/features.h
./src/stm32f4xx_rng.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_rng.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_rng.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_rng.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_rng.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_rtc.o: ./inc/stm32f4xx_rtc.h stm32f4xx.h
./src/stm32f4xx_rtc.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_rtc.o: /usr/include/features.h
./src/stm32f4xx_rtc.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_rtc.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_rtc.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_rtc.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_rtc.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_sdio.o: ./inc/stm32f4xx_sdio.h stm32f4xx.h
./src/stm32f4xx_sdio.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_sdio.o: /usr/include/features.h
./src/stm32f4xx_sdio.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_sdio.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_sdio.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_sdio.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_sdio.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_spi.o: ./inc/stm32f4xx_spi.h stm32f4xx.h
./src/stm32f4xx_spi.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_spi.o: /usr/include/features.h
./src/stm32f4xx_spi.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_spi.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_spi.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_spi.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_spi.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_syscfg.o: ./inc/stm32f4xx_syscfg.h stm32f4xx.h
./src/stm32f4xx_syscfg.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_syscfg.o: /usr/include/features.h
./src/stm32f4xx_syscfg.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_syscfg.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_syscfg.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_syscfg.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_syscfg.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_tim.o: ./inc/stm32f4xx_tim.h stm32f4xx.h
./src/stm32f4xx_tim.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_tim.o: /usr/include/features.h
./src/stm32f4xx_tim.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_tim.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_tim.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_tim.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_tim.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_usart.o: ./inc/stm32f4xx_usart.h stm32f4xx.h
./src/stm32f4xx_usart.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_usart.o: /usr/include/features.h
./src/stm32f4xx_usart.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_usart.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_usart.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_usart.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_usart.o: ./inc/stm32f4xx_rcc.h
./src/stm32f4xx_wwdg.o: ./inc/stm32f4xx_wwdg.h stm32f4xx.h
./src/stm32f4xx_wwdg.o: ../CMSIS/Include/core_cm4.h /usr/include/stdint.h
./src/stm32f4xx_wwdg.o: /usr/include/features.h
./src/stm32f4xx_wwdg.o: ../CMSIS/Include/core_cmInstr.h
./src/stm32f4xx_wwdg.o: ../CMSIS/Include/core_cmFunc.h
./src/stm32f4xx_wwdg.o: ../CMSIS/Include/core_cm4_simd.h
./src/stm32f4xx_wwdg.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
./src/stm32f4xx_wwdg.o: ./inc/stm32f4xx_rcc.h
./src/misc.o: ./inc/misc.h stm32f4xx.h ../CMSIS/Include/core_cm4.h
./src/misc.o: /usr/include/stdint.h /usr/include/features.h
./src/misc.o: ../CMSIS/Include/core_cmInstr.h ../CMSIS/Include/core_cmFunc.h
./src/misc.o: ../CMSIS/Include/core_cm4_simd.h
./src/misc.o: ../CMSIS/ST/STM32F4xx/Include/system_stm32f4xx.h
