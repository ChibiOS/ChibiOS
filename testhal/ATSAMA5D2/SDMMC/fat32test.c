/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */
#include <string.h>
#include "ch.h"
#include "hal.h"

#include "ff.h"
#include "sama_sdmmc_lld.h"
#include "chprintf.h"


extern BaseSequentialStream * ts;



#if FF_MULTI_PARTITION == 1
#define	DEMO_VOLUMES	2

PARTITION VolToPart[] = {
		{1, 1},    /* "0:" ==> Physical drive 1, 1st partition */
		{1, 2},    /* "1:" ==> Physical drive 1, 2nd partition */
};

const TCHAR volume_paths[DEMO_VOLUMES][3] =
{
		{ '0', ':', '\0' },
		{ '1', ':', '\0' }
};

const TCHAR volume_labels[DEMO_VOLUMES][13] =
{
		{ '0', ':','C','H','I','B','I','V','O','L','_','1','\0' },
		{ '1', ':','C','H','I','B','I','V','O','L','_','2','\0' }
};

//two partition: 20% of total size the first, 80% the second
//using a number > 100 means sector count
const DWORD plist[] = { 50, 50, 0, 0 };


#else
#define	DEMO_VOLUMES	1
const TCHAR volume_paths[DEMO_VOLUMES][3] =
{
		{ '0', ':', '\0' }
};

const TCHAR volume_labels[DEMO_VOLUMES][13] =
{
		{ '0', ':','C','H','I','B','I','V','O','L','_','1','\0' }
};

#endif




static FATFS fs_header;
static FIL f_header;
const char test_file_path[] = "test.txt";

//Welcome to Chibios!
const uint8_t buffer[19]={
		0x57,0x65,0x6C,0x63,0x6F,
		0x6D,0x65,0x20,0x74,0x6F,
		0x20,0x43,0x68,0x69,0x62,
		0x69,0x4F,0x53,0x21
};

static void getdir(uint8_t vi);
static void writefile(uint8_t vi);
static void readfile(uint8_t vi);
static bool format(void);
static bool mountVolume(uint8_t vi, FATFS *fs);
static bool unmountVolume(uint8_t vi);

extern CACHE_ALIGNED uint8_t data_buf[];

void fat32test_demo(void)
{
	uint8_t i;
	chprintf(ts,"Init FAT32 FS\r\n" );
	if ( format() ) {

		for (i=0;i<DEMO_VOLUMES;i++) {
			if ( mountVolume(i,&fs_header) ) {
				writefile(i);
				chprintf(ts,"reading dir:\n\r");
				getdir(i);
				readfile(i);

				unmountVolume(i);

			}
		}

	}
}


static void writefile(uint8_t vi)
{
	TCHAR file_path[sizeof(volume_paths[vi]) + sizeof(test_file_path)];
	FRESULT res;
	UINT len;

	strcpy(file_path, volume_paths[vi]);
	strcat(file_path, test_file_path);

	chprintf(ts,"Creating new file ... %s ",file_path);
	res = f_open(&f_header, file_path, FA_WRITE | FA_CREATE_ALWAYS);
	if (res == FR_OK) {
		chprintf(ts,"OK\r\n");
		res = f_write(&f_header, buffer, 19, &len);
		if (res == FR_OK) {
			chprintf(ts,"written %d bytes\n\r", len);
		}
		else
		{
			chprintf(ts,"error %d \n\r", res);
		}
	}
	else
	{
		chprintf(ts,"Failed error %d\n\r", res);
	}
	f_close(&f_header);

}

static void readfile(uint8_t vi)
{
	TCHAR file_path[sizeof(volume_paths[vi]) + sizeof(test_file_path)];
	FRESULT res;
	UINT len;
	uint8_t buffer[19];
	UINT i;

	strcpy(file_path, volume_paths[vi]);
	strcat(file_path, test_file_path);

	chprintf(ts,"Reading back the new file ... ");
	res = f_open(&f_header, file_path, FA_OPEN_EXISTING | FA_READ);
	if (res == FR_OK) {
		chprintf(ts,"OK\r\n");
		res = f_read(&f_header, buffer, 19, &len);
		if (res == FR_OK) {
			chprintf(ts,"read %d bytes\n\r", len);
			for (i=0;i<len;i++) {
				chprintf(ts,"%c", buffer[i]);
			}
			chprintf(ts,"\r\n");
		}
	}
	else
	{
		chprintf(ts,"Failed error %d\n\r", res);
	}
	f_close(&f_header);

}

static void getdir(uint8_t vi)
{
	DIR dir = { .sect = 0 };
	FILINFO fno = { 0 };
	FRESULT res;
	bool is_dir;

	res = f_opendir(&dir, volume_paths[vi]);
	if (res != FR_OK) {
		chprintf(ts,"Failed to open dir, error %d\n\r", res);
		return;
	}
	chprintf(ts,"files in the root directory:\n\r");
	for (;;) {
		res = f_readdir(&dir, &fno);
		if (res != FR_OK) {
			chprintf(ts,"cannot read dir, error (%d)\n\r", res);
			break;
		}
		if (fno.fname[0] == '\0')
			break;
		is_dir = fno.fattrib & AM_DIR ? true : false;
		chprintf(ts,"    %s%s%c\n\r", is_dir ? "[" : "", fno.fname,
				is_dir ? ']' : ' ');
	}

	res = f_closedir(&dir);
}

static bool mountVolume(uint8_t vi, FATFS *fs)
{
	FRESULT res;
	DWORD fre_clust, fre_sect, tot_sect;

	memset(fs, 0, sizeof(FATFS));
	res = f_mount(fs, volume_paths[vi], 1);
	if (res != FR_OK) {
		chprintf(ts,"Failed to mount FAT file system, error %d\n\r", res);
		return false;
	}
	else
	{
		chprintf(ts, "setting label %s",volume_labels[vi]);
		res = f_setlabel(volume_labels[vi]);
		if (res == FR_OK) {
			chprintf(ts, " OK\r\n");
		}
		else
		{
			chprintf(ts, " FAILED err %d\r\n",res);
			return false;
		}
		res = f_getfree(volume_paths[vi], &fre_clust, &fs);
		if (res == FR_OK) {
			//Get total sectors and free sectors
			tot_sect = (fs->n_fatent - 2) * fs->csize;
			fre_sect = fre_clust * fs->csize;

			chprintf(ts,"%10lu KiB total drive space.\r\n%10lu KiB available.\r\n",tot_sect / 2, fre_sect / 2);
		}

	}

	return true;
}


static bool unmountVolume(uint8_t vi)
{
	FRESULT res;
	bool rc = true;

	res = f_mount(NULL, volume_paths[vi], 0);
	if (res != FR_OK)
		rc = false;

	return rc;

}


static bool format(void)
{
	uint8_t i;

	FRESULT res = FR_OK;



#if FF_MULTI_PARTITION == 1
	chprintf(ts, "fdisk ... \n\r");
	res = f_fdisk(0, plist, data_buf);
#endif

	if (res == FR_OK) {

		for (i = 0; i < DEMO_VOLUMES; i++) {
			chprintf(ts, "Creating fat32 volume %s\n\r", volume_paths[i]);

			res = f_mkfs(volume_paths[i], FM_FAT32, 0, data_buf,FF_MAX_SS);

			if (res == FR_OK) {
				chprintf(ts, "format OK\r\n");

			} else {
				chprintf(ts, "failed creating volume %s error %d\n\r", volume_paths[i],res);
				return false;
			}

		}
	}
	return true;
}
