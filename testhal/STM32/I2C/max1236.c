/**
 * Maxim ADC has not so suitable default settings after startup.
 * So we will create init function to tune this ADC.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "max1236.h"

/* Data buffers */
static i2cblock_t max1236_rx_data[MAX1236_RX_DEPTH];
static i2cblock_t max1236_tx_data[MAX1236_TX_DEPTH];
/* ADC results */
static uint16_t ch1 = 0, ch2 = 0, ch3 = 0, ch4 = 0;


/* Error trap */
static void i2c_max1236_error_cb(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg){
  (void)i2cscfg;
  int status = 0;
  status = i2cp->id_i2c->SR1;
  while(TRUE);
}


/* This callback raise up when transfer finished */
static void i2c_max1236_cb(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg){
  (void)*i2cp;
  (void)*i2cscfg;
  /* get ADC data */
  ch1 = ((max1236_rx_data[0] & 0xF) << 8) + max1236_rx_data[1];
  ch2 = ((max1236_rx_data[2] & 0xF) << 8) + max1236_rx_data[3];
  ch3 = ((max1236_rx_data[4] & 0xF) << 8) + max1236_rx_data[5];
  ch4 = ((max1236_rx_data[6] & 0xF) << 8) + max1236_rx_data[7];
}


/* ADC maxim MAX1236 config */

static const I2CSlaveConfig max1236 = {
		i2c_max1236_cb,
    i2c_max1236_error_cb,
};

#define max1236_addr 0b0110100

/**
 * Initilization routine. See datasheet on page 13 to understand
 * how to initialize ADC.
 */
void init_max1236(void){
  /* this data we must send via IC to setup ADC */
#define RXBYTES 0
#define TXBYTES 2
  max1236_tx_data[0] = 0b10000011; /* config register content. Consult datasheet */
  max1236_tx_data[1] = 0b00000111; /* config register content. Consult datasheet */


  /* transmit out 2 bytes */
  i2cAcquireBus(&I2CD2);
  i2cMasterTransmit(&I2CD2, &max1236, max1236_addr, max1236_tx_data, TXBYTES, max1236_rx_data, RXBYTES);
  while(I2CD2.id_state != I2C_READY){
    chThdSleepMilliseconds(1);
  }
  i2cReleaseBus(&I2CD2);
#undef RXBYTES
#undef TXBYTES
}


/* Now simply read 8 bytes to get all 4 ADC channels */
void read_max1236(void){
#define TXBYTES 0
#define RXBYTES 8

  i2cAcquireBus(&I2CD2);
  i2cMasterReceive(&I2CD2, &max1236, max1236_addr, max1236_rx_data, RXBYTES);
  i2cReleaseBus(&I2CD2);
#undef RXBYTES
#undef TXBYTES
}
