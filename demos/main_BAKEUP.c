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
unsigned int X_SIZE = 240;
unsigned int Y_SIZE = 320;
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
uint16_t  HDP=319;   //319
uint16_t  HT=397;  //554 
uint16_t  HPS=20;   //46
uint16_t  LPS=10; //15  
uint8_t   HPW=48; //48   
uint16_t  VDP=239;   //239
uint16_t  VT=253;   //267
uint16_t  VPS=2;  //16 
uint16_t  FPS=4;   //8
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
uint8_t ili9341read_data(){
  DCX_ACTIVE;
  uint8_t data = ADDR_DATA;
  return data;
}

 static void initialize_lcd_fsmc(void) {
ili9341write_cmd(ILI9341_CMD_RESET);
chThdSleepMilliseconds(10);

ili9341write_cmd(0xCB);
ili9341write_data(0x39); 
ili9341write_data(0x2C);
ili9341write_data(0x00); 
ili9341write_data(0x34);
ili9341write_data(0x02);
chThdSleepMilliseconds(10);
ili9341write_cmd(0xCF);
ili9341write_data(0x00); 
ili9341write_data(0xC1);
ili9341write_data(0x30); 
chThdSleepMilliseconds(10);
ili9341write_cmd(0xE8);
ili9341write_data(0x85); 
ili9341write_data(0x00);
ili9341write_data(0x78);
chThdSleepMilliseconds(10); 
ili9341write_cmd(0xEA);
ili9341write_data(0x00); 
ili9341write_data(0x00);
chThdSleepMilliseconds(10);
ili9341write_cmd(0xED);
ili9341write_data(0x64); 
ili9341write_data(0x03);
ili9341write_data(0x12); 
ili9341write_data(0x81);
ili9341write_cmd(0xF7);
ili9341write_data(0x20); 
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_FRAME_CTL_NORMAL);
ili9341write_data(0x00);
ili9341write_data(0x1B);// Частота кадра - 591 Гц(21 МГц/(70*1*(320+HPS+LPS)))
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_FUNCTION_CTL);
ili9341write_data(0x02);//PTG=00 NORMAL SCAN, PT=10 AGND ЗОНА без картинки
ili9341write_data(0x80);//normal white, GS=0, SM=0,SS=0
ili9341write_data(0x27);//320 LINES
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_POWER_CTL_1);//Power Control 1
ili9341write_data(0x21);
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_POWER_CTL_2);//Power Control 2
ili9341write_data(0x10);
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_VCOM_CTL_1);//VCOM Control 1
ili9341write_data(0x3E);
ili9341write_data(0x28);
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_VCOM_CTL_2);//VCOM Control 1
ili9341write_data(0x86);
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_MEM_ACS_CTL);
ili9341write_data(0x00);
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_RGB_IF_SIG_CTL);
ili9341write_data(0xE0);//RCM=11 
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_PIX_FMT);// Настройка формата пикселей (16 бит)
ili9341write_data(0x55);
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_IF_CTL);
ili9341write_data(0x01);
ili9341write_data(0x10);
ili9341write_data(0x09);//VSYNC interface mode,System interface/VSYNC interface,16- bit RGB interface (1 transfer/pixel)- SYNC MODE
chThdSleepMilliseconds(10);
ili9341write_cmd(ILI9341_SET_BLANKING_PORCH_CTL);
ili9341write_data(HPS);
ili9341write_data(LPS);
ili9341write_data(VPS);
ili9341write_data(FPS);
ili9341write_cmd(ILI9341_CMD_SLEEP_OFF);
chThdSleepMilliseconds(150);
ili9341write_cmd(ILI9341_CMD_DISPLAY_ON);
chThdSleepMilliseconds(150);
palSetPad(GPIOB, GPIOB_PIN1);
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
for (uint32_t i = 0; i < 3*X_SIZE*Y_SIZE;i++) {
ili9341write_data(color);
}
}
int main(void) {

  halInit();
  chSysInit();
  chThdSleepMilliseconds(200);
  sramInit();
  sramStart(&SRAMD1, &cfg);
  initialize_lcd_fsmc();
  ili9341_set_window(0,0,X_SIZE-1,Y_SIZE-1);
  ili9341write_cmd(0x2C);
  chThdSleepMicroseconds(1);
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL); 
  while (true) {
  ClearScreen(ILI9341_RED);          
}
}