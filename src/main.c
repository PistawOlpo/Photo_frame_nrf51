/**
 * Copyright (c) 2016 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/** @file
 * @defgroup frame_main main.c
 * @{
 * @ingroup frame
 * @brief Frame Application main file.
 *
 * This file contains the source code for a sample application using FAT filesystem and SD card library.
 * The program reads in sequence all .jpg files in the folder outdata/ on the SD card and displaying
 * them on the 7.3 inch color display from waveshare 
 *
 */


#include "nrf.h"
#include "bsp.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define NRF_LOG_MODULE_NAME2 "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "epaper.h"
#include <string.h>


#define TEST_STRING2 "SD card example.\r\n"

#define SDC_SCK_PIN     ARDUINO_13_PIN  ///< SDC serial clock (SCK) pin.
#define SDC_MOSI_PIN    ARDUINO_11_PIN  ///< SDC serial data in (DI) pin.
#define SDC_MISO_PIN    ARDUINO_12_PIN  ///< SDC serial data out (DO) pin.
#define SDC_CS_PIN      ARDUINO_10_PIN  ///< SDC chip select (CS) pin.




#define TEST_STRING "Frame"

uint8_t      read_buf[400];
static volatile bool buf_write_read_done; 
uint32_t current_File[12+1];


/**
 * @brief  SDC block device definition
 * */
NRF_BLOCK_DEV_SDC_DEFINE(
        m_block_dev_sdc,
        NRF_BLOCK_DEV_SDC_CONFIG(
                SDC_SECTOR_SIZE,
                APP_SDCARD_CONFIG(SDC_MOSI_PIN, SDC_MISO_PIN, SDC_SCK_PIN, SDC_CS_PIN)
         ),
         NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "SDC", "1.00")
);

/**
 * @brief Function for FAFTS usage.
 **/
static void fatfs_example()
{
    static FATFS fs;
    static DIR dir,dj;
    static FILINFO fno2;
    static FIL file;

    UINT br;         /* File read/write count */


    FRESULT ff_result;
    DSTATUS disk_state = STA_NOINIT;

    // Initialize FATFS disk I/O interface by providing the block device.
    static diskio_blkdev_t drives[] =
    {
            DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev), NULL)
    };

    diskio_blockdev_register(drives, ARRAY_SIZE(drives));

    NRF_LOG_INFO("Initializing disk 0 (SDC)...\r\n");
    for (uint32_t retries = 3; retries && disk_state; --retries)
    {
        disk_state = disk_initialize(0);
    }
    if (disk_state)
    {
        NRF_LOG_INFO("Disk initialization failed.\r\n");
        return;
    }
    
    uint32_t blocks_per_mb = (1024uL * 1024uL) / m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_size;
    uint32_t capacity = m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_count / blocks_per_mb;
    NRF_LOG_INFO("Capacity: %d MB\r\n", capacity);

    NRF_LOG_INFO("Mounting volume...\r\n");
    ff_result = f_mount(&fs, "", 1);
    if (ff_result)
    {
        NRF_LOG_INFO("Mount failed.\r\n");
        return;
    }

    NRF_LOG_INFO("\r\n Listing directory: /outdata\r\n");
    ff_result = f_opendir(&dir, "/outdata");
    if (ff_result)
    {
        NRF_LOG_INFO("Directory listing failed!\r\n");
        return;
    }
    
    NRF_LOG_RAW_INFO("\r\n");
       
   
    ff_result = f_findfirst(&dj, &fno2, "/OUTDATA", "*.DAT"); /* Start to search for photo files */
    NRF_LOG_INFO("file: %s .\r\n",(u_int32_t)fno2.fname);


    while(ff_result == FR_OK && fno2.fname[0]) {
        TCHAR dest[8+1+12+1]="/outdata/";
        strncat(dest, fno2.fname, 13);
    
        ff_result = f_open(&file, dest , FA_READ );
        if (ff_result != FR_OK)
        {
            NRF_LOG_INFO("Unable to open or create file: %s .\r\n",(u_int32_t)dest);
            return;
        }
            NRF_LOG_INFO("Open file: %s .\r\n",(u_int32_t) dest );

          if (Epd_Init() != 0) {
            NRF_LOG_INFO("e-Paper init failed\r\n")
        return ;
        } 
        Epd_Clear(EPD_7IN3F_WHITE);
        nrf_delay_ms(14000);
        Epd_SendCommand(0x10);
  
        for (;;) {
            ff_result = f_read(&file, read_buf, sizeof(read_buf), &br); /* Read a chunk of data from the source file */
            NRF_LOG_INFO("Read %u\r\n.", br);
            if (br == 0) break; /* error or eof */
            for(UINT j = 0 ; j < br; j ++) {
                    Epd_SendData(read_buf[j]);
                }
        }
        (void) f_close(&file);
        Epd_TurnOnDisplay();
        nrf_delay_ms(14000);
        Epd_Sleep();

        nrf_delay_ms(60000);
        ff_result = f_findnext(&dj, &fno2);
    }

    if (ff_result != FR_OK)
    {
        NRF_LOG_INFO("read failed\r\n.");
    }
    else
    {
        NRF_LOG_INFO("read success\r\n");
    }   
    return;
}

int main(void)
{

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

    NRF_LOG_INFO("SPI test\r\n");

    
    Epd_Reset();
    if (Epd_Init() != 0) {
        NRF_LOG_INFO("e-Paper init failed\r\n");
        return -1;
    } 
    NRF_LOG_INFO("e-Paper is init\r\n");
   
    
    while (true)
    {
        fatfs_example();
    }
    NRF_LOG_INFO("\r\nEnd of program.\r\n\r\n");
}

/** @} */
