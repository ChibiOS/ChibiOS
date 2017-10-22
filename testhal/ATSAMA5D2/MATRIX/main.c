#include "ch.h"
#include "hal.h"

/*
 * @brief Fragment to turn execution into non-secure mode
 */
void go2ns(void) {
  /* Configure SRAM0 as not secure Read and not secure write */
  mtxConfigSlaveSec(MATRIX0,H64MX_SLAVE_SRAM, LOWER_AREA_SECURABLE,
                      NOT_SECURE_READ, NOT_SECURE_WRITE);
  /* Configure SRAM1 upper area as not secure Read and secure write */
  mtxConfigSlaveSec(MATRIX0, H64MX_SLAVE_L2C_SRAM, UPPER_AREA_SECURABLE,
                    NOT_SECURE_READ, SECURE_WRITE);
  asm(
    "mrc p15, 0, r0, c1, c1, 0\n\t" /* Set NS bit into SCR register */
    "orr r0, r0, #1\n\t"            /* bit 0 is the NS bit */
    "mcr p15, 0, r0, c1, c1, 0\n\t"
  );
}
/*
 * Application entry point.
 */
int main(void) {
  /* SRAM1 split at 64K */
  mtxSetSlaveSplitAddr(MATRIX0, H64MX_SLAVE_L2C_SRAM, MATRIX_AREA_SIZE_64K,
                       REGION_0);

  uint32_t *writeNotSecureSRAM1 = (uint32_t *)(0x220000 + 61 * 1024); /* Lower area region SRAM1 */
  uint32_t *writeSecureSRAM1 = (uint32_t *)(0x220000 + 65 * 1024);    /* Upper area region SRAM1 */
  /* Go into Not Secure Mode*/
  go2ns();
  /* Writing in SRAM1 Lower Area */
  *writeNotSecureSRAM1 = 0xAA55AA55; /* writing succeeded*/
  /* Writing in SRAM1 Upper Area */
  *writeSecureSRAM1 = 0xAA55AA55;    /* writing not succeeded*/

  while (true) {
    ;
  }
}
