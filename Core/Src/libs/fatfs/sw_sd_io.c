/*
 * sw_sd_io.c
 *
 *  Created on: 13 wrz 2021
 *      Author: simon
 */


#include "sw_mcu_conf.h"

#include "ff.h"
#include "diskio.h"


DSTATUS MMC_disk_initialize ( void ) {
	return 0;
}
DSTATUS MMC_disk_status ( void ){
	return 0;
}
DRESULT MMC_disk_read ( BYTE *buff, LBA_t sector, UINT count ) {
	return 0;
}
DRESULT MMC_disk_write ( const BYTE *buff, LBA_t sector, UINT count ) {
	return 0;
}
DRESULT MMC_disk_ioctl ( void ) {
	return 0;
}















DSTATUS RAM_disk_initialize ( void ) { return 0; }
DSTATUS RAM_disk_status ( void ) { return 0; }
DRESULT RAM_disk_read ( BYTE *buff, LBA_t sector, UINT count ) { return 0; }
DRESULT RAM_disk_write ( const BYTE *buff, LBA_t sector, UINT count ) { return 0; }
DRESULT RAM_disk_ioctl ( void ) { return 0; }
DSTATUS USB_disk_initialize ( void ) { return 0; }
DSTATUS USB_disk_status ( void ){ return 0; }
DRESULT USB_disk_read ( BYTE *buff, LBA_t sector, UINT count ) { return 0; }
DRESULT USB_disk_write ( const BYTE *buff, LBA_t sector, UINT count ) { return 0; }
DRESULT USB_disk_ioctl ( void ) { return 0; }
