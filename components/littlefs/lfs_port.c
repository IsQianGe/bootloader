/*
 * lfs_port.c
 *
 *  Created on: 2021年12月13日
 *      Author: DELL
 */
#include "lfs_port.h"
#include "sfud.h"

lfs_t lfs;

const struct lfs_config lfs_cfg = {
    // block device operations
    .read = sfud_read,
    .prog = sfud_write,
    .erase = sfud_erase,
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
  sfud_err ret;
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
