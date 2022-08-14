
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PACKET_1K_SIZE          ((uint32_t)1024U) /*!<Packet 1K Size*/

#define LOADER_REGION_ROM_START 0x80200000
typedef enum
{
  BOOT_ERROR = 0x01U,
  BOOT_SUCCESS = 0x00U
} BOOT_ErrorStatus;

typedef enum
{
  APP_OK = 0,     //App runs normally
  APP_UPDATA,     //App needs to be upgraded
  APP_UPDATA_OK,  //App updata finsh and Reboot
  APP_READY,      //App runs for the first time
} UPDATA_STATUS;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
