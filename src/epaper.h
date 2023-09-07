/*****************************************************************************
* | File      	:   epaper.h
* | Author      :   Waveshare team, and other
* | Function    :   7.3inch e-paper F
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-10-21
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



#ifndef __EPAPER_7IN3F_H__
#define __EPAPER_7IN3F_H__


// Display resolution
#define EPD_WIDTH       800
#define EPD_HEIGHT      480

#define UWORD   unsigned int
#define UBYTE   unsigned char
#define UDOUBLE  unsigned long

/**********************************
Color Index
**********************************/
#define EPD_7IN3F_BLACK   0x0	/// 000
#define EPD_7IN3F_WHITE   0x1	///	001
#define EPD_7IN3F_GREEN   0x2	///	010
#define EPD_7IN3F_BLUE    0x3	///	011
#define EPD_7IN3F_RED     0x4	///	100
#define EPD_7IN3F_YELLOW  0x5	///	101
#define EPD_7IN3F_ORANGE  0x6	///	110
#define EPD_7IN3F_CLEAN   0x7	///	111   unavailable  Afterimage

int  Epd_Init(void);
void Epd_BusyHigh(void);
void Epd_TurnOnDisplay(void);
void Epd_Reset(void);
void Epd_Display(const UBYTE *image);
void Epd_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart, 
                                 UWORD image_width, UWORD image_heigh);
void Epd_Show7Block(void);
void Epd_SendCommand(unsigned char command);
void Epd_SendData(unsigned char data);
void Epd_Sleep(void);
void Epd_Clear(UBYTE color);

#define EP_SCK_PIN     9  ///< epaper serial clock (SCK) pin P.
#define EP_MOSI_PIN    10  ///< epaper serial data in (DI) pin P0.10.
#define EP_MISO_PIN    11 ///< epaper not using it serial data out (DO) pin.
#define EP_CS_PIN      8   ///< epaper chip select (CS) pin P0.08.


#define EP_BUSY        5 // epaper busy pin P0.05
#define EP_RST         6 // P0.06
#define EP_DC          7 // P0.07


#endif /* */



