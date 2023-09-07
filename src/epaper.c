/*****************************************************************************
* | File      	:   epaper.c
* | Author      :   Waveshare team, and others
* | Function    :   7.3inch e-paper F
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-09-07
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/



#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "epaper.h"


#define SPI_INSTANCE  1 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    spi_xfer_done = true;
}
bool spi_not_set =true;
int IfInit(void)
{
    if (spi_not_set == false) return 0;
    nrf_gpio_cfg_output(EP_RST);
    nrf_gpio_cfg_output(EP_DC);

    nrf_gpio_cfg_input(EP_BUSY, NRF_GPIO_PIN_PULLDOWN);

    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.frequency = NRF_SPI_FREQ_125K;
    spi_config.ss_pin   = EP_CS_PIN;
    spi_config.miso_pin = EP_MISO_PIN;
    spi_config.mosi_pin = EP_MOSI_PIN;
    spi_config.sck_pin  = EP_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler));
    spi_not_set=false;
    return 0;
}
/**
 *  @brief: basic function for sending commands
 */
void SpiTransfer(u_int8_t data)
{
    nrf_gpio_pin_clear(EP_CS_PIN);

    spi_xfer_done = false;

    (nrf_drv_spi_transfer(&spi, &data, 1,NULL, 0));
    while (!spi_xfer_done) {
        __WFE();
    }
    nrf_gpio_pin_set(EP_CS_PIN);
}
void Epd_SendCommand(u_int8_t command)
{
    nrf_gpio_pin_clear(EP_DC);
    SpiTransfer(command);
}
void Epd_SendData(u_int8_t data)
{
    nrf_gpio_pin_set(EP_DC);
    SpiTransfer(data);
}

void Epd_BusyHigh(void)// If BUSYN=0 then waiting
{
    while(!nrf_gpio_pin_read(EP_BUSY)) {
        nrf_delay_ms(10);
    }
}
void Epd_Reset(void)
{
    nrf_gpio_pin_set(EP_RST);
    nrf_delay_ms(200);
    nrf_gpio_pin_clear(EP_RST);           //module reset
    nrf_delay_ms(10);
    nrf_gpio_pin_set(EP_RST);
    nrf_delay_ms(200);
}
int Epd_Init(void)
{
    if (IfInit() != 0) {
        return -1;
    }
    Epd_Reset();
    nrf_delay_ms(200);
    Epd_BusyHigh();

    Epd_SendCommand(0xAA);    // CMDH
    Epd_SendData(0x49);
    Epd_SendData(0x55);
    Epd_SendData(0x20);
    Epd_SendData(0x08);
    Epd_SendData(0x09);
    Epd_SendData(0x18);

    Epd_SendCommand(0x01);
    Epd_SendData(0x3F);
    Epd_SendData(0x00);
    Epd_SendData(0x32);
    Epd_SendData(0x2A);
    Epd_SendData(0x0E);
    Epd_SendData(0x2A);

    Epd_SendCommand(0x00);
    Epd_SendData(0x5F);
    Epd_SendData(0x69);

    Epd_SendCommand(0x03);
    Epd_SendData(0x00);
    Epd_SendData(0x54);
    Epd_SendData(0x00);
    Epd_SendData(0x44);

    Epd_SendCommand(0x05);
    Epd_SendData(0x40);
    Epd_SendData(0x1F);
    Epd_SendData(0x1F);
    Epd_SendData(0x2C);

    Epd_SendCommand(0x06);
    Epd_SendData(0x6F);
    Epd_SendData(0x1F);
    Epd_SendData(0x1F);
    Epd_SendData(0x22);

    Epd_SendCommand(0x08);
    Epd_SendData(0x6F);
    Epd_SendData(0x1F);
    Epd_SendData(0x1F);
    Epd_SendData(0x22);

    Epd_SendCommand(0x13);    // IPC
    Epd_SendData(0x00);
    Epd_SendData(0x04);

    Epd_SendCommand(0x30);
    Epd_SendData(0x3C);

    Epd_SendCommand(0x41);     // TSE
    Epd_SendData(0x00);

    Epd_SendCommand(0x50);
    Epd_SendData(0x3F);

    Epd_SendCommand(0x60);
    Epd_SendData(0x02);
    Epd_SendData(0x00);

    Epd_SendCommand(0x61);
    Epd_SendData(0x03);
    Epd_SendData(0x20);
    Epd_SendData(0x01);
    Epd_SendData(0xE0);

    Epd_SendCommand(0x82);
    Epd_SendData(0x1E);

    Epd_SendCommand(0x84);
    Epd_SendData(0x00);

    Epd_SendCommand(0x86);    // AGID
    Epd_SendData(0x00);

    Epd_SendCommand(0xE3);
    Epd_SendData(0x2F);

    Epd_SendCommand(0xE0);   // CCSET
    Epd_SendData(0x00);

    Epd_SendCommand(0xE6);   // TSSET
    Epd_SendData(0x00);

    return 0;
}
void Epd_TurnOnDisplay(void)
{
    Epd_SendCommand(0x04);  // POWER_ON
    Epd_BusyHigh();

    Epd_SendCommand(0x12);  // DISPLAY_REFRESH
    Epd_SendData(0x00);
    Epd_BusyHigh();

    Epd_SendCommand(0x02);  // POWER_OFF
    Epd_SendData(0x00);
    Epd_BusyHigh();
}
/******************************************************************************
function :  Sends the part image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart,
                      UWORD image_width, UWORD image_heigh)
{
    unsigned long i,j;

    Epd_SendCommand(0x10);
    for(i=0; i<EPD_HEIGHT; i++) {
        for(j=0; j< EPD_WIDTH/2; j++) {
            if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
                Epd_SendData(image[(j-xstart/2) + (image_width/2*(i-ystart))]);
            } else {
                Epd_SendData(0x11);
            }
        }
    }

    Epd_TurnOnDisplay();
}

/******************************************************************************
function :  show 7 kind of color block
parameter:
******************************************************************************/
void Epd_Show7Block(void)
{
    unsigned long i, j, k;
    unsigned char const Color_seven[8] = {
        EPD_7IN3F_BLACK, EPD_7IN3F_BLUE, EPD_7IN3F_GREEN, EPD_7IN3F_ORANGE,
        EPD_7IN3F_RED, EPD_7IN3F_YELLOW, EPD_7IN3F_WHITE, EPD_7IN3F_WHITE
    };

    Epd_SendCommand(0x10);
    for(i=0; i<240; i++) {
        for(k = 0 ; k < 4; k ++) {
            for(j = 0 ; j < 100; j ++) {
                Epd_SendData((Color_seven[k]<<4) |Color_seven[k]);
            }
        }
    }

    for(i=0; i<240; i++) {
        for(k = 4 ; k < 8; k ++) {
            for(j = 0 ; j < 100; j ++) {
                Epd_SendData((Color_seven[k]<<4) |Color_seven[k]);
            }
        }
    }
    Epd_TurnOnDisplay();
}
/******************************************************************************
function :
      Clear screen
******************************************************************************/
void Epd_Clear(UBYTE color)
{
    Epd_SendCommand(0x10);
    for(int i=0; i<EPD_WIDTH/2; i++) {
        for(int j=0; j<EPD_HEIGHT; j++) {
            Epd_SendData((color<<4)|color);
        }
    }

    Epd_TurnOnDisplay();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          The only one parameter is a check code, the command would be
 *          You can use EPD_Reset() to awaken
 */
void Epd_Sleep(void)
{
    Epd_SendCommand(0x07);
    Epd_SendData(0xA5);
    nrf_delay_ms(100);
    nrf_gpio_pin_clear(EP_RST); // Reset
}



/* END OF FILE */