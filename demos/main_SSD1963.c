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
#define ADDR_DATA *(volatile uint16_t *) 0x60040000 //0b0000 0000 0000 0000 0000 0000 0000
#define DCX_ACTIVE  palSetPad(GPIOD,GPIOD_LED3)
#define DCX_PASSIVE palClearPad(GPIOD,GPIOD_LED3)
uint32_t X_SIZE = 864;
uint32_t Y_SIZE = 480;
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
uint16_t  HDP=863;   //849
uint16_t  HT=1050;  //1010 
uint16_t  HPS=46;   //46
uint16_t  LPS=141; //15  
uint8_t   HPW=48; //48   
uint16_t  VDP=479;   
uint16_t  VT=525;   //515
uint16_t  VPS=10;  //16 
uint16_t  FPS=10;   //8
uint8_t   VPW=8; //16

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
  .btr = 0x00000000      
};

void ili9341write_cmd(uint16_t cmd){
  DCX_PASSIVE;
  ADDR_CMD = cmd;
}

void ili9341write_data(uint16_t data){
  DCX_ACTIVE;
  ADDR_DATA = data;
}

static void initialize_lcd_fsmc(void) {

ili9341write_cmd(0x01);
chThdSleepMilliseconds(10);

ili9341write_cmd(0xE2);//На входе 21 МГц
ili9341write_data(0x07);//Умножение на 3=63 МГц
ili9341write_data(0x04);//Деление на 2=21,5 МГц
ili9341write_data(0x04);// Частота такта для дисплея 15,75 МГц(63 МГц)
chThdSleepMilliseconds(30);
ili9341write_cmd(0xE0);//enable pll
ili9341write_cmd(0x01);
chThdSleepMilliseconds(30);

ili9341write_cmd(0xE0);
ili9341write_cmd(0x03);//enable pll with pllClock
chThdSleepMilliseconds(30);

ili9341write_cmd(0xB0);
ili9341write_data(0x38);//24 бит
ili9341write_data(0x00);
ili9341write_data((HDP>>8)&0x00FF);
ili9341write_data(HDP&0x00FF);
ili9341write_data((VDP>>8)&0x00FF);
ili9341write_data(VDP&0x00FF);
ili9341write_data(0x00);
chThdSleepMilliseconds(10);

ili9341write_cmd(0xF0);// Настройка формата пикселей (16 бит)
ili9341write_data(0x03);
chThdSleepMilliseconds(10);

ili9341write_cmd(0x36);
ili9341write_data(0x00);

ili9341write_cmd(0xE6);//pixel clock freguence
ili9341write_data(0x00);
ili9341write_data(0xD9);
ili9341write_data(0x16);//21,5 МГц *(55575)/(1024*1024)=1,14 МГц


ili9341write_cmd(0xB4);
ili9341write_data((HT>>8)&0x00FF);
ili9341write_data(HT&0x00FF);
ili9341write_data((HPS>>8)&0x00FF);
ili9341write_data(HPS&0x00FF);
ili9341write_data(HPW);
ili9341write_data((LPS>>8)&0x00FF);
ili9341write_data(LPS&0x00FF);
chThdSleepMilliseconds(10);

ili9341write_cmd(0xB6);
ili9341write_data((VT>>8)&0x00FF);
ili9341write_data(VT&0x00FF);
ili9341write_data((VPS>>8)&0x00FF);
ili9341write_data(VPS&0x00FF);
ili9341write_data(VPW);
ili9341write_data((FPS>>8)&0x00FF);
ili9341write_data(FPS&0x00FF);
chThdSleepMilliseconds(10);


ili9341write_cmd(0x11);
chThdSleepMilliseconds(200);


// ili9341write_cmd(0x26);
// ili9341write_data(0x08);
// chThdSleepMilliseconds(100);
ili9341write_cmd(0x29);
chThdSleepMilliseconds(150);

// ili9341write_cmd(0xD0);
// ili9341write_data(0x0D);

}
void ili9341_set_window( uint16_t x0,uint16_t y0, uint16_t x1,uint16_t y1) {
ili9341write_cmd(0x2B); // Page Address Set
ili9341write_data((y0 >> 8)&0xFF);
ili9341write_data(y0 & 0xFF);
ili9341write_data((y1 >> 8)&0xFF);
ili9341write_data(y1 & 0xFF);
ili9341write_cmd(0x2A); // Column Address Set
ili9341write_data((x0 >> 8)&0xFF);
ili9341write_data(x0 & 0xFF);
ili9341write_data((x1 >> 8)&0xFF);
ili9341write_data(x1 & 0xFF);
}

void ClearScreen(uint16_t color) {
for (uint32_t i = 0; i < 3*X_SIZE*Y_SIZE;i++) ili9341write_data(color);
}

void SetPicture() {
for (uint32_t i = 0; i < 470;i++) {
for (uint32_t j = 0; j < 850; j++)
{
ili9341write_data(wolf3d_vgagraph_chunk87[i][j]);
}
}
// for (uint32_t i = 0; i < 3*X_SIZE*Y_SIZE; i++)
// {
//   ili9341write_data(0xF800);
// }
}

int main(void) {
halInit();
chSysInit();
chThdSleepMilliseconds(200);
sramInit();
sramStart(&SRAMD1, &cfg);

initialize_lcd_fsmc();
chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
ili9341_set_window(0,0,X_SIZE-1,Y_SIZE-1);
ili9341write_cmd(0x2C);
chThdSleepMicroseconds(1);
ClearScreen(ILI9341_BLACK);
ili9341_set_window(14,5,863,474);
ili9341write_cmd(0x2C);
chThdSleepMicroseconds(1);
while (true) {
SetPicture();
}       
}

