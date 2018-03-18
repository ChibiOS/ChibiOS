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
#include <stdio.h>
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "sama_sdmmc_lld.h"

#if SDMMC_USE_RELEDGE_LIB == 1
#include "chprintf.h"
#include <redfs.h>
#include <redposix.h>
#include <redfse.h>
#include <redconf.h>
#include <redtypes.h>
#include <redmacs.h>
#include <redvolume.h>

static int32_t format(uint8_t volnum);
static void readfiles(void);
static void writefiles(uint8_t volnum);

#define fsRAM_BUFFER_SIZE	30
static char cRAMBuffer[ fsRAM_BUFFER_SIZE ];

static const char *pcDirectory1 = "/DIR1", *pcDirectory2 = "/DIR1/DIR2";
extern BaseSequentialStream * ts;
extern const VOLCONF gaRedVolConf[REDCONF_VOLUME_COUNT];

void relianceedge_demo(void)
{
  int32_t ret;
  int volnum = 0;
  const char *pszVolume = gaRedVolConf[volnum].pszPathPrefix;

  chprintf(ts,"Init Reliance Edge FS\r\n" );
  ret = red_init();

  chprintf(ts,"Start formatting fs ... " );
  ret = format(volnum);

  if ( ret == 0 )
  {
    chprintf(ts,"OK , mounting volume .." );

    ret = red_mount(pszVolume);
    if ( ret == 0 )
    {
      chprintf(ts,"OK\r\n writing files ..\r\n" );
      writefiles(volnum);
      chprintf(ts,"OK\r\n reading files ..\r\n" );
      readfiles();
      chprintf(ts,"OK\r\n" );
    }
  }
  else
  {
    chprintf(ts,"FAILED! \r\n" );
  }
}



static void writefiles(uint8_t volnum)
{
  uint32_t i, j;
  char path[ 64 ];
  const uint32_t filen = 5;

  uint32_t ulEventMask;
  int32_t written, filedescr, status;
  int iByte;

  const char *pszVolume = gaRedVolConf[volnum].pszPathPrefix;
  /* Save the current transaction point settings. */
  status = red_gettransmask( pszVolume, &ulEventMask );
  osalDbgCheck( status == 0 );

  /* Disable automatic transaction points so that all of the files can be
		created in one atomic operation. */
  status = red_settransmask( pszVolume, RED_TRANSACT_MANUAL );
  osalDbgCheck( status == 0 );

  /* Create filen files.  Each created file will be
		( i * fsRAM_BUFFER_SIZE ) bytes in length, and filled
		with a different repeating character. */
  for( i = 1; i <= filen; i++ )
  {
    /* Generate a file name. */
    sprintf( path, "/root%03d.txt", (int)i );

    /* Print out the file name and the directory into which the file is
			being written. */
    chprintf(ts,"Creating file %s\r\n", path );

    /* Open the file, creating the file if it does not already exist. */
    filedescr = red_open( path, RED_O_CREAT|RED_O_TRUNC|RED_O_WRONLY );
    osalDbgCheck( filedescr != -1 );

    /* Fill the RAM buffer with data that will be written to the file.  This
			is just a repeating ascii character that indicates the file number. */
    memset( cRAMBuffer, ( int ) ( '0' + i ), fsRAM_BUFFER_SIZE );

    /* Write the RAM buffer to the opened file a number of times.  The
			number of times the RAM buffer is written to the file depends on the
			file number, so the length of each created file will be different. */
    for( j = 0; j < i; j++ )
    {
      written = red_write( filedescr, cRAMBuffer, fsRAM_BUFFER_SIZE );
      osalDbgCheck( written == fsRAM_BUFFER_SIZE );
    }

    /* Close the file so another file can be created. */
    status = red_close( filedescr );
    osalDbgCheck( status == 0 );
  }

  /* Commit a transaction point, atomically adding the set of files to the
		transacted state. */
  status = red_transact( pszVolume );
  osalDbgCheck( status == 0 );

  /* Create a sub directory. */
  chprintf(ts,"Creating directory %s\r\n", pcDirectory1 );

  status = red_mkdir( pcDirectory1 );
  osalDbgCheck( status == 0 );

  /* Create a subdirectory in the new directory. */
  chprintf(ts, "Creating directory %s\r\n", pcDirectory2 );

  status = red_mkdir( pcDirectory2 );
  osalDbgCheck( status == 0 );

  /* Generate the file name. */
  sprintf( path, "%s/file.txt", pcDirectory2 );

  /* Print out the file name and the directory into which the file is being
		written. */
  chprintf(ts, "Writing file %s\r\n", path );

  filedescr = red_open( path, RED_O_CREAT|RED_O_TRUNC|RED_O_WRONLY );

  /* Write the file.  It is filled with incrementing ascii characters starting
		from '0'. */
  for( iByte = 0; iByte < fsRAM_BUFFER_SIZE; iByte++ )
  {
    cRAMBuffer[ iByte ] = ( char ) ( ( int ) '0' + iByte );
  }

  written = red_write( filedescr, cRAMBuffer, fsRAM_BUFFER_SIZE );
  osalDbgCheck( written == fsRAM_BUFFER_SIZE );

  /* Finished so close the file. */
  status = red_close( filedescr );
  osalDbgCheck( status == 0 );

  /* Commit a transaction point, atomically adding the set of files and
		directories to the transacted state. */
  status = red_transact( pszVolume );
  osalDbgCheck( status == 0 );

  /* Restore previous transaction point settings. */
  status = red_settransmask( pszVolume, ulEventMask );
  osalDbgCheck( status == 0 );
}

static void readfiles(void)
{
  uint32_t i, j;
  char path[ 64 ];
  const uint32_t filen = 5;
  long lChar;
  int32_t lBytesRead, filedescr, status;
  int iByte;


  /* Read back the files that were created by prvCreateDemoFiles(). */
  for( i = 1; i <= filen; i++ )
  {
    /* Generate the file name. */
    sprintf( path, "/root%03d.txt",(int) i );

    /* Print out the file name and the directory from which the file is
			being read. */
    chprintf(ts,"Reading file %s\r\n", path );

    /* Open the file for reading. */
    filedescr = red_open( path, RED_O_RDONLY );
    osalDbgCheck( filedescr != -1 );

    /* Read the file into the RAM buffer, checking the file contents are as
			expected.  The size of the file depends on the file number. */
    for( j = 0; j < i; j++ )
    {
      /* Start with the RAM buffer clear. */
      memset( cRAMBuffer, 0x00, fsRAM_BUFFER_SIZE );

      lBytesRead = red_read( filedescr, cRAMBuffer, fsRAM_BUFFER_SIZE );
      osalDbgCheck( lBytesRead == fsRAM_BUFFER_SIZE );

      /* Check the RAM buffer is filled with the expected data.  Each
				file contains a different repeating ascii character that indicates
				the number of the file. */
      for( lChar = 0; lChar < fsRAM_BUFFER_SIZE; lChar++ )
      {
        osalDbgCheck( cRAMBuffer[ lChar ] == ( '0' + ( char ) i ) );
      }
    }

    /* Close the file. */
    status = red_close( filedescr );
    osalDbgCheck( status == 0 );
  }

  /* Generate the file name. */
  sprintf( path, "%s/file.txt", pcDirectory2 );

  /* Print out the file name and the directory from which the file is being
		read. */
  chprintf(ts, "Reading file %s\r\n", path );

  /* This time the file is opened for reading. */
  filedescr = red_open( path, RED_O_RDONLY );
  osalDbgCheck( filedescr != -1 );

  /* Read the file. */
  lBytesRead = red_read( filedescr, cRAMBuffer, fsRAM_BUFFER_SIZE );
  osalDbgCheck( lBytesRead == fsRAM_BUFFER_SIZE );

  /* Verify the file 1 byte at a time. */
  for( iByte = 0; iByte < fsRAM_BUFFER_SIZE; iByte++ )
  {
    osalDbgCheck( cRAMBuffer[ iByte ] == ( char ) ( ( int ) '0' + iByte ) );
  }

  /* Finished so close the file. */
  status = red_close( filedescr );
  osalDbgCheck( status == 0 );

}


static int32_t format(uint8_t volnum)
{
  int32_t ret=-1;
#if REDCONF_API_POSIX_FORMAT == 1
  const char *pszVolume = gaRedVolConf[volnum].pszPathPrefix;
  ret = red_format(pszVolume);

#endif
  return ret;
}

#endif

