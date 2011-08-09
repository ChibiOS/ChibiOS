#include <stdlib.h>
#include "ch.h"

#ifndef LIS3_H_
#define LIS3_H_



/* buffers depth */
#define ACCEL_RX_DEPTH 8
#define ACCEL_TX_DEPTH 8

/* autoincrement bit position. This bit needs to perform reading of
 * multiple bytes at one request */
#define AUTO_INCREMENT_BIT (1<<7)

/* slave specific addresses */
#define ACCEL_STATUS_REG  0x27
#define ACCEL_CTRL_REG1   0x20
#define ACCEL_OUT_DATA    0x28



inline int init_lis3(void);
inline void request_acceleration_data(void);


#endif /* LIS3_H_ */
