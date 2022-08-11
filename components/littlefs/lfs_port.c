/*
 * lfs_port.c
 *
 *  Created on: 2021年12月13日
 *      Author: DELL
 */
#include "lfs_port.h"

//SPI_HandleTypeDef hspi2;
lfs_t lfs;

const struct lfs_config lfs_cfg = {
    // block device operations
    .read  = W25Qxx_readLittlefs,
    .prog  = W25Qxx_writeLittlefs,
    .erase = W25Qxx_eraseLittlefs,
    .sync  = W25Qxx_syncLittlefs,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = W25Q16_ERASE_GRAN,
    .block_count = W25Q16_NUM_GRAN,
    .cache_size = 256,
    .lookahead_size = W25Q16_NUM_GRAN / 8,
    .block_cycles = 500,
};

HAL_StatusTypeDef LFS_Init(void)
{
  HAL_StatusTypeDef err;

  /* mount spi flash */
  if (lfs_mount(&lfs, &lfs_cfg))
  {
	// osDelay(50);
// #ifdef FACTORY_FIRMWARE
  // lfs_format(&lfs, &lfs_cfg);
// #endif
    // lfs_format(&lfs, &lfs_cfg);
	lfs_mount(&lfs, &lfs_cfg);
  }
  return err;
}

void app_init(void)
{
  lfs_file_t file;
  UPDATA_STATUS app_status;
  /* Rtos running successfully, set APP_STATUS to APP_OK  */
  if(lfs_file_open(&lfs, &file, "APP_STATUS",  LFS_O_RDWR | LFS_O_CREAT) == LFS_ERR_OK)
  {
    lfs_file_read(&lfs, &file, &app_status, 1);
    while(app_status != APP_OK)
    {
	  /* set APP_STATUS to APP_OK */
      app_status = APP_OK;
      lfs_file_seek(&lfs, &file,0,LFS_SEEK_SET);
      lfs_file_write(&lfs, &file, &app_status, 1);
    //   osDelay(100U);
      /* Confirm write */
      lfs_file_seek(&lfs, &file,0,LFS_SEEK_SET);
      lfs_file_read(&lfs, &file, &app_status, 1);
    }
    lfs_file_close(&lfs, &file);
  }
}
