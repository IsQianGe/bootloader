/*
 * lfs_port.c
 *
 *  Created on: 2021年12月13日
 *      Author: DELL
 */
#include "lfs_port.h"
#include "sfud.h"

lfs_t lfs;

int com_lfs_flash_read(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, void *buffer,
                       lfs_size_t size)
{
    int ret = LFS_ERR_OK;
    LFS_DEBUG("%s %d block:%d off: %d buffer: %p size: %d", __FUNCTION__, __LINE__, block, off,
              buffer, size);
    const sfud_flash *flash = sfud_get_device(SFUD_ZD25_DEVICE_INDEX);
    if (SFUD_SUCCESS != sfud_read(flash, block * cfg->block_size + off, size, buffer)) {
        ret = LFS_ERR_IO;
    }
    return ret;
}

int com_lfs_flash_prog(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off,
                       const void *buffer, lfs_size_t size)
{
    int ret = LFS_ERR_OK;
    LFS_DEBUG("%s %d block:%d off: %d buffer: %p size: %d", __FUNCTION__, __LINE__, block, off,
              buffer, size);
    const sfud_flash *flash = sfud_get_device(SFUD_ZD25_DEVICE_INDEX);
    if (SFUD_SUCCESS != sfud_write(flash, block * cfg->block_size + off, size, buffer)) {
        ret = LFS_ERR_IO;
    }
    return ret;
}

int com_lfs_flash_erase(const struct lfs_config *cfg, lfs_block_t block)
{
    int ret = LFS_ERR_OK;
    LFS_DEBUG("%s %d block:%d ", __FUNCTION__, __LINE__, block);
    const sfud_flash *flash = sfud_get_device(SFUD_ZD25_DEVICE_INDEX);
    if (SFUD_SUCCESS != sfud_erase(flash, block * cfg->block_size, cfg->block_size)) {
        ret = LFS_ERR_IO;
    }
    return ret;
}


const struct lfs_config lfs_cfg = {
    // block device operations
    .read = com_lfs_flash_read,
    .prog = com_lfs_flash_prog,
    .erase = com_lfs_flash_erase,
    .sync = NULL,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 512,
    .block_count = CAPACITY / 512,
    .cache_size = 256,
    .lookahead_size = 16,
    .block_cycles = 500,
};

BOOT_ErrorStatus lfs_Init(void)
{
  if (SFUD_SUCCESS != sfud_init())
  {
    return BOOT_ERROR;
  }
  /* mount spi flash */
  if (lfs_mount(&lfs, &lfs_cfg))
  {
    // osDelay(50);
    if(lfs_mount(&lfs, &lfs_cfg))
    return BOOT_ERROR;
  }
return BOOT_SUCCESS;
}
