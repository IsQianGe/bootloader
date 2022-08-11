BOARD = stm32wlxx
ST_BOARD = wl
DEPS_SUBMODULES += hardware/mcu/st/CMSIS_5 hardware/mcu/st/cmsis_device_$(ST_BOARD) hardware/mcu/st/stm32$(ST_BOARD)xx_hal_driver

ST_CMSIS = hardware/mcu/st/cmsis_device_$(ST_BOARD)
ST_HAL_DRIVER = hardware/mcu/st/stm32$(ST_BOARD)xx_hal_driver
BOARD_PATH = hardware/bsp/$(BOARD)
CMSIS = hardware/mcu/st/CMSIS_5/CMSIS/Core


CFLAGS += \
  -mthumb \
  -mabi=aapcs \
  -mcpu=cortex-m4 \
  -mfloat-abi=soft \
  -nostdlib -nostartfiles \
  -u _printf_float \
  -DCORE_CM4 \
  -DUSE_HAL_DRIVER \
  -USE_FULL_LL_DRIVER


# suppress warning caused by vendor mcu driver
CFLAGS += \
 -Wno-error=maybe-uninitialized \
 -Wno-error=unused-parameter \
 -Wno-error=int-conversion \
 -Wno-error=implicit-fallthrough= \
 -Wno-error=sign-compare

# link script
LDSCRIPT =  $(ST_CMSIS)/Source/Templates/gcc/linker/STM32WLE5XX_FLASH.ld

# ASM sources
ASM_SOURCES +=  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32wle5xx.s

C_SOURCES += 	$(ST_CMSIS)/Source/Templates/system_stm32$(ST_BOARD)xx.c 
C_SOURCES +=  $(filter-out %template.c, $(wildcard $(ST_HAL_DRIVER)/Src/*.c))
C_SOURCES += $(wildcard $(BOARD_PATH)/*.c)

C_INCLUDES += -I$(ST_CMSIS)/Include
C_INCLUDES += -I$(CMSIS)/Include
C_INCLUDES += -I$(ST_HAL_DRIVER)/Inc
C_INCLUDES += -I$(BOARD_PATH)
