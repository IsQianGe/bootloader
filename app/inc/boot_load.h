#ifndef __BOOT_LOAD_H
#define __BOOT_LOAD_H

/* Exported types ------------------------------------------------------------*/
/**
  * Local loader status Type Definition
  */
typedef enum
{
  BOOT_LOADER_OK                            = 0x00U,
  BOOT_LOADER_ERR_COM                       = 0x01U,
  BOOT_LOADER_ERR_AUTH_FAILED               = 0x02U,
  BOOT_LOADER_ERR_FW_LENGTH                 = 0x03U,
  BOOT_LOADER_ERR_FW_VERSION                = 0x04U,
  BOOT_LOADER_ERR_FLASH                     = 0x05U,
  BOOT_LOADER_ERR_CRYPTO                    = 0x06U
} BOOT_LOADER_StatusTypeDef;

#endif /* __BOOT_LOAD_H */