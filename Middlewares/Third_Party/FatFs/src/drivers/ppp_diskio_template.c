/**
  ******************************************************************************
  * @file    ppp_diskio.c
  * @author  MCD Application Team
  * @brief   PPP Disk I/O driver generic driver template
             this driver is not functional and is intended to show
	           how to implement a FatFs diskio driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Block Size in Bytes */
/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
DSTATUS PPP_initialize (FF_BYTE);
DSTATUS PPP_status (FF_BYTE);
DRESULT PPP_read (FF_BYTE, FF_BYTE*, FF_DWORD, FF_UINT);
#if _USE_WRITE == 1
  DRESULT PPP_write (FF_BYTE, const FF_BYTE*, FF_DWORD, FF_UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT PPP_ioctl (FF_BYTE, FF_BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  PPP_Driver =
{
  PPP_initialize,
  PPP_status,
  PPP_read,
#if  _USE_WRITE == 1
  PPP_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  PPP_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS PPP_initialize(FF_BYTE lun)
{
  Stat = STA_NOINIT;

  /* Configure the uPPP device */
  if(MEDIA_Init() == PPP_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS PPP_status(FF_BYTE lun)
{
  Stat = STA_NOINIT;

  if(MEDIA_GetStatus() == PPP_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT PPP_read(FF_BYTE lun, FF_BYTE *buff, FF_DWORD sector, FF_UINT count)
{
  DRESULT res = RES_OK;

  if(MEDIA_Read((uint32_t*)buff,
                       sector,
                       count) != PPP_OK)
  {
    res = RES_ERROR;
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
DRESULT PPP_write(FF_BYTE lun, const FF_BYTE *buff, FF_DWORD sector, FF_UINT count)
{
  DRESULT res = RES_OK;

  if(MEDIA_Write((uint32_t*)buff,
                       sector,
                       count) != PPP_OK)
  {
    res = RES_ERROR;
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
DRESULT PPP_ioctl(FF_BYTE lun, FF_BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  MEDIA_Info_t Info;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (FF_DWORD) */
  case GET_SECTOR_COUNT :
    MEDIA_GetInfo(&Info);
    *(FF_DWORD*)buff = Info.SectorNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (FF_WORD) */
  case GET_SECTOR_SIZE :
    *(FF_WORD*)buff = Info.SectorSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (FF_DWORD) */
  case GET_BLOCK_SIZE :
    *(FF_DWORD*)buff = Info.BlockSize;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

