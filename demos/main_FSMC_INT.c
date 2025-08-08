/*
    Copyright (C) 2013-2015 Andrea Zoppi

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

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"
#if (HAL_USE_SERIAL_USB == TRUE)
#include "usbcfg.h"
#endif
#include "ili9341.h"
#include "res/wolf3d_vgagraph_chunk87.h"

#define ADDR_CMD  *(volatile uint16_t *) 0x60000000
#define ADDR_DATA *(volatile uint16_t *) 0x60040000 
#define DCX_ACTIVE  palSetPad(GPIOD,GPIOD_LED3)
#define DCX_PASSIVE palClearPad(GPIOD,GPIOD_LED3)
#define CSX_ACTIVE  palSetPad(GPIOD,GPIOD_PIN7)
#define CSX_PASSIVE palClearPad(GPIOD,GPIOD_PIN7)
#define WRX_ACTIVE  palSetPad(GPIOD,GPIOD_PIN7)
#define WRX_PASSIVE palClearPad(GPIOD,GPIOD_PIN7)
#define RDX_ACTIVE  palSetPad(GPIOD,GPIOD_PIN7)
#define RDX_PASSIVE palClearPad(GPIOD,GPIOD_PIN7)
uint32_t X_SIZE = 240;
uint32_t Y_SIZE = 320;
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F


const SPIConfig spi_cfg2 = {
  .circular= FALSE,
  .slave = FALSE,
  .ssport=GPIOB,
  .sspad=GPIOB_PIN12,//PB12
  .cr1=SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0,
  .cr2=0
};
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  while (true) {
    palSetPad(GPIOA, GPIOA_SDI);
    palSetPad(GPIOA, GPIOA_SDO);
    chThdSleepMilliseconds(500);   
    palClearPad(GPIOA, GPIOA_SDO); 
    palClearPad(GPIOA, GPIOA_SDI);       // Освобождение мьютекса
    chThdSleepMilliseconds(500);
  }
}

//Конфигурация SPI
const SRAMConfig cfg = {
  .bcr = FSMC_BCR_MBKEN | 
         FSMC_BCR_WREN | 
         FSMC_BCR_MWID_16 |
         FSMC_BCR_MTYP_SRAM,
  .btr = 0x0FF3FF12      
};
extern SPIDriver SPID2;
//Конфигурация дисплея
 const ILI9341Config ili9341_cfg = {
   &SPID2,  //SPI2
   GPIOD,    //dcx port
   GPIOD_LED3 //FSMC_A18 -PD13 -D/CX
  };

static void initialize_lcd(void) {
palSetPadMode(GPIOD,GPIOD_LED3,PAL_STM32_MODE_ALTERNATE | PAL_STM32_OSPEED_MASK | PAL_STM32_ALTERNATE(12));
  static const uint8_t pgamma[15] = {
    0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
    0x37, 0x07, 0x10, 0x03, 0x10, 0x11, 0x00
  };
  static const uint8_t ngamma[15] = {
    0x00, 0x16, 0x1B, 0x04, 0x11, 0x07, 0x31, 0x33,
    0x42, 0x05, 0x0C, 0x0A, 0x28, 0x2F, 0x0F
  };

  ILI9341Driver *const lcdp = &ILI9341D1;

ili9341AcquireBus(lcdp);
ili9341Select(lcdp);

ili9341WriteCommand(lcdp, ILI9341_CMD_RESET);
chThdSleepMilliseconds(120);

ili9341WriteCommand(lcdp, 0xCB);//Power Control A
ili9341WriteByte(lcdp, 0x39);
ili9341WriteByte(lcdp, 0x2C);
ili9341WriteByte(lcdp, 0x00);
ili9341WriteByte(lcdp, 0x34);
ili9341WriteByte(lcdp, 0x02);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, 0xCF);//Power Control B
ili9341WriteByte(lcdp, 0x00);
ili9341WriteByte(lcdp, 0xC1);
ili9341WriteByte(lcdp, 0x30);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, 0xE8);//Driver timing control A
ili9341WriteByte(lcdp, 0x85);
ili9341WriteByte(lcdp, 0x00);
ili9341WriteByte(lcdp, 0x78);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, 0xEA);//Driver timing control B
ili9341WriteByte(lcdp, 0x00);
ili9341WriteByte(lcdp, 0x00);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, 0xED);//Power on Sequence control
ili9341WriteByte(lcdp, 0x04);
ili9341WriteByte(lcdp, 0x03);
ili9341WriteByte(lcdp, 0x12);
ili9341WriteByte(lcdp, 0x81);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, 0xF7);//Pump ratio control
ili9341WriteByte(lcdp, 0x20);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_POWER_CTL_1);//Power Control 1
ili9341WriteByte(lcdp, 0x21);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_POWER_CTL_2);//Power Control 2
ili9341WriteByte(lcdp, 0x10);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_VCOM_CTL_1);//VCOM Control 1
ili9341WriteByte(lcdp, 0x3E);
ili9341WriteByte(lcdp, 0x28);
chThdSleepMilliseconds(5);
ili9341WriteCommand(lcdp, 0xC7);//VCOM Control 2
ili9341WriteByte(lcdp, 0x86);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_MEM_ACS_CTL);//Pixel Format Set
ili9341WriteByte(lcdp, 0x00);
chThdSleepMilliseconds(5);

// Настройка формата пикселей (16 бит)
ili9341WriteCommand(lcdp, ILI9341_SET_PIX_FMT);
ili9341WriteByte(lcdp, 0x55); // RGB565
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_FRAME_CTL_NORMAL);
ili9341WriteByte(lcdp, 0x00);
ili9341WriteByte(lcdp, 0x1B);// Частота кадров 79 Гц
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_FUNCTION_CTL);
ili9341WriteByte(lcdp, 0x0A);
ili9341WriteByte(lcdp, 0x82);
ili9341WriteByte(lcdp, 0x27);//320 строк
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_RGB_IF_SIG_CTL);//?
ili9341WriteByte(lcdp, 0x00);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_IF_CTL);//?
ili9341WriteByte(lcdp, 0x01);
ili9341WriteByte(lcdp, 0x10);
ili9341WriteByte(lcdp, 0x06);
chThdSleepMilliseconds(5);

ili9341WriteCommand(lcdp, ILI9341_SET_GAMMA);
ili9341WriteByte(lcdp, 0x01);
chThdSleepMilliseconds(5);
ili9341WriteCommand(lcdp, ILI9341_SET_PGAMMA);
ili9341WriteChunk(lcdp, pgamma, 15);
chThdSleepMilliseconds(5);
ili9341WriteCommand(lcdp, ILI9341_SET_NGAMMA);
ili9341WriteChunk(lcdp, ngamma, 15);
chThdSleepMilliseconds(5);
ili9341WriteCommand(lcdp, ILI9341_CMD_SLEEP_OFF);
chThdSleepMilliseconds(150);
ili9341WriteCommand(lcdp, ILI9341_CMD_DISPLAY_ON);
ili9341Unselect(lcdp);
ili9341ReleaseBus(lcdp);
palSetPad(GPIOB, GPIOB_PIN1);
}
void ili9341write_cmd(uint16_t cmd){
  DCX_PASSIVE;
  ADDR_CMD = cmd;
}

void ili9341write_data(uint16_t data){
  DCX_ACTIVE;
  ADDR_DATA = data;
}
uint8_t ili9341read_data(){
  DCX_ACTIVE;
  uint8_t data = ADDR_DATA;
  return data;
}
static void initialize_lcd_fsmc(void) {

static const uint8_t pgamma[15] = {
0x0F, 0x31, 0x2B, 0x0C, 0x0E, 
0x08, 0x4E, 0xF1, 0x37, 0x07, 
0x10, 0x03, 0x0E, 0x09, 0x00
};
static const uint8_t ngamma[15] = {
0x00, 0x0E, 0x14, 0x03, 0x11, 
0x07, 0x31, 0xC1, 0x48, 0x08, 
0x0F, 0x0C, 0x31, 0x36, 0x0F
};
ili9341write_cmd(ILI9341_CMD_RESET);
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_POWER_CTL_1);//Power Control 1
ili9341write_data(0x26);//0b0010 0011 VRH=100011 GVDD=4.6 V
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_POWER_CTL_2);//Power Control 2
ili9341write_data(0x11);
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_VCOM_CTL_1);//VCOM Control 1
ili9341write_data(0x35);//0b0011 1110 VMH=0111110 VCOMH=4.25 V
ili9341write_data(0x3E);//0b0010 1000 VML=0101000 VCOML=-1.5 V
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_VCOM_CTL_2);//VCOM Control 1
ili9341write_data(0xBE);//0b1000 0110  VMF=0000110
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_MEM_ACS_CTL);
ili9341write_data(0x00);//0b0100 1000 MY=0,MX=1,MV=0,ML=0,RGB=0,MH=0
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_PIX_FMT);// Настройка формата пикселей (16 бит)
ili9341write_data(0x55);//DPI=101,DBI=101 
chThdSleepMilliseconds(1);

ili9341write_cmd(ILI9341_SET_FRAME_CTL_NORMAL);
ili9341write_data(0x00);//fosc;
ili9341write_data(0x1d);//70 Hz
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_FUNCTION_CTL);
ili9341write_data(0x0a);//0b0000 1000 PTG=10,PT=00
ili9341write_data(0x02);//0b1000 0010 REV=1,GS=0,SS=0,SM=0,ISC=2(5 frames)
ili9341write_data(0x27);// 0b0010 0111 NL=100111 320 lines
ili9341write_data(0x3F);
chThdSleepMilliseconds(1);
// ili9341write_cmd(ILI9341_CMD_INVERT_ON);
// chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_RGB_IF_SIG_CTL);
ili9341write_data(0xC0);//0100 0000 RCM=10
chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_SET_IF_CTL);
ili9341write_data(0x01);//WEMODE=1
ili9341write_data(0x10);//0b0001 0000 EPF=01
ili9341write_data(0x06);//0b0000 0110 DM=01,RM=1,RIM=0
chThdSleepMilliseconds(1);
// ili9341write_cmd(ILI9341_SET_BLANKING_PORCH_CTL);
// ili9341write_data(0x7F);
// ili9341write_data(0x7F);
// ili9341write_data(0x1F);
// ili9341write_data(0x1F);
// chThdSleepMilliseconds(1);
// ili9341write_cmd(ILI9341_SET_GAMMA);
// ili9341write_data(0x01); 
// chThdSleepMilliseconds(1);
// ili9341write_cmd(ILI9341_SET_PGAMMA);
// for (int i=0;i<15;i++) ili9341write_data(pgamma[i]);
// chThdSleepMilliseconds(1);
// ili9341write_cmd(ILI9341_SET_NGAMMA);
// for (int i=0;i<15;i++) ili9341write_data(ngamma[i]);
// chThdSleepMilliseconds(1);
ili9341write_cmd(ILI9341_CMD_SLEEP_OFF);
chThdSleepMilliseconds(150);
ili9341write_cmd(ILI9341_CMD_DISPLAY_ON);
palSetPad(GPIOB, GPIOB_PIN1);
}
 
void ili9341_set_window( uint16_t x0,uint16_t y0, uint16_t x1,uint16_t y1) {
ili9341write_cmd(ILI9341_SET_PAGE_ADDR); // Page Address Set
ili9341write_data(y0 >> 8);
ili9341write_data(y0 & 0xFF);
ili9341write_data(y1 >> 8);
ili9341write_data(y1 & 0xFF);
ili9341write_cmd(ILI9341_SET_COL_ADDR); // Column Address Set
ili9341write_data(x0 >> 8);
ili9341write_data(x0 & 0xFF);
ili9341write_data(x1 >> 8);
ili9341write_data(x1 & 0xFF);
}


void SetPicture(uint16_t color) {
for (uint32_t i = 0; i < 2*X_SIZE*Y_SIZE; i++) {
  ili9341write_data(color);
  if (color!=0x0000)
  color=(color<<1);
  else color =0x0001;
  chThdSleepMicroseconds(10);
}
}
int main(void) {
halInit();
chSysInit();

// spiStart(&SPID2, &spi_cfg2);
// spiSelect(&SPID2);

sramInit();
sramStart(&SRAMD1, &cfg);

// ili9341ObjectInit(&ILI9341D1);
// ili9341Start(&ILI9341D1, &ili9341_cfg);
//initialize_lcd();
initialize_lcd_fsmc();
chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
ili9341_set_window(0,0,X_SIZE-1,Y_SIZE-1);
ili9341write_cmd(ILI9341_SET_MEM);
chThdSleepMicroseconds(1);
while (true) {
SetPicture(0x0001);
}       
}
