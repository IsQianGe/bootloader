
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

#define LOADER_REGION_ROM_START 0x80200000
typedef enum
{
  BOOT_ERROR = 0x01U,
  BOOT_SUCCESS = 0x00U
} BOOT_ErrorStatus;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
