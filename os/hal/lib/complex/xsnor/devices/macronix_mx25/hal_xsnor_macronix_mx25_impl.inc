/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

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

/* This is an, automatically generated, implementation file that can be
   manually edited, it is not re-generated if already present.*/

#define PAGE_SIZE                                   256U
#define PAGE_MASK                                   (PAGE_SIZE - 1U)
/**
 * @name    Command codes, SPI mode
 * @{
 */
#define CMD_SPI_READ3B                              0x03U
#define CMD_SPI_FAST_READ3B                         0x0BU
#define CMD_SPI_PP3B                                0x02U
#define CMD_SPI_SE3B                                0x20U
#define CMD_SPI_BE3B                                0xD8U
#define CMD_SPI_CE                                  0xC7U
#define CMD_SPI_READ4B                              0x13U
#define CMD_SPI_FAST_READ4B                         0x0CU
#define CMD_SPI_PP4B                                0x12U
#define CMD_SPI_SE4B                                0x21U
#define CMD_SPI_BE4B                                0xDCU
#define CMD_SPI_WREN                                0x06U
#define CMD_SPI_WRDI                                0x04U
#define CMD_SPI_PE_SUSPEND                          0xB0U
#define CMD_SPI_PE_RESUME                           0x30U
#define CMD_SPI_DP                                  0xB9U
#define CMD_SPI_SBL                                 0xC0U
#define CMD_SPI_ENSO                                0xB1U
#define CMD_SPI_EXSO                                0xC1U
#define CMD_SPI_NOP                                 0x00U
#define CMD_SPI_RSTEN                               0x66U
#define CMD_SPI_RST                                 0x99U
#define CMD_SPI_RDID                                0x9FU
#define CMD_SPI_RDSFDP                              0x5AU
#define CMD_SPI_RDSR                                0x05U
#define CMD_SPI_RDCR                                0x15U
#define CMD_SPI_WRSR                                0x01U
#define CMD_SPI_RDCR2                               0x71U
#define CMD_SPI_WRCR2                               0x72U
#define CMD_SPI_RDFBR                               0x16U
#define CMD_SPI_WRFBR                               0x17U
#define CMD_SPI_ESFBR                               0x18U
#define CMD_SPI_RDSCUR                              0x2BU
#define CMD_SPI_WRSCUR                              0x2FU
#define CMD_SPI_WRLR                                0x2CU
#define CMD_SPI_RDLR                                0x2DU
#define CMD_SPI_WRSPB                               0xE3U
#define CMD_SPI_ESSPB                               0xE4U
#define CMD_SPI_RDSPB                               0xE2U
#define CMD_SPI_WRDPB                               0xE1U
#define CMD_SPI_RDDPB                               0xE0U
#define CMD_SPI_WPSEL                               0x68U
#define CMD_SPI_GBLK                                0x7EU
#define CMD_SPI_GBULK                               0x98U
#define CMD_SPI_RDPASS                              0x27U
#define CMD_SPI_WRPASS                              0x28U
#define CMD_SPI_PASSULK                             0x29U
/** @} */

/**
 * @name    Command codes, OPI mode
 * @{
 */
#define CMD_OPI_8READ                               0xEC13U
#define CMD_OPI_8DTRD                               0xEE11U
#define CMD_OPI_RDID                                0x9F60U
#define CMD_OPI_RDSFDP                              0x5AA5U
#define CMD_OPI_PP                                  0x12EDU
#define CMD_OPI_SE                                  0x21DEU
#define CMD_OPI_BE                                  0xDC23U
#define CMD_OPI_CE                                  0xC738U
#define CMD_OPI_WREN                                0x06F9U
#define CMD_OPI_WRDI                                0x04FBU
#define CMD_OPI_PE_SUSPEND                          0xB04FU
#define CMD_OPI_PE_RESUME                           0x30CFU
#define CMD_OPI_DP                                  0xB946U
#define CMD_OPI_SBL                                 0xC03FU
#define CMD_OPI_ENSO                                0xB14EU
#define CMD_OPI_EXSO                                0xC13EU
#define CMD_OPI_NOP                                 0x00FFU
#define CMD_OPI_RSTEN                               0x6699U
#define CMD_OPI_RST                                 0x9966U
#define CMD_OPI_RDSR                                0x05FAU
#define CMD_OPI_RDCR                                0x15EAU
#define CMD_OPI_WRSR                                0x01FEU
#define CMD_OPI_WRCR                                0x01FEU
#define CMD_OPI_RDCR2                               0x718EU
#define CMD_OPI_WRCR2                               0x728DU
#define CMD_OPI_RDFBR                               0x16E9U
#define CMD_OPI_WRFBR                               0x17E8U
#define CMD_OPI_ESFBR                               0x18E7U
#define CMD_OPI_RDSCUR                              0x2BD4U
#define CMD_OPI_WRSCUR                              0x2FD0U
#define CMD_OPI_WRLR                                0x2CD3U
#define CMD_OPI_RDLR                                0x2DD2U
#define CMD_OPI_WRSPB                               0xE31CU
#define CMD_OPI_ESSPB                               0xE41BU
#define CMD_OPI_RDSPB                               0xE21DU
#define CMD_OPI_WRDPB                               0xE11EU
#define CMD_OPI_RDDPB                               0xE01FU
#define CMD_OPI_WPSEL                               0x6897U
#define CMD_OPI_GBLK                                0x7E81U
#define CMD_OPI_GBULK                               0x9867U
#define CMD_OPI_RDPASS                              0x27D8U
#define CMD_OPI_WRPASS                              0x28D7U
#define CMD_OPI_PASSULK                             0x29D6U
/** @} */

/**
 * @name    Flags status register bits
 * @{
 */
#define FLAGS_WPSEL                                 0x80U
#define FLAGS_E_FAIL                                0x40U
#define FLAGS_P_FAIL                                0x20U
#define FLAGS_ESB                                   0x08U
#define FLAGS_PSB                                   0x04U
#define FLAGS_LDSO                                  0x02U
#define FLAGS_SECURED_OTP                           0x01U
#define FLAGS_ALL_ERRORS                            (FLAGS_E_FAIL | FLAGS_P_FAIL)
/** @} */

#if (XSNOR_USE_WSPI == TRUE) || defined(__DOXYGEN__)
static const xsnor_commands_t cmd_spi = {
  .cmd                  = (WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE),
  .cmd_addr             = (WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32            |
                           WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_ONE_LINE      |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE),
  .cmd_data             = (WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_ONE_LINE),
  .cmd_addr_data        = (WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32            |
                           WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_ONE_LINE      |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_ONE_LINE)
};

static const xsnor_commands_t cmd_8str = {
  .cmd                  = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE),
  .cmd_addr             = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_ADDR_SIZE_32            |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_EIGHT_LINES   |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE),
  .cmd_data             = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_EIGHT_LINES),
  .cmd_addr_data        = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_ADDR_SIZE_32            |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_EIGHT_LINES   |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_EIGHT_LINES)
};

static const xsnor_commands_t cmd_8dtr = {
  .cmd                  = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_ALL_DTR),
  .cmd_addr             = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_ADDR_SIZE_32            |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_EIGHT_LINES   |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_ALL_DTR),
  .cmd_data             = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_EIGHT_LINES          |
                           WSPI_CFG_ALL_DTR),
  .cmd_addr_data        = (WSPI_CFG_CMD_SIZE_16             |
                           WSPI_CFG_ADDR_SIZE_32            |
                           WSPI_CFG_CMD_MODE_EIGHT_LINES    |
                           WSPI_CFG_ADDR_MODE_EIGHT_LINES   |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_EIGHT_LINES          |
                           WSPI_CFG_ALL_DTR)
};
#endif /* XSNOR_USE_WSPI == TRUE */

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module class "hal_xsnor_macronix_mx25_c" methods.                         */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_xsnor_macronix_mx25_c
 * @{
 */
/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__mx25_objinit_impl(void *ip, const void *vmt) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __xsnor_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->descriptor.attributes    = FLASH_ATTR_ERASED_IS_ONE |
                                   FLASH_ATTR_REWRITABLE |
                                   FLASH_ATTR_SUSPEND_ERASE_CAPABLE;
  self->descriptor.page_size     = 256U;
  self->descriptor.sectors_count = 0U; /* Overwritten.*/
  self->descriptor.sectors       = NULL;
  if ((self->config->options & MX25_OPT_USE_SUBSECTORS) != 0U) {
    self->descriptor.sectors_size = 0x00001000U;
  }
  else {
    self->descriptor.sectors_size = 0x00010000U;
  }
  self->descriptor.address       = 0U;
  self->descriptor.size          = 0U; /* Overwritten.*/

  return self;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance to be disposed.
 */
void __mx25_dispose_impl(void *ip) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Finalization code.*/
  /* Implementation.*/

  /* Finalization of the ancestors-defined parts.*/
  __xsnor_dispose_impl(self);
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_init().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @return                      An error code.
 */
flash_error_t __mx25_init_impl(void *ip) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;
  const xsnor_config_t *config = self->config;

  /* Bus type and width setup, only necessary if WSPI is in use.*/
#if XSNOR_USE_WSPI == TRUE
  switch (config->bus_type) {
  case XSNOR_BUS_MODE_SPI:
    self->commands = NULL;
    break;
  case XSNOR_BUS_MODE_WSPI_8LINES:
    if ((self->config->options &MX25_OPT_USE_DTR) != 0U) {
      self->commands = &cmd_8str;
    }
    else {
      self->commands = &cmd_8dtr;
    }
    break;
  default:
    osalDbgAssert(false, "invalid bus type");
    self->commands = NULL;
    return FLASH_ERROR_HW_FAILURE;
  }
#endif

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_read().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 */
flash_error_t __mx25_read_impl(void *ip, flash_offset_t offset, size_t n,
                               uint8_t *rp) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;
  (void)offset;
  (void)n;
  (void)rp;

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_program().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 */
flash_error_t __mx25_program_impl(void *ip, flash_offset_t offset, size_t n,
                                  const uint8_t *pp) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;
  (void)offset;
  (void)n;
  (void)pp;

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_start_erase_all().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @return                      An error code.
 */
flash_error_t __mx25_start_erase_all_impl(void *ip) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_start_erase_sector().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 */
flash_error_t __mx25_start_erase_sector_impl(void *ip, flash_sector_t sector) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;
  (void)sector;

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_query_erase().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @param[out]    msec          Recommended time, in milliseconds, that should
 *                              be spent before calling this function again,
 *                              can be @p NULL
 * @return                      An error code.
 */
flash_error_t __mx25_query_erase_impl(void *ip, unsigned *msec) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;
  (void)msec;

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_verify_erase().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @param[in]     sector        Sector to be verified.
 * @return                      An error code.
 */
flash_error_t __mx25_verify_erase_impl(void *ip, flash_sector_t sector) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;
  (void)sector;

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_mmap_on().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 * @param[out]    addrp         Pointer to the memory mapped memory or @p NULL
 * @return                      An error code.
 */
flash_error_t __mx25_mmap_on_impl(void *ip, uint8_t **addrp) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;
  (void)addrp;

  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_xsnor_macronix_mx25_c
 * @protected
 *
 * @brief       Override of method @p snor_device_mmap_off().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_macronix_mx25_c
 *                              instance.
 */
void __mx25_mmap_off_impl(void *ip) {
  hal_xsnor_macronix_mx25_c *self = (hal_xsnor_macronix_mx25_c *)ip;

  /* Implementation.*/
  (void)self;
}
/** @} */

/**
 * @brief       VMT structure of SNOR Macronix MX25 driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_xsnor_macronix_mx25_vmt __hal_xsnor_macronix_mx25_vmt = {
  .dispose                  = __mx25_dispose_impl,
  .init                     = __mx25_init_impl,
  .read                     = __mx25_read_impl,
  .program                  = __mx25_program_impl,
  .start_erase_all          = __mx25_start_erase_all_impl,
  .start_erase_sector       = __mx25_start_erase_sector_impl,
  .query_erase              = __mx25_query_erase_impl,
  .verify_erase             = __mx25_verify_erase_impl,
  .mmap_on                  = __mx25_mmap_on_impl,
  .mmap_off                 = __mx25_mmap_off_impl
};

