/*
 * sw_sd_io.h
 *
 *  Created on: 13 wrz 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_FATFS_SW_SD_IO_H_
#define SRC_LIBS_FATFS_SW_SD_IO_H_


DSTATUS MMC_disk_initialize ( void );
DSTATUS MMC_disk_status ( void );
DRESULT MMC_disk_read ( BYTE *buff, LBA_t sector, UINT count );
DRESULT MMC_disk_write ( const BYTE *buff, LBA_t sector, UINT count );
DRESULT MMC_disk_ioctl ( void );




DSTATUS RAM_disk_initialize ( void );
DSTATUS RAM_disk_status ( void );
DRESULT RAM_disk_read ( BYTE *buff, LBA_t sector, UINT count );
DRESULT RAM_disk_write ( const BYTE *buff, LBA_t sector, UINT count );
DRESULT RAM_disk_ioctl ( void );
DSTATUS USB_disk_initialize ( void );
DSTATUS USB_disk_status ( void );
DRESULT USB_disk_read ( BYTE *buff, LBA_t sector, UINT count );
DRESULT USB_disk_write ( const BYTE *buff, LBA_t sector, UINT count );
DRESULT USB_disk_ioctl ( void );

#endif /* SRC_LIBS_FATFS_SW_SD_IO_H_ */
