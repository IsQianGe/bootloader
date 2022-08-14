#include "boot_load.h"
#include <stdio.h>
#include "flash.h"

static uint32_t m_uDwlAreaAddress = 0U;          /*!< Address of to write in download area */
static uint32_t m_uFileSize = 0U;                /*!<  file size being received */
static uint32_t m_uNbrBlocks = 0U;               /*!< Number of blocks being received via */
static uint32_t m_uPacketsReceived = 0U;         /*!< Number of packets received via */
static BOOT_LOADER_StatusTypeDef m_LoaderStatus; /*!< Status of the loader */

/**
 * @brief   Header Packet Transfer completed callback.
 * @param  uFileSize: Dimension of the file that will be received.
 * @retval BOOT_ErrorStatus BOOT_SUCCESS if successful, BOOT_ERROR otherwise.
 */
BOOT_ErrorStatus BOOT_HeaderPktRxCpltCallback(uint32_t uFileSize)
{
    BOOT_ErrorStatus e_ret_status = BOOT_SUCCESS;
    BOOT_FLASH_StatusTypeDef x_flash_info;
    /*Reset of the  variables */
    m_uPacketsReceived = 0U;
    m_uNbrBlocks = 0U;
    m_uDwlAreaAddress = LOADER_REGION_ROM_START;

    /*Filesize information is stored*/
    m_uFileSize = uFileSize;
    /* erase rom*/
    if (BOOT_FLASH_Erase_Size(&x_flash_info, (uint8_t *)m_uDwlAreaAddress, LOADER_REGION_ROM_SIZE) !=
        BOOT_SUCCESS)
    {
        m_LoaderStatus = BOOT_LOADER_ERR_FLASH;
        e_ret_status = BOOT_ERROR;
    }
    return e_ret_status;
}

/**
 * @brief  Spi flash Data Packet Transfer completed callback.
 * @param  pData: Pointer to the buffer.
 * @param  uSize: Packet dimension.
 * @retval BOOT_ErrorStatus BOOT_SUCCESS if successful, BOOT_ERROR otherwise.
 */
/* Teraterm variant of the spi flash: uses 1kB packets */
BOOT_ErrorStatus BOOT_DataPktRxCpltCallback(uint8_t *pData, uint32_t uSize)
{
    /* Size of downloaded Image initialized with first packet (header) and checked along download process */
    BOOT_ErrorStatus e_ret_status = BOOT_SUCCESS;
    BOOT_FLASH_StatusTypeDef x_flash_info;
    uint32_t rx_size = uSize;
    uint8_t *p_data = pData;
    uint32_t uOldSize;

    /* Check the pointers allocation */
    if (p_data == NULL)
    {
        return BOOT_ERROR;
    }

    /*Increase the number of received packets*/
    m_uPacketsReceived++;

    /* Last packet : size of data to write could be different than BOOT_COM__PACKET_1K_SIZE */
    if (m_uPacketsReceived == m_uNbrBlocks)
    {
        /*Extracting actual payload from last packet*/
        if (0U == (m_uFileSize % PACKET_1K_SIZE))
        {
            /* The last packet must be fully considered */
            rx_size = PACKET_1K_SIZE;
        }
        else
        {
            /* The last packet is not full, drop the extra bytes */
            rx_size = m_uFileSize - ((uint32_t)(m_uFileSize / PACKET_1K_SIZE) *
                                     PACKET_1K_SIZE);
        }
    }

    if (e_ret_status == BOOT_SUCCESS)
    {
        /* Check size to avoid writing beyond DWL image size */
        if ((m_uDwlAreaAddress + rx_size) > (LOADER_REGION_ROM_START + m_uFileSize))
        {
            m_LoaderStatus = BOOT_LOADER_ERR_FW_LENGTH;
            e_ret_status = BOOT_ERROR;
        }
    }

    if (e_ret_status == BOOT_SUCCESS)
    {
        /* Set dimension to the appropriate length for FLASH programming.
         * Example: 64-bit length for L4.
         */
        if ((rx_size % (uint32_t)sizeof(BOOT_FLASH_write_t)) != 0U)
        {
            /* By construction, the length of the buffer (fw_decrypted_chunk or p_data) must be a multiple of
               sizeof(BOOT_IMG_write_t) to avoid reading out of the buffer */
            uOldSize = rx_size;
            rx_size = rx_size + ((uint32_t)sizeof(BOOT_FLASH_write_t) - (rx_size % (uint32_t)sizeof(BOOT_FLASH_write_t)));
            while (uOldSize < rx_size)
            {
                p_data[uOldSize] = 0xFF;
                uOldSize++;
            }
        }
    }

    /* Write Data in Flash */
    if (e_ret_status == BOOT_SUCCESS)
    {
        if (BOOT_FLASH_Write(&x_flash_info, (uint8_t *)m_uDwlAreaAddress, p_data, rx_size) == BOOT_SUCCESS)
        {
            m_uDwlAreaAddress += (rx_size);
        }
        else
        {
            m_LoaderStatus = BOOT_LOADER_ERR_FLASH;
            e_ret_status = BOOT_ERROR;
        }
    }

    /* Last packet : reset m_uPacketsReceived */
    if (m_uPacketsReceived == m_uNbrBlocks)
    {
        m_uPacketsReceived = 0U;
    }

    /* Reset data counters in case of error */
    if (e_ret_status == BOOT_ERROR)
    {
        /*Reset of the spi flash update variables */
        m_uFileSize = 0U;
        m_uPacketsReceived = 0U;
        m_uNbrBlocks = 0U;
    }

    return e_ret_status;
}