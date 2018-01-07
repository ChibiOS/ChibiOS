#ifndef CH_SDMMC_SAMA5D2_H_
#define CH_SDMMC_SAMA5D2_H_


#define EXT_SIZE	512
#define SSR_SIZE	64
#define SCR_SIZE	8
#define SB1_SIZE	64
#define SB2_SIZE	8

/** Frequency of the board main clock oscillator */
#define BOARD_MAIN_CLOCK_EXT_OSC 12000000
#define MAIN_CLOCK_INT_OSC 12000000
#define OSC_STARTUP_TIME	0xFFu
#define MAINFRDY_TIMEOUT	32u
#define MOSCXTS_TIMEOUT		((OSC_STARTUP_TIME * 8) + 8)
#define MOSCSELS_TIMEOUT	32u


typedef enum
{
	SDMMC_SLOT0 = 0,
	SDMMC_SLOT1
}sdmmcslots_t;

#define CONFIG_HAVE_PMIC_ACT8945A 0
#define CONFIG_HAVE_PMC_PLLADIV2 1
#define CONFIG_HAVE_PMC_H32MXDIV 1

/* ========== Pio definition for SDMMC0 peripheral ========== */
#define PIO_PA13A_SDMMC0_CD    (1u << 13) /**< \brief Sdmmc0 signal: SDMMC0_CD */
#define PIO_PA11A_SDMMC0_VDDSEL (1u << 11)/**< \brief Sdmmc0 signal: SDMMC0_VDDSEL */
#define PIO_PA10A_SDMMC0_RSTN  (1u << 10) /**< \brief Sdmmc0 signal: SDMMC0_RSTN */
#define PIO_PA0A_SDMMC0_CK     (1u << 0)  /**< \brief Sdmmc0 signal: SDMMC0_CK */
#define PIO_PA1A_SDMMC0_CMD    (1u << 1)  /**< \brief Sdmmc0 signal: SDMMC0_CMD */
#define PIO_PA12A_SDMMC0_WP    (1u << 12) /**< \brief Sdmmc0 signal: SDMMC0_WP */
#define PIO_PA2A_SDMMC0_DAT0   (1u << 2)  /**< \brief Sdmmc0 signal: SDMMC0_DAT0 */
#define PIO_PA3A_SDMMC0_DAT1   (1u << 3)  /**< \brief Sdmmc0 signal: SDMMC0_DAT1 */
#define PIO_PA4A_SDMMC0_DAT2   (1u << 4)  /**< \brief Sdmmc0 signal: SDMMC0_DAT2 */
#define PIO_PA5A_SDMMC0_DAT3   (1u << 5)  /**< \brief Sdmmc0 signal: SDMMC0_DAT3 */
#define PIO_PA6A_SDMMC0_DAT4   (1u << 6)  /**< \brief Sdmmc0 signal: SDMMC0_DAT4 */
#define PIO_PA7A_SDMMC0_DAT5   (1u << 7)  /**< \brief Sdmmc0 signal: SDMMC0_DAT5 */
#define PIO_PA8A_SDMMC0_DAT6   (1u << 8)  /**< \brief Sdmmc0 signal: SDMMC0_DAT6 */
#define PIO_PA9A_SDMMC0_DAT7   (1u << 9)  /**< \brief Sdmmc0 signal: SDMMC0_DAT7 */

/* ========== Pio PIN definition for SDMMC0 peripheral ========== */


/* ========== Pio definition for SDMMC1 peripheral ========== */
#define PIO_PA30E_SDMMC1_CD    (1u << 30) /**< \brief Sdmmc1 signal: SDMMC1_CD */
#define PIO_PA27E_SDMMC1_RSTN  (1u << 27) /**< \brief Sdmmc1 signal: SDMMC1_RSTN */
#define PIO_PA22E_SDMMC1_CK    (1u << 22) /**< \brief Sdmmc1 signal: SDMMC1_CK */
#define PIO_PA28E_SDMMC1_CMD   (1u << 28) /**< \brief Sdmmc1 signal: SDMMC1_CMD */
#define PIO_PA29E_SDMMC1_WP    (1u << 29) /**< \brief Sdmmc1 signal: SDMMC1_WP */
#define PIO_PA18E_SDMMC1_DAT0  (1u << 18) /**< \brief Sdmmc1 signal: SDMMC1_DAT0 */
#define PIO_PA19E_SDMMC1_DAT1  (1u << 19) /**< \brief Sdmmc1 signal: SDMMC1_DAT1 */
#define PIO_PA20E_SDMMC1_DAT2  (1u << 20) /**< \brief Sdmmc1 signal: SDMMC1_DAT2 */
#define PIO_PA21E_SDMMC1_DAT3  (1u << 21) /**< \brief Sdmmc1 signal: SDMMC1_DAT3 */


/* mask for board capabilities defines: voltage, slot type and 8-bit support */
#define CAPS0_MASK (SDMMC_CA0R_V33VSUP | SDMMC_CA0R_V30VSUP | \
                    SDMMC_CA0R_V18VSUP | SDMMC_CA0R_SLTYPE_Msk | \
                    SDMMC_CA0R_ED8SUP)

#define BOARD_SDMMC0_CAPS0 (SDMMC_CA0R_V33VSUP | \
                            SDMMC_CA0R_V18VSUP | \
                            SDMMC_CA0R_SLTYPE_EMBEDDED | \
                            SDMMC_CA0R_ED8SUP)

#define BOARD_SDMMC1_CAPS0 (SDMMC_CA0R_V33VSUP | \
                            SDMMC_CA0R_SLTYPE_REMOVABLECARD)

#endif /* CH_SDMMC_SAMA5D2_H_ */
