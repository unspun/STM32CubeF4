/**
  ******************************************************************************
  * @file    sd_diskio_template_bspv2.c
  * @author  MCD Application Team
  * @brief   SD Disk I/O template driver Using the BSPv2 API. This file needs
  *          to be renamed and copied into the application project
  *          alongside the respective header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017-2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* use the default SD timeout as defined in the platform BSP driver*/
#if defined(SDMMC_DATATIMEOUT)
#define SD_TIMEOUT SDMMC_DATATIMEOUT
#elif defined(SD_DATATIMEOUT)
#define SD_TIMEOUT SD_DATATIMEOUT
#else
#define SD_TIMEOUT 30 * 1000
#endif

#define SD_DEFAULT_BLOCK_SIZE 512

/*
 * in case the BSP_SD_XXX() API is called by the application
 * make sure the use the same instance in the application and the
 * diskio driver
 */
#ifndef BSP_SD_INSTANCE
#define BSP_SD_INSTANCE 0
#endif

#if BSP_SD_INSTANCE >= SD_INSTANCES_NBR
#error "Wrong BSP_SD_INSTANCE"
#endif
/*
 * Depending on the usecase, the SD card initialization could be done at the
 * application level, if it is the case define the flag below to disable
 * the BSP_SD_Init() call in the SD_Initialize().
 */

/* #define DISABLE_SD_INIT */

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
static DSTATUS SD_CheckStatus(FF_BYTE lun);
DSTATUS SD_initialize (FF_BYTE);
DSTATUS SD_status (FF_BYTE);
DRESULT SD_read (FF_BYTE, FF_BYTE*, FF_DWORD, FF_UINT);
#if _USE_WRITE == 1
  DRESULT SD_write (FF_BYTE, const FF_BYTE*, FF_DWORD, FF_UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT SD_ioctl (FF_BYTE, FF_BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read,
#if  _USE_WRITE == 1
  SD_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/
static DSTATUS SD_CheckStatus(FF_BYTE lun)
{
  Stat = STA_NOINIT;

  if(BSP_SD_GetCardState(BSP_SD_INSTANCE) == BSP_ERROR_NONE)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_initialize(FF_BYTE lun)
{
  Stat = STA_NOINIT;
#if !defined(DISABLE_SD_INIT)

  if(BSP_SD_Init(BSP_SD_INSTANCE) == MSD_OK)
  {
    Stat = SD_CheckStatus(lun);
  }

#else
  Stat = SD_CheckStatus(lun);
#endif
  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_status(FF_BYTE lun)
{
  return SD_CheckStatus(lun);
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SD_read(FF_BYTE lun, FF_BYTE *buff, FF_DWORD sector, FF_UINT count)
{
  DRESULT res = RES_ERROR;

  if(BSP_SD_ReadBlocks(BSP_SD_INSTANCE,(uint32_t*)buff,
                       (uint32_t) (sector),
                       count) == BSP_ERROR_NONE)
  {
    /* wait until the read operation is finished */
    while(BSP_SD_GetCardState(BSP_SD_INSTANCE)!= BSP_ERROR_NONE)
    {
    }
    res = RES_OK;
  }

  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT SD_write(FF_BYTE lun, const FF_BYTE *buff, FF_DWORD sector, FF_UINT count)
{
  DRESULT res = RES_ERROR;

  if(BSP_SD_WriteBlocks(BSP_SD_INSTANCE,(uint32_t*)buff,
                        (uint32_t)(sector),
                        count) == BSP_ERROR_NONE)
  {
	/* wait until the Write operation is finished */
    while(BSP_SD_GetCardState(BSP_SD_INSTANCE) != BSP_ERROR_NONE)
    {
    }
    res = RES_OK;
  }

  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(FF_BYTE lun, FF_BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  BSP_SD_CardInfo CardInfo;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (FF_DWORD) */
  case GET_SECTOR_COUNT :
    BSP_SD_GetCardInfo(BSP_SD_INSTANCE,&CardInfo);
    *(FF_DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (FF_WORD) */
  case GET_SECTOR_SIZE :
    BSP_SD_GetCardInfo(BSP_SD_INSTANCE,&CardInfo);
    *(FF_WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (FF_DWORD) */
  case GET_BLOCK_SIZE :
    BSP_SD_GetCardInfo(BSP_SD_INSTANCE,&CardInfo);
    *(FF_DWORD*)buff = CardInfo.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
	res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

