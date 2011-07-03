/**
 * This is most complex and difficult device.
 * It realize "read through write" paradigm. This is not standard, but
 * most of I2C devices use this paradigm.
 * You must write to device reading address, send restart to bus,
 * and then begin reading process.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "lis3.h"


/* buffers */
static i2cblock_t accel_rx_data[ACCEL_RX_DEPTH];
static i2cblock_t accel_tx_data[ACCEL_TX_DEPTH];

/* Error trap */
static void i2c_lis3_error_cb(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg){
  (void)i2cscfg;
  int status = 0;
  status = i2cp->id_i2c->SR1;
  while(TRUE);
}

/**
 * This treading need for convenient realize
 * "read through write" process.
 */
static WORKING_AREA(I2CAccelThreadWA, 128);
static Thread *i2c_accel_tp = NULL;
static msg_t I2CAccelThread(void *arg) {
  (void)arg;

  int16_t acceleration_x = 0;
  int16_t acceleration_y = 0;
  int16_t acceleration_z = 0;

  I2CSlaveConfig *i2cscfg;
  msg_t msg;

  while (TRUE) {
    /* Waiting for wake up */
    chSysLock();
    i2c_accel_tp = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    msg = chThdSelf()->p_msg;    /* Retrieving the message, optional.*/
    chSysUnlock();

    /***************** Perform processing here. ***************************/
    i2cscfg = (I2CSlaveConfig *)msg;

    /* collect measured data */
    acceleration_x = accel_rx_data[0] + (accel_rx_data[1] << 8);
    acceleration_y = accel_rx_data[2] + (accel_rx_data[3] << 8);
    acceleration_z = accel_rx_data[4] + (accel_rx_data[5] << 8);
  }
  return 0;
}

/* This callback raise up when transfer finished */
static void i2c_lis3_cb(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg){
  (void) i2cp;
  /* only wake up processing thread */
  if (i2c_accel_tp != NULL) {
    i2c_accel_tp->p_msg = (msg_t)i2cscfg;
    chSchReadyI(i2c_accel_tp);
    i2c_accel_tp = NULL;
  }
}


/* Accelerometer lis3lv02dq config */
static const I2CSlaveConfig lis3 = {
	i2c_lis3_cb,
  i2c_lis3_error_cb,
};


#define lis3_addr 0b0011101



/**
 * Init function. Here we will also start personal serving thread.
 */
int init_lis3(void){

  /* Starting the accelerometer serving thread.*/
  i2c_accel_tp = chThdCreateStatic(I2CAccelThreadWA,
                                  sizeof(I2CAccelThreadWA),
                                  HIGHPRIO,
                                  I2CAccelThread,
                                  NULL);

  /* wait thread statup */
  while (i2c_accel_tp == NULL)
    chThdSleepMilliseconds(1);

#define TXBYTES 4
#define RXBYTES 0 /* set to 0 because we need only transmit */

  /* configure accelerometer */
  accel_tx_data[0] = ACCEL_CTRL_REG1 | AUTO_INCREMENT_BIT; /* register address */
  accel_tx_data[1] = 0b11100111;
  accel_tx_data[2] = 0b01000001;
  accel_tx_data[3] = 0b00000000;

  /* sending */
  i2cMasterTransmit(&I2CD1, &lis3, lis3_addr, accel_tx_data, TXBYTES, accel_rx_data, RXBYTES);
  chThdSleepMilliseconds(1);

#undef RXBYTES
#undef TXBYTES

  return 0;
}

/**
 *
 */
void request_acceleration_data(void){
#define RXBYTES 6
#define TXBYTES 1
  accel_tx_data[0] = ACCEL_OUT_DATA | AUTO_INCREMENT_BIT; // register address
  i2cAcquireBus(&I2CD1);
  i2cMasterTransmit(&I2CD1, &lis3, lis3_addr, accel_tx_data, TXBYTES, accel_rx_data, RXBYTES);
  i2cReleaseBus(&I2CD1);
#undef RXBYTES
#undef TXBYTES
}

