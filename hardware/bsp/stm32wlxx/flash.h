#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  BOOT_FLASH_ERROR = 0U,       /*!< Error Flash generic*/
  BOOT_FLASH_ERR_HAL,          /*!< Error Flash HAL init */
  BOOT_FLASH_ERR_ERASE,        /*!< Error Flash erase */
  BOOT_FLASH_ERR_WRITING,      /*!< Error writing data in Flash */
  BOOT_FLASH_ERR_WRITINGCTRL,  /*!< Error checking data written in Flash */
  BOOT_FLASH_SUCCESS           /*!< Flash Success */
} BOOT_FLASH_StatusTypeDef;

typedef uint64_t BOOT_FLASH_write_t;

/* Exported constants --------------------------------------------------------*/
/**
  * Length of a MAGIC tag (32 bytes).
  * This must be a multiple of @ref BOOT_FLASH_write_t with a minimum value of 32.
  */
#define MAGIC_LENGTH ((uint32_t)32U)

/* External variables --------------------------------------------------------*/
#define IS_ALIGNED(address) (0U == ((address) % FLASH_PAGE_SIZE))

/* Exported functions ------------------------------------------------------- */
BOOT_ErrorStatus BOOT_FLASH_Init(void);
BOOT_ErrorStatus BOOT_FLASH_Erase_Size(BOOT_FLASH_StatusTypeDef *pxFlashStatus, uint8_t *pStart, uint32_t Length);
BOOT_ErrorStatus BOOT_FLASH_Write(BOOT_FLASH_StatusTypeDef *pxFlashStatus, uint8_t *pDestination,
                                       const uint8_t *pSource, uint32_t Length);
BOOT_ErrorStatus BOOT_FLASH_Read(uint8_t *pDestination, const uint8_t *pSource, uint32_t Length);
BOOT_ErrorStatus BOOT_FLASH_Compare(const uint8_t *pFlash, const uint32_t Pattern1, const uint32_t Pattern2, uint32_t Length);
uint32_t BOOT_FLASH_GetPage(uint32_t Addr);
void NMI_Handler(void);
void HardFault_Handler(void);

#endif /* __FLASH_H */