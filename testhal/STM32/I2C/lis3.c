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


// buffers
static i2cblock_t accel_rx_data[ACCEL_RX_DEPTH];
static i2cblock_t accel_tx_data[ACCEL_TX_DEPTH];

/* Error trap */
static void i2c_lis3_error_cb(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){
  (void)i2cscfg;
  int status = 0;
  status = i2cp->id_i2c->SR1;
  while(TRUE);
}

// Accelerometer lis3lv02dq config
static I2CSlaveConfig lis3 = {
  NULL,
  i2c_lis3_error_cb,
  accel_rx_data,
  ACCEL_RX_DEPTH,
  0,
  0,
  accel_tx_data,
  ACCEL_TX_DEPTH,
  0,
  0,
  0b0011101,
  FALSE,
};


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

  I2CDriver *i2cp;
  msg_t msg;

  while (TRUE) {
    /* Waiting for wake up */
    chSysLock();
    i2c_accel_tp = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    msg = chThdSelf()->p_msg;    /* Retrieving the message, optional.*/
    chSysUnlock();

    /***************** Perform processing here. ***************************/
    i2cp = (I2CDriver *)msg;

    /* collect measured data */
    acceleration_x = lis3.rxbuf[0] + (lis3.rxbuf[1] << 8);
    acceleration_y = lis3.rxbuf[2] + (lis3.rxbuf[3] << 8);
    acceleration_z = lis3.rxbuf[4] + (lis3.rxbuf[5] << 8);
  }
  return 0;
}



/* This callback raise up when transfer finished */
static void i2c_lis3_cb(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){

  if (i2cp->id_slave_config->restart){ // is it restart flag set to TRUE
    /* reset restart flag */
    i2cp->id_slave_config->restart = FALSE;
    /* now send restart and read acceleration data.
     * Function i2cMasterReceive() send restart implicitly. */
    i2cMasterReceive(i2cp, i2cscfg);
  }
  else{
    /* If jump here than requested data have been read.
     * Stop communication, release bus and wake up processing thread */
    i2cMasterStop(i2cp);
    i2cReleaseBus(&I2CD1);

    // wake up heavy thread for data processing
    if (i2c_accel_tp != NULL) {
      i2c_accel_tp->p_msg = (msg_t)i2cp;
      chSchReadyI(i2c_accel_tp);
      i2c_accel_tp = NULL;
    }
  }
}

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

  lis3.txbufhead = 0;
  lis3.rxbufhead = 0;


  /* Write configuration data */
  lis3.txbytes = 4;
  /* fill transmit buffer. See datasheet to understand what we write */
  lis3.txbuf[0] = ACCEL_CTRL_REG1 | AUTO_INCREMENT_BIT;
  lis3.txbuf[1] = 0b11100111;
  lis3.txbuf[2] = 0b01000001;
  lis3.txbuf[3] = 0b00000000;

  /* setting callback */
  lis3.id_callback = i2c_lis3_cb;

  i2cAcquireBus(&I2CD1);

  /* sending */
  i2cMasterTransmit(&I2CD1, &lis3);

  return 0;
}

/**
 *
 */
void request_acceleration_data(void){

  /* fill transmit buffer with address of register that we want to read */
  lis3.txbufhead = 0;
  lis3.txbuf[0] = ACCEL_OUT_DATA | AUTO_INCREMENT_BIT; // register address
  lis3.txbytes = 1;

  /* tune receive structures */
  lis3.rxbufhead = 0;
  lis3.rxbytes = 6;

  /* Now it is most important action. We must set restart flag to TRUE.
   * And in callback function we must reset it to FALSE after sending
   * of register address. In TMP75 and MAX1236 this flag does not use. */
  lis3.restart = TRUE;

  /* talk to slave what we want from it */
  i2cAcquireBus(&I2CD1);
  i2cMasterTransmit(&I2CD1, &lis3);
}

