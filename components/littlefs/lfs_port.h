/*
 * app_lfs.h
 *
 *  Created on: 2021年12月13日
 *      Author: DELL
 */

#ifndef __LFS_PORT_H__
#define __LFS_PORT_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lfs.h"

#define CAPACITY  16 * 1024

extern lfs_t lfs;

BOOT_ErrorStatus lfs_Init(void);

#endif /*  __LFS_PORT_H__ */
