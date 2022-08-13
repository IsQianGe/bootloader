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

extern lfs_t lfs;

void LFS_Init(void);
extern void app_init(void);

#endif /*  __LFS_PORT_H__ */
