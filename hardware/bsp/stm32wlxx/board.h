#ifndef __BOARD_H
#define __BOARD_H

#include "stm32wlxx_hal.h"
#include "spi.h"
#include "flash.h"
#include "gpio.h"
#include "system.h"

void boot_init(void);
void Error_Handler(void);

#endif /* __BOARD_H */