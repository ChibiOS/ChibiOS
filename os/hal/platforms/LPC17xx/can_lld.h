/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

/**
 * @file    LPC17xx/can_lld.h
 * @brief   LPC17xx CAN subsystem low level driver header.
 *
 * @addtogroup CAN
 * @{
 */

/*
  This file has been contributed by:
                Marcin Jokel.
*/

#ifndef _CAN_LLD_H_
#define _CAN_LLD_H_

#if HAL_USE_CAN || defined(__DOXYGEN__)
/* TODO: FullCAN mode. */
/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 *  CAN Mode register bits.
 */
#define CANMOD_RM                   (1UL << 0)
#define CANMOD_LOM                  (1UL << 1)
#define CANMOD_STM                  (1UL << 2)
#define CANMOD_TPM                  (1UL << 3)
#define CANMOD_SM                   (1UL << 4)
#define CANMOD_RPM                  (1UL << 5)
#define CANMOD_TM                   (1UL << 7)

/*
 *  CAN Command register bits.
 */
#define CANCMR_TR                   (1UL << 0)
#define CANCMR_AT                   (1UL << 1)
#define CANCMR_RRB                  (1UL << 2)
#define CANCMR_CDO                  (1UL << 3)
#define CANCMR_SRR                  (1UL << 4)
#define CANCMR_STB1                 (1UL << 5)
#define CANCMR_STB2                 (1UL << 6)
#define CANCMR_STB3                 (1UL << 7)

/*
 *  CAN Global Status register bits.
 */
#define CANGSR_RBS                  (1UL << 0)
#define CANGSR_DOS                  (1UL << 1)
#define CANGSR_TBS                  (1UL << 2)
#define CANGSR_TCS                  (1UL << 3)
#define CANGSR_RS                   (1UL << 4)
#define CANGSR_TS                   (1UL << 5)
#define CANGSR_ES                   (1UL << 6)
#define CANGSR_BS                   (1UL << 7)
#define CANGSR_RXERR                0x00FF0000
#define CANGSR_TXERR                0xFF000000

/*
 *  CAN Interrupt and Capture register bits.
 */
#define CANICR_RI                   (1UL << 0)
#define CANICR_TI1                  (1UL << 1)
#define CANICR_EI                   (1UL << 2)
#define CANICR_DOI                  (1UL << 3)
#define CANICR_WUI                  (1UL << 4)
#define CANICR_EPI                  (1UL << 5)
#define CANICR_ALI                  (1UL << 6)
#define CANICR_BEI                  (1UL << 7)
#define CANICR_IDI                  (1UL << 8)
#define CANICR_TI2                  (1UL << 9)
#define CANICR_TI3                  (1UL << 10)
#define CANICR_ERRBIT               0x001F0000
#define CANICR_ERRBIT_SOF           0x00030000
#define CANICR_ERRBIT_ID28_ID21     0x00020000
#define CANICR_ERRBIT_ID20_ID18     0x00060000
#define CANICR_ERRBIT_SRTR          0x00040000
#define CANICR_ERRBIT_IDE           0x00050000
#define CANICR_ERRBIT_ID17_ID13     0x00070000
#define CANICR_ERRBIT_ID12_ID5      0x000F0000
#define CANICR_ERRBIT_ID4_ID0       0x000E0000
#define CANICR_ERRBIT_RTR           0x000C0000
#define CANICR_ERRBIT_RES_BIT1      0x000D0000
#define CANICR_ERRBIT_RES_BIT0      0x00090000
#define CANICR_ERRBIT_DATA_LEN_CODE 0x000B0000
#define CANICR_ERRBIT_DATA_FIELD    0x000A0000
#define CANICR_ERRBIT_CRC_SEQ       0x00080000
#define CANICR_ERRBIT_CRC_DEL       0x00180000
#define CANICR_ERRBIT_ACK_SLOT      0x00190000
#define CANICR_ERRBIT_ACK_DEL       0x001B0000
#define CANICR_ERRBIT_EOF           0x001A0000
#define CANICR_ERRBIT_INTERMISSION  0x00120000
#define CANICR_ERRBIT_ACTIVE_ERROR  0x00110000
#define CANICR_ERRBIT_PASSIVE_ERROR 0x00160000
#define CANICR_ERRBIT_TOL_DOM_BITS  0x00130000
#define CANICR_ERRBIT_ERROR_DEL     0x00170000
#define CANICR_ERRBIT_OVERLOAD      0x001C0000
#define CANICR_ERRDIR               (1UL << 21)
#define CANICR_ERRC                 (3UL << 22)
#define CANICR_ERRC_BIT_ERR         (0UL << 22)
#define CANICR_ERRC_FORM_ERR        (1UL << 22)
#define CANICR_ERRC_STUFF_ERR       (2UL << 22)
#define CANICR_ERRC_OTHER_ERR       (3UL << 22)
#define CANICR_ALCBIT               0xFF000000

/*
 *  CAN Interrupt Enable register bits.
 */
#define CANIER_RIE                  (1UL << 0)
#define CANIER_TIE1                 (1UL << 1)
#define CANIER_EIE                  (1UL << 2)
#define CANIER_DOIE                 (1UL << 3)
#define CANIER_WUIE                 (1UL << 4)
#define CANIER_EPIE                 (1UL << 5)
#define CANIER_ALIE                 (1UL << 6)
#define CANIER_BEIE                 (1UL << 7)
#define CANIER_IDIE                 (1UL << 8)
#define CANIER_TIE2                 (1UL << 9)
#define CANIER_TIE3                 (1UL << 10)

/*
 *   CAN Bus Timing register bits.
 */
#define CANBTR_BRP(t)               ((t) << 0)
#define CANBTR_SJW(t)               ((t) << 14)
#define CANBTR_TESG1(t)             ((t) << 16)
#define CANBTR_TESG2(t)             ((t) << 20)
#define CANBTR_SAM                  (1UL << 23)

/*
 *    CAN Status register bits.
 */
#define CANSR_RBS                   (1UL << 0)
#define CANSR_DOS                   (1UL << 1)
#define CANSR_TBS1                  (1UL << 2)
#define CANSR_TCS1                  (1UL << 3)
#define CANSR_RS                    (1UL << 4)
#define CANSR_TS1                   (1UL << 5)
#define CANSR_ES                    (1UL << 6)
#define CANSR_BS                    (1UL << 7)
#define CANSR_TBS2                  (1UL << 10)
#define CANSR_TCS2                  (1UL << 11)
#define CANSR_TS2                   (1UL << 13)
#define CANSR_TBS3                  (1UL << 18)
#define CANSR_TCS3                  (1UL << 19)
#define CANSR_TS3                   (1UL << 21)

/*
 *    CAN Receive Frame Status register bits.
 */
#define CANRFS_ID                   0x000001FF
#define CANRFS_BP                   (1UL << 10)
#define CANRFS_DLC                  0x000F0000
#define CANRFS_RTR                  (1UL << 30)
#define CANRFS_FF                   (1UL << 31)

/*
 *    CAN Transmit Frame Information register bits.
 */
#define CANTFI_PRIO(p)              ((p) << 0)
#define CANTFI_DLC(l)               ((l) << 16)
#define CANTFI_RTR                  (1UL << 30)
#define CANTFI_FF                   (1UL << 31)

/*
 *    CAN Sleep Clear register bits.
 */
#define CANSLEEPCLR_CAN1SLEEP       (1UL << 1)
#define CANSLEEPCLR_CAN2SLEEP       (1UL << 2)

/*
 *    CAN Wake-up Flags register bits.
 */
#define CANWAKEFLAGS_CAN1WAKE       (1UL << 1)
#define CANWAKEFLAGS_CAN2WAKE       (1UL << 2)

/*
 *    Central Transmit Status register bits.
 */
#define CANTSR_TS1                  (1UL << 0)
#define CANTSR_TS2                  (1UL << 1)
#define CANTSR_TBS1                 (1UL << 8)
#define CANTSR_TBS2                 (1UL << 9)
#define CANTSR_TCS1                 (1UL << 16)
#define CANTSR_TCS2                 (1UL << 17)

/*
 *     Central Receive Status register bits.
 */
#define CANRSR_RS1                  (1UL << 0)
#define CANRSR_RS2                  (1UL << 1)
#define CANRSR_RB1                  (1UL << 8)
#define CANRSR_RB2                  (1UL << 9)
#define CANRSR_DOS1                 (1UL << 16)
#define CANRSR_DOS2                 (1UL << 17)

/*
 *     Central Miscellaneous Status register bits.
 */
#define CANMSR_E1                   (1UL << 0)
#define CANMSR_E2                   (1UL << 1)
#define CANMSR_BS1                  (1UL << 8)
#define CANMSR_BS2                  (1UL << 9)

/*
 *     Acceptance Filter Mode register bits.
 */
#define AFMR_ACC_OFF                (1UL << 0)
#define AFMR_ACC_BP                 (1UL << 1)
#define AFMR_E_FCAN                 (1UL << 2)

/**
 * @brief   This switch defines whether the driver implementation supports
 *          a low power switch mode with automatic an wakeup feature.
 */
#define CAN_SUPPORTS_SLEEP          TRUE

/**
 * @brief   This implementation supports three transmit mailboxes.
 */
#define CAN_TX_MAILBOXES            3

/**
 * @brief   This implementation supports two receive mailboxes.
 */
#define CAN_RX_MAILBOXES            1

/** @} */

/**
 * @name    CAN registers helper macros
 * @{
 */
#define CAN_IDE_STD                 0           /**< @brief Standard id.    */
#define CAN_IDE_EXT                 1           /**< @brief Extended id.    */

#define CAN_RTR_DATA                0           /**< @brief Data frame.     */
#define CAN_RTR_REMOTE              1           /**< @brief Remote frame.   */

#undef CAN_LIMIT_WARNING
#undef CAN_LIMIT_ERROR
#undef CAN_BUS_OFF_ERROR
#undef CAN_FRAMING_ERROR

#define CAN_WARNING_ERROR           1
#define CAN_PASSIVE_ERROR           2
#define CAN_BUS_ERROR               4

/**
 * @brief   CAN filter fullCAN (standard identifier) type entry.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
#define CANFilterFCEntry(ctrl_i1, dis_i1, id_i1, ctrl_i2, dis_i2, id_i2)                       \
  ((((uint32_t)ctrl_i1) << 29) | (((uint32_t)dis_i1) << 28) | (((uint32_t)id_i1) << 16)   |     \
   (((uint32_t)ctrl_i2) << 13)  | (((uint32_t)dis_i2) << 12)  | ((uint32_t)(id_i2)))

/**
 * @brief   CAN filter standard identifier type entry.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
#define CANFilterStdEntry(ctrl_i1, dis_i1, id_i1, ctrl_i2, dis_i2, id_i2)                       \
  ((((uint32_t)ctrl_i1) << 29) | (((uint32_t)dis_i1) << 28) | (((uint32_t)id_i1) << 16)   |     \
   (((uint32_t)ctrl_i2) << 13)  | (((uint32_t)dis_i2) << 12)  | ((uint32_t)(id_i2)))
/**
 * @brief   CAN filter extended identifier range type entry.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
#define CANFilterStdRangeEntry(ctrl_low, dis_low, id_low, ctrl_high, dis_high, id_high)         \
  ((((uint32_t)ctrl_low) << 29) | (((uint32_t)dis_low) << 28) | (((uint32_t)id_low) << 16)   |  \
   (((uint32_t)ctrl_up) << 13)  | (((uint32_t)dis_up) << 12)  | ((uint32_t)(id_up)))

/**
 * @brief   CAN filter extended identifier type entry.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
#define CANFilterExtEntry(ctrl, id) \
  ((((uint32_t)ctrl) << 29) | ((uint32_t)(id)))

/**
 * @brief   CAN filter extended range identifier type entry.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
#define CANFilterExtRangeEntry(ctrl, id) \
  ((((uint32_t)ctrl) << 29) | ((uint32_t)(id)))
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   CAN1 driver enable switch.
 * @details If set to @p TRUE the support for CAN1 is included.
 */
#if !defined(LPC17xx_CAN_USE_CAN1) || defined(__DOXYGEN__)
#define LPC17xx_CAN_USE_CAN1                  FALSE
#endif

/**
 * @brief   CAN2 driver enable switch.
 * @details If set to @p TRUE the support for CAN2 is included.
 */
#if !defined(LPC17xx_CAN_USE_CAN2) || defined(__DOXYGEN__)
#define LPC17xx_CAN_USE_CAN2                  FALSE
#endif

/**
 * @brief   CAN1 and CAN2 interrupt priority level setting.
 */
#if !defined(LPC17xx_CAN_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC17xx_CAN_IRQ_PRIORITY              11
#endif
/** @} */

/**
 * @brief   CAN1 and CAN2 local self test enable.
 */
#if !defined(LPC17xx_CAN_USE_LOCAL_SELF_TEST) || defined(__DOXYGEN__)
#define LPC17xx_CAN_USE_LOCAL_SELF_TEST       FALSE
#endif
/** @} */

/**
 * @brief   CAN1 and CAN2 acceptance filter enable.
 */
#if !defined(LPC17xx_CAN_USE_FILTER) || defined(__DOXYGEN__)
#define LPC17xx_CAN_USE_FILTER              FALSE
#endif
/** @} */

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if LPC17xx_CAN_USE_CAN1 && !LPC17xx_HAS_CAN1
#error "CAN1 not present in the selected device"
#endif

#if LPC17xx_CAN_USE_CAN2 && !LPC17xx_HAS_CAN2
#error "CAN2 not present in the selected device"
#endif

#if !LPC17xx_CAN_USE_CAN1 && !LPC17xx_CAN_USE_CAN2
#error "CAN driver activated but no CAN peripheral assigned"
#endif

#if !LPC17xx_CAN_USE_CAN1 && LPC17xx_CAN_USE_CAN2
#error "CAN2 requires CAN1, it cannot operate independently"
#endif

#if CAN_USE_SLEEP_MODE && !CAN_SUPPORTS_SLEEP
#error "CAN sleep mode not supported in this architecture"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a transmission mailbox index.
 */
typedef uint32_t canmbx_t;

/**
 * @brief   CAN transmission frame.
 * @note    Accessing the frame data as word16 or word32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  struct {
    uint8_t                 DLC:4;          /**< @brief Data length.        */
    uint8_t                 RTR:1;          /**< @brief Frame type.         */
    uint8_t                 IDE:1;          /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              SID:11;         /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              EID:29;         /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 data8[8];       /**< @brief Frame data.         */
    uint16_t                data16[4];      /**< @brief Frame data.         */
    uint32_t                data32[2];      /**< @brief Frame data.         */
  };
} CANTxFrame;

/**
 * @brief   CAN received frame.
 * @note    Accessing the frame data as word16 or word32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  struct {
    uint16_t                IDF:10;          /**< @brief Filter id.          */
  };
  struct {
    uint8_t                 DLC:4;          /**< @brief Data length.        */
    uint8_t                 RTR:1;          /**< @brief Frame type.         */
    uint8_t                 IDE:1;          /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              SID:11;         /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              EID:29;         /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 data8[8];       /**< @brief Frame data.         */
    uint16_t                data16[4];      /**< @brief Frame data.         */
    uint32_t                data32[2];      /**< @brief Frame data.         */
  };
} CANRxFrame;

/**
 * @brief   CAN filter standard identifier type.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
typedef const uint32_t CANFilterStd;

/**
 * @brief   CAN filter standard identifier range type.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
typedef const uint32_t CANFilterStdRange;

/**
 * @brief   CAN filter extended identifier range type.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
typedef  const uint32_t CANFilterExt;

/**
 * @brief   CAN filter extended range identifier type.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
typedef  const uint32_t CANFilterExtRange;

/**
 * @brief   CAN filter configuration structure.
 * @note    Refer to the LPC17xx reference manual for info about filters.
 */
typedef struct {
  /**
   * @brief   Acceptance filter mode register.
   */
  uint32_t                  afmr;
  /**
   * @brief   FullCAN (standard identifier) table.
   */
  CANFilterStd              *fc_id_table;
  /**
     * @brief  Number of positions in fullCAN (standard identifier) table.
     */
  uint32_t                  fc_id_table_n;
  /**
   * @brief   Standard identifier table.
   */
  CANFilterStd              *std_id_table;
  /**
     * @brief  Number of positions in standard identifier table.
     */
  uint32_t                  std_id_table_n;
  /**
   * @brief   Standard range identifier table.
   */
  CANFilterStdRange         *std_range_id_table;
  /**
   * @brief   Number of positions in standard range identifier table.
   */
  uint32_t                  std_range_id_table_n;
  /**
   * @brief   Extended identifier table.
   */
  CANFilterExt              *ext_id_table;
  /**
   * @brief   Number of positions in extended identifier table.
   */
  uint32_t                  ext_id_table_n;
  /**
   * @brief   Extended range identifier table.
   */
  CANFilterExtRange         *ext_range_id_table;
  /**
   * @brief   Number of positions in extended range identifier table.
   */
  uint32_t                  ext_range_id_table_n;
} CANFilterConfig;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief   CAN MOD register initialization data.
   * @note    Some bits in this register are enforced by the driver regardless
   *          their status in this field.
   */
  uint32_t                  mod;
  /**
   * @brief   CAN BTR register initialization data.
   * @note    Some bits in this register are enforced by the driver regardless
   *          their status in this field.
   */
  uint32_t                  btr;
} CANConfig;

/**
 * @brief   Structure representing an CAN driver.
 */
typedef struct {
  /**
   * @brief   Driver state.
   */
  canstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const CANConfig           *config;
  /**
   * @brief   Transmission queue semaphore.
   */
  Semaphore                 txsem;
  /**
   * @brief   Receive queue semaphore.
   */
  Semaphore                 rxsem;
  /**
   * @brief   One or more frames become available.
   * @note    After broadcasting this event it will not be broadcasted again
   *          until the received frames queue has been completely emptied. It
   *          is <b>not</b> broadcasted for each received frame. It is
   *          responsibility of the application to empty the queue by
   *          repeatedly invoking @p chReceive() when listening to this event.
   *          This behavior minimizes the interrupt served by the system
   *          because CAN traffic.
   * @note    The flags associated to the listeners will indicate which
   *          receive mailboxes become non-empty.
   */
  EventSource               rxfull_event;
  /**
   * @brief   One or more transmission mailbox become available.
   * @note    The flags associated to the listeners will indicate which
   *          transmit mailboxes become empty.
   *
   */
  EventSource               txempty_event;
  /**
   * @brief   A CAN bus error happened.
   * @note    The flags associated to the listeners will indicate the
   *          error(s) that have occurred.
   */
  EventSource               error_event;
#if CAN_USE_SLEEP_MODE || defined (__DOXYGEN__)
  /**
   * @brief   Entering sleep state event.
   */
  EventSource               sleep_event;
  /**
   * @brief   Exiting sleep state event.
   */
  EventSource               wakeup_event;
#endif /* CAN_USE_SLEEP_MODE */
  /* End of the mandatory fields.*/
  /**
   * @brief   Pointer to the CAN registers.
   */
  LPC_CAN_TypeDef               *can;
} CANDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if LPC17xx_CAN_USE_CAN1 && !defined(__DOXYGEN__)
extern CANDriver CAND1;
#endif

#if LPC17xx_CAN_USE_CAN2 && !defined(__DOXYGEN__)
extern CANDriver CAND2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void can_lld_init(void);
  void can_lld_start(CANDriver *canp);
  void can_lld_stop(CANDriver *canp);
  bool_t can_lld_is_tx_empty(CANDriver *canp,
                             canmbx_t mailbox);
  void can_lld_transmit(CANDriver *canp,
                        canmbx_t mailbox,
                        const CANTxFrame *crfp);
  bool_t can_lld_is_rx_nonempty(CANDriver *canp,
                                canmbx_t mailbox);
  void can_lld_receive(CANDriver *canp,
                       canmbx_t mailbox,
                       CANRxFrame *ctfp);
#if CAN_USE_SLEEP_MODE
  void can_lld_sleep(CANDriver *canp);
  void can_lld_wakeup(CANDriver *canp);
#endif /* CAN_USE_SLEEP_MODE */
#if LPC17xx_CAN_USE_FILTER
  void canSetFilter(const CANFilterConfig *cfc);
#endif  /* LPC17xx_CAN_USE_FILTER */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_CAN */

#endif /* _CAN_LLD_H_ */

/** @} */
