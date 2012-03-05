/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    stm32_otg.h
 * @brief   STM32 OTG registers layout header.
 *
 * @addtogroup USB
 * @{
 */

#ifndef _STM32_OTG_H_
#define _STM32_OTG_H_

/**
 * @brief   number of the implemented endpoints.
 * @details This value does not include the endpoint 0 that is always present.
 */
#define STM32_OTG_ENDOPOINTS_NUMBER     3

/**
 * @brief   Host channel registers group.
 */
typedef struct {
  volatile uint32_t HCCHAR;     /**< @brief Host channel characteristics
                                            register.                       */
  volatile uint32_t resvd8;
  volatile uint32_t HCINT;      /**< @brief Host channel interrupt register.*/
  volatile uint32_t HCINTMSK;   /**< @brief Host channel interrupt mask
                                            register.                       */
  volatile uint32_t HCTSIZ;     /**< @brief Host channel transfer size
                                            register.                       */
  volatile uint32_t resvd14;
  volatile uint32_t resvd18;
  volatile uint32_t resvd1c;
} stm32_otg_host_chn_t;

/**
 * @brief   Device Input endpoint registers group.
 */
typedef struct {
  volatile uint32_t DIEPCTL;    /**< @brief Device control IN endpoint control
                                            register.                       */
  volatile uint32_t resvd4;
  volatile uint32_t DIEPINT;    /**< @brief Device IN endpoint interrupt
                                            register.                       */
  volatile uint32_t resvdC;
  volatile uint32_t DIEPTSIZ;   /**< @brief Device IN endpoint transfer size
                                            register.                       */
  volatile uint32_t resvd14;
  volatile uint32_t DTXFSTS;    /**< @brief Device IN endpoint transmit FIFO
                                            status register.                */
  volatile uint32_t resvd1C;
} stm32_otg_in_ep_t;

/**
 * @brief   Device Output endpoint registers group.
 */
typedef struct {
  volatile uint32_t DOEPCTL;    /**< @brief Device control OUT endpoint control
                                            register.                       */
  volatile uint32_t resvd4;
  volatile uint32_t DOEPINT;    /**< @brief Device OUT endpoint interrupt
                                            register.                       */
  volatile uint32_t resvdC;
  volatile uint32_t DOEPTSIZ;   /**< @brief Device OUT endpoint transfer size
                                            register.                       */
  volatile uint32_t resvd14;
  volatile uint32_t resvd18;
  volatile uint32_t resvd1C;
} stm32_t_out_ep_t;

/**
 * @brief   USB registers memory map.
 */
typedef struct {
  volatile uint32_t GOTGCTL;    /**< @brief OTG control and status register.*/
  volatile uint32_t GOTGINT;    /**< @brief OTG interrupt register.         */
  volatile uint32_t GAHBCFG;    /**< @brief AHB configuration register.     */
  volatile uint32_t GUSBCFG;    /**< @brief USB configuration register.     */
  volatile uint32_t GRSTCTL;    /**< @brief Reset register size.            */
  volatile uint32_t GINTSTS;    /**< @brief Interrupt register.             */
  volatile uint32_t GINTMSK;    /**< @brief Interrupt mask register.        */
  volatile uint32_t GRXSTSR;    /**< @brief Receive status debug read
                                            register.                       */
  volatile uint32_t GRXSTSP;    /**< @brief Receive status read/pop
                                            register.                       */
  volatile uint32_t GRXFSIZ;    /**< @brief Receive FIFO size register.     */
  volatile uint32_t DIEPTXF0;   /**< @brief Endpoint 0 transmit FIFO size
                                            register.                       */
  volatile uint32_t HNPTXSTS;   /**< @brief Non-periodic transmit FIFO/queue
                                            status register.                */
  volatile uint32_t resvd30;
  volatile uint32_t resvd34;
  volatile uint32_t GCCFG;      /**< @brief General core configuration.     */
  volatile uint32_t CID;        /**< @brief Core ID register.               */
  volatile uint32_t resvd58[48];
  volatile uint32_t HPTXFSIZ;   /**< @brief Host periodic transmit FIFO size
                                            register.                       */
  volatile uint32_t DIEPTXF[15];/**< @brief Ddevice IN endpoint transmit FIFO
                                            size registers.                 */
  volatile uint32_t resvd140[176];
  volatile uint32_t HCFG;       /**< @brief Host configuration register.    */
  volatile uint32_t HFIR;       /**< @brief Host frame interval register.   */
  volatile uint32_t HFNUM;      /**< @brief Host frame number/frame time
                                            Remaining register.             */
  volatile uint32_t resvd40C;
  volatile uint32_t HPTXSTS;    /**< @brief Host periodic transmit FIFO/queue
                                            status register.                */
  volatile uint32_t HAINT;      /**< @brief Host all channels interrupt
                                            register.                       */
  volatile uint32_t HAINTMSK;   /**< @brief Host all channels interrupt mask
                                            register.                       */
  volatile uint32_t resvd41C[9];
  volatile uint32_t HPRT;       /**< @brief Host port control and status
                                            register.                       */
  volatile uint32_t resvd444[47];
  stm32_otg_host_chn_t hc[16];  /**< @brief Host channels array.            */
  volatile uint32_t resvd700[64];
  volatile uint32_t DCFG;       /**< @brief Device configuration register.  */
  volatile uint32_t DCTL;       /**< @brief Device control register.        */
  volatile uint32_t DSTS;       /**< @brief Device status register.         */
  volatile uint32_t resvd80C;
  volatile uint32_t DIEPMSK;    /**< @brief Device IN endpoint common interrupt
                                            mask register.                  */
  volatile uint32_t DOEPMSK;    /**< @brief Device OUT endpoint common interrupt
                                            mask register.                  */
  volatile uint32_t DAINT;      /**< @brief Device all endpoints interrupt
                                            register.                       */
  volatile uint32_t DAINTMSK;   /**< @brief Device all endpoints interrupt mask
                                            register.                       */
  volatile uint32_t resvd820;
  volatile uint32_t resvd824;
  volatile uint32_t DVBUSDIS;   /**< @brief Device VBUS Discharge time
                                            register.                       */
  volatile uint32_t DVBUSPULSE; /**< @brief Device VBUS Pulsing time
                                            register.                       */
  volatile uint32_t resvd830;
  volatile uint32_t DIEPEMPMSK; /**< @brief Device IN endpoint FIFO empty
                                            interrupt mask register.        */
  volatile uint32_t resvd838;
  volatile uint32_t resvd83C;
  volatile uint32_t resvd840[16];
  volatile uint32_t resvd880[16];
  volatile uint32_t resvd8C0[16];
  stm32_otg_in_ep_t ie[16];     /**< @brief Input endpoints.                */
  stm32_t_out_ep_t oe[16];      /**< @brief Output endpoints.               */
  volatile uint32_t resvdD00[64];
  volatile uint32_t PCGCCTL;    /**< @brief Power and clock gating control
                                            register.                       */
  volatile uint32_t resvdE04[127];
} stm32_otg_t;

/*
 * GOTGCTL register bit definitions.
 */
#define GOTGCTL_BSesVld         (1<<19) /**< B-Session Valid.               */
#define GOTGCTL_ASesVld         (1<<18) /**< A-Session Valid.               */
#define GOTGCTL_Dbnctime        (1<<17) /**< Long/Short Debounce time.      */
#define GOTGCTL_ConIDSts        (1<<16) /**< Connector ID status.           */
#define GOTGCTL_DevHNPEn        (1<<11) /**< Device HNP Enabled.            */
#define GOTGCTL_HstSetHNPEn     (1<<10) /**< Host Set HNP Enable.           */
#define GOTGCTL_HNPReq          (1<<9)  /**< HNP Request.                   */
#define GOTGCTL_HstNegScs       (1<<8)  /**< Host Negotiation Success.      */
#define GOTGCTL_SesReq          (1<<1)  /**< Session Request.               */
#define GOTGCTL_SesReqScs       (1<<0)  /**< Session Request Success.       */

/*
 * GOTGINT register bit definitions.
 */
#define GOTGINT_DbnceDone       (1<<19) /**< Debounce Done.                 */
#define GOTGINT_ADevTOUTChg     (1<<18) /**< A-Device timeout Change.       */
#define GOTGINT_HstNegDet       (1<<17) /**< Host Negotiation Detected.     */
#define GOTGINT_HstNegSucStsChng (1<<9) /**< Host Negotiation Success status
                                             Change.                        */
#define GOTGINT_SesReqSucStsChng (1<<8) /**< Session Request Success status
                                             Change.                        */
#define GOTGINT_SesEndDet       (1<<2)  /**< Session End Detected.          */

/*
 * GAHBCFG register bit definitions.
 */
#define GAHBCFG_PTxFEmpLvl      (1<<8)  /**< periodic TxFIFO Empty Level.   */
#define GAHBCFG_NPTxFEmpLvl     (1<<7)  /**< Non-periodic TxFIFO Empty
                                             Level.                         */
#define GAHBCFG_GlblIntrMsk     (1<<0)  /**< Global interrupt mask.         */

/*
 * GUSBCFG register bit definitions.
 */
#define GUSBCFG_ForceDevMode    (1<<30) /**< Force Device Mode.             */
#define GUSBCFG_ForceHstMode    (1<<29) /**< Force Host Mode.               */
#define GUSBCFG_TxEndDelay      (1<<28) /**< Tx End Delay.                  */
#define GUSBCFG_USBTrdTim_MASK  (15<<10)/**< USB Turnaround time field mask.*/
#define GUSBCFG_USBTrdTim(n)    ((n)<<10)/**< USB Turnaround time field
                                             value.                         */
#define GUSBCFG_HNPCap          (1<<9)  /**< HNP-Capable.                   */
#define GUSBCFG_SRPCap          (1<<8)  /**< SRP-Capable.                   */
#define GUSBCFG_PHYSel          (1<<6)  /**< USB 2.0 High-Speed PHY or USB
                                             1.1 Full-Speed Serial
                                             Transceiver Select.            */
#define GUSBCFG_TOutCal_MASK    (7<<0)  /**< HS/FS timeout Calibration field
                                             mask.                          */
#define GUSBCFG_TOutCal_(n)     ((n)<<0)/**< HS/FS timeout Calibration field
                                             value.                         */

/*
 * GRSTCTL register bit definitions.
 */
#define GRSTCTL_AHBIdle         (1u<<31)/**< AHB Master Idle.               */
#define GRSTCTL_DMAReq          (1<<30) /**< DMA Request Signal.            */
#define GRSTCTL_TxFNum_MASK     (31<<6) /**< TxFIFO number field mask.      */
#define GRSTCTL_TxFNum(n)       ((n)<<6)/**< TxFIFO number field value.     */
#define GRSTCTL_TxFFlsh         (1<<5)  /**< TxFIFO Flush.                  */
#define GRSTCTL_RxFFlsh         (1<<4)  /**< RxFIFO Flush.                  */
#define GRSTCTL_INTknQFlsh      (1<<3)  /**< IN Token Sequence Learning
                                             queue Flush.                   */
#define GRSTCTL_FrmCntrRst      (1<<2)  /**< Host frame Counter Reset.      */
#define GRSTCTL_HSftRst         (1<<1)  /**< HClk Soft Reset.               */
#define GRSTCTL_CSftRst         (1<<0)  /**< Core Soft Reset.               */

/*
 * GINTSTS register bit definitions.
 */
#define GINTSTS_WkUpInt         (1u<<31)/**< Resume/Remote Wakeup Detected
                                             interrupt.                     */
#define GINTSTS_SessReqInt      (1<<30) /**< Session Request/New Session
                                             Detected interrupt.            */
#define GINTSTS_DisconnInt      (1<<29) /**< Disconnect Detected interrupt. */
#define GINTSTS_ConIDStsChng    (1<<28) /**< Connector ID status Change.    */
#define GINTSTS_LPM_Int         (1<<27) /**< LPM Transaction Received
                                             interrupt.                     */
#define GINTSTS_PTxFEmp         (1<<26) /**< periodic TxFIFO Empty.         */
#define GINTSTS_HChInt          (1<<25) /**< Host channels interrupt.       */
#define GINTSTS_PrtInt          (1<<24) /**< Host port interrupt.           */
#define GINTSTS_ResetDet        (1<<23) /**< Reset Detected interrupt.      */
#define GINTSTS_FetSusp         (1<<22) /**< Data Fetch Suspended.          */
#define GINTSTS_incomplP        (1<<21) /**< Incomplete periodic transfer.  */
#define GINTSTS_incompISOOUT    (1<<21) /**< Incomplete Isochronous OUT
                                             transfer.                      */
#define GINTSTS_incompISOIN     (1<<20) /**< Incomplete Isochronous IN
                                             transfer.                      */
#define GINTSTS_OEPInt          (1<<19) /**< OUT endpoints interrupt.       */
#define GINTSTS_IEPInt          (1<<18) /**< IN endpoints interrupt.        */
#define GINTSTS_EPMis           (1<<17) /**< endpoint Mismatch interrupt.   */
#define GINTSTS_EOPF            (1<<15) /**< End of periodic frame
                                             interrupt.                     */
#define GINTSTS_ISOOutDrop      (1<<14) /**< Isochronous OUT Packet Dropped
                                             interrupt.                     */
#define GINTSTS_EnumDone        (1<<13) /**< Enumeration Done.              */
#define GINTSTS_USBRst          (1<<12) /**< USB Reset.                     */
#define GINTSTS_USBSusp         (1<<11) /**< USB Suspend.                   */
#define GINTSTS_ErlySusp        (1<<10) /**< Early Suspend.                 */
#define GINTSTS_GOUTNakEff      (1<<7)  /**< Global OUT NAK Effective.      */
#define GINTSTS_GINNakEff       (1<<6)  /**< Global IN Non-periodic NAK
                                             Effective.                     */
#define GINTSTS_NPTxFEmp        (1<<5)  /**< Non-periodic TxFIFO Empty.     */
#define GINTSTS_RxFLvl          (1<<4)  /**< RxFIFO Non-Empty.              */
#define GINTSTS_Sof             (1<<3)  /**< Start of (micro)frame.         */
#define GINTSTS_OTGInt          (1<<2)  /**< OTG interrupt.                 */
#define GINTSTS_ModeMis         (1<<1)  /**< Mode Mismatch interrupt.       */
#define GINTSTS_CurMod          (1<<0)  /**< Current Mode of Operation.     */

/*
 * GINTMSK register bit definitions.
 */
#define GINTMSK_WkUpIntMsk      (1u<<31)/**< Resume/Remote Wakeup Detected
                                             interrupt mask.                */
#define GINTMSK_SessReqIntMsk   (1<<30) /**< Session Request/New Session
                                             Detected interrupt mask.       */
#define GINTMSK_DisconnIntMsk   (1<<29) /**< Disconnect Detected interrupt
                                             mask.                          */
#define GINTMSK_ConIDStsChngMsk (1<<28) /**< Connector ID status Change
                                             mask.                          */
#define GINTMSK_LPM_IntMsk      (1<<27) /**< LPM Transaction Received
                                             interrupt mask.                */
#define GINTMSK_PTxFEmpMsk      (1<<26) /**< periodic TxFIFO Empty mask.    */
#define GINTMSK_HChIntMsk       (1<<25) /**< Host channels interrupt mask.  */
#define GINTMSK_PrtIntMsk       (1<<24) /**< Host port interrupt mask.      */
#define GINTMSK_ResetDetMsk     (1<<23) /**< Reset Detected interrupt mask. */
#define GINTMSK_FetSuspMsk      (1<<22) /**< Data Fetch Suspended mask.     */
#define GINTMSK_incomplPMsk     (1<<21) /**< Incomplete periodic transfer
                                             mask.                          */
#define GINTMSK_incompISOOUTMsk (1<<21) /**< Incomplete Isochronous OUT
                                             transfer mask.                 */
#define GINTMSK_incompISOINMsk  (1<<20) /**< Incomplete Isochronous IN
                                             transfer mask.                 */
#define GINTMSK_OEPIntMsk       (1<<19) /**< OUT endpoints interrupt mask.  */
#define GINTMSK_IEPIntMsk       (1<<18) /**< IN endpoints interrupt mask.   */
#define GINTMSK_EPMisMsk        (1<<17) /**< endpoint Mismatch interrupt
                                             mask.                          */
#define GINTMSK_EOPFMsk         (1<<15) /**< End of periodic frame interrupt
                                             mask.                          */
#define GINTMSK_ISOOutDropMsk   (1<<14) /**< Isochronous OUT Packet Dropped
                                             interrupt mask.                */
#define GINTMSK_EnumDoneMsk     (1<<13) /**< Enumeration Done mask.         */
#define GINTMSK_USBRstMsk       (1<<12) /**< USB Reset mask.                */
#define GINTMSK_USBSuspMsk      (1<<11) /**< USB Suspend mask.              */
#define GINTMSK_ErlySuspMsk     (1<<10) /**< Early Suspend mask.            */
#define GINTMSK_GOUTNakEffMsk   (1<<7)  /**< Global OUT NAK Effective mask. */
#define GINTMSK_GINNakEffMsk    (1<<6)  /**< Global Non-periodic IN NAK
                                             Effective mask.                */
#define GINTMSK_NPTxFEmpMsk     (1<<5)  /**< Non-periodic TxFIFO Empty mask.*/
#define GINTMSK_RxFLvlMsk       (1<<4)  /**< Receive FIFO Non-Empty mask.   */
#define GINTMSK_SofMsk          (1<<3)  /**< Start of (micro)frame mask.    */
#define GINTMSK_OTGIntMsk       (1<<2)  /**< OTG interrupt mask.            */
#define GINTMSK_ModeMisMsk      (1<<1)  /**< Mode Mismatch interrupt mask.  */

/*
 * GRXSTSR register bit definitions.
 */
#define GRXSTSR_PktSts_MASK         (15<<17)    /**< Packet status mask.    */
#define GRXSTSR_PktSts(n)           ((n)<<17)   /**< Packet status value.   */
#define GRXSTSR_DPID_MASK           (3<<15)     /**< Data PID mask.         */
#define GRXSTSR_DPID(n)             ((n)<<15)   /**< Data PID value.        */
#define GRXSTSR_BCnt_MASK           (0x7FF<<4)  /**< Byte Count mask.       */
#define GRXSTSR_BCnt_OFF            4           /**< Byte Count offset.     */
#define GRXSTSR_BCnt(n)             ((n)<<5)    /**< Byte Count value.      */
#define GRXSTSR_ChNum_MASK          (15<<0)     /**< channel number mask.   */
#define GRXSTSR_ChNum(n)            ((n)<<0)    /**< channel number value.  */
#define GRXSTSR_EPNum_MASK          (15<<0)     /**< endpoint number mask.  */
#define GRXSTSR_EPNum(n)            ((n)<<0)    /**< endpoint number value. */

/*
 * GRXSTSP register bit definitions.
 */
#define GRXSTSP_PktSts_MASK         (15<<17)    /**< Packet status mask.    */
#define GRXSTSP_PktSts(n)           ((n)<<17)   /**< Packet status value.   */
#define GRXSTSP_OUT_GLOBAL_NAK      GRXSTSP_PktSts(1)
#define GRXSTSP_OUT_DATA            GRXSTSP_PktSts(2)
#define GRXSTSP_OUT_COMP            GRXSTSP_PktSts(3)
#define GRXSTSP_SETUP_COMP          GRXSTSP_PktSts(4)
#define GRXSTSP_SETUP_DATA          GRXSTSP_PktSts(6)
#define GRXSTSP_DPID_MASK           (3<<15)     /**< Data PID mask.         */
#define GRXSTSP_DPID(n)             ((n)<<15)   /**< Data PID value.        */
#define GRXSTSP_BCnt_MASK           (0x7FF<<4)  /**< Byte Count mask.       */
#define GRXSTSP_BCnt_OFF            4           /**< Byte Count offset.     */
#define GRXSTSP_BCnt(n)             ((n)<<5)    /**< Byte Count value.      */
#define GRXSTSP_ChNum_MASK          (15<<0)     /**< channel number mask.   */
#define GRXSTSP_ChNum_OFF           0           /**< channel number mask.   */
#define GRXSTSP_ChNum(n)            ((n)<<0)    /**< channel number value.  */
#define GRXSTSP_EPNum_MASK          (15<<0)     /**< endpoint number mask.  */
#define GRXSTSP_EPNum_OFF           0           /**< endpoint number offset.*/
#define GRXSTSP_EPNum(n)            ((n)<<0)    /**< endpoint number value. */

/*
 * GRXFSIZ register bit definitions.
 */
#define GRXFSIZ_RxFDep_MASK         (0xFFFF<<0) /**< RxFIFO Depth mask.     */
#define GRXFSIZ_RxFDep(n)           ((n)<<0)    /**< RxFIFO Depth value.    */

/*
 * GNPTXFSIZ register bit definitions.
 */
#define GNPTXFSIZ_NPTxFDep_MASK     (0xFFFFu<<16)/**< Non-periodic TxFIFO Depth
                                                     mask.                  */
#define GNPTXFSIZ_NPTxFDep(n)       ((n)<<16)   /**< Non-periodic TxFIFO Depth
                                                     value.                 */
#define GNPTXFSIZ_NPTxFStAddr_MASK  (0xFFFF<<0) /**< Non-periodic transmit RAM
                                                     Start Address mask.    */
#define GNPTXFSIZ_NPTxFStAddr(n)    ((n)<<0)    /**< Non-periodic transmit RAM
                                                     Start Address value.   */

/*
 * GNPTXSTS register bit definitions.
 */
#define GNPTXSTS_NPTxQTop_MASK      (0x7F<<24)  /**< Top of the Non-periodic
                                                     transmit Request queue
                                                     mask.                  */
#define GNPTXSTS_NPTxQTop(n)        ((n)<<24)   /**< Top of the Non-periodic
                                                     transmit Request queue
                                                     value.                 */
#define GNPTXSTS_NPTxQSpcAvail_MASK (0xFF<<16)  /**< Non-periodic transmit
                                                     Request queue Space
                                                     Available mask.        */
#define GNPTXSTS_NPTxQSpcAvail(n)   ((n)<<16)   /**< Non-periodic transmit
                                                     Request queue Space
                                                     Available value.       */
#define GNPTXSTS_NPTxFSpcAvail_MASK (0xFFFF<<0) /**< Non-periodic TxFIFO
                                                     Space Available mask.  */
#define GNPTXSTS_NPTxFSpcAvail(n)   ((n)<<0)    /**< Non-periodic TxFIFO
                                                     Space Available value. */

/*
 * HPTXFSIZ register bit definitions.
 */
#define HPTXFSIZ_PTxFsize_MASK      (0xFFFF<<16)/**< Host periodic TxFIFO
                                                     Depth mask.            */
#define HPTXFSIZ_PTxFsize(n)        ((n)<<16)   /**< Host periodic TxFIFO
                                                     Depth value.           */
#define HPTXFSIZ_PTxFStAddr_MASK    (0xFFFF<<0) /**< Host periodic TxFIFO
                                                     Start Address mask.    */
#define HPTXFSIZ_PTxFStAddr(n)      ((n)<<0)    /**< Host periodic TxFIFO
                                                     Start Address value.   */

/*
 * DPTXFSIZ register bit definitions.
 */
#define DPTXFSIZ_DPTxFsize_MASK     (0xFFFF<<16 /**< Device periodic TxFIFO
                                                     size mask.             */
#define DPTXFSIZ_DPTxFsize(n)       ((n)<<16)   /**< Device periodic TxFIFO
                                                     size value.            */
#define DPTXFSIZ_DPTxFStAddr_MASK   (0xFFFF<<0) /**< Device periodic TxFIFO
                                                     RAM Start Address mask.*/
#define DPTXFSIZ_DPTxFStAddr(n)     ((n)<<0)    /**< Device periodic TxFIFO
                                                     RAM Start Address
                                                     value.                 */

/*
 * HCFG register bit definitions.
 */
#define HCFG_ResValid_MASK          (0xFF<<8)   /**< Resume Validation
                                                     Period mask.           */
#define HCFG_ResValid(n)            ((n)<<8)    /**< Resume Validation
                                                     Period value.          */
#define HCFG_Ena32KHzS              (1<<7)      /**< Enable 32-KHz Suspend
                                                     Mode.                  */
#define HCFG_FSLSSupp               (1<<2)      /**< FS- and LS-Only
                                                     Support.               */
#define HCFG_FSLSPclkSel_MASK       (3<<0)      /**< FS/LS PHY Clock Select
                                                     mask.                  */
#define HCFG_FSLSPclkSel_30_60      (0<<0)      /**< PHY clock is running at
                                                     30/60 MHz.             */
#define HCFG_FSLSPclkSel_48         (1<<0)      /**< PHY clock is running at
                                                     48 MHz.                */

/*
 * HFIR register bit definitions.
 */
#define HFIR_FrInt_MASK             (0xFFFF<<0) /**< frame interval mask.   */
#define HFIR_FrInt(n)               ((n)<<0)    /**< frame interval value.  */

/*
 * HFNUM register bit definitions.
 */
#define HFNUM_FrRem_MASK            (0xFFFF<<16)/**< frame time Remaining
                                                     mask.                  */
#define HFNUM_FrRem(n)              ((n)<<16)   /**< frame time Remaining
                                                     value.                 */
#define HFNUM_FrNum_MASK            (0xFFFF<<0) /**< frame number mask.     */
#define HFNUM_FrNum(n)              ((n)<<0)    /**< frame number value.    */

/*
 * HPTXSTS register bit definitions.
 */
#define HPTXSTS_PTxQTop_MASK        (0xFF<<24)  /**< Top of the periodic
                                                     transmit Request queue
                                                     mask.                  */
#define HPTXSTS_PTxQTop(n)          ((n)<<24)   /**< Top of the periodic
                                                     transmit Request queue
                                                     value.                 */
#define HPTXSTS_PTxQSpcAvail_MASK   (0xFF<<16)  /**< periodic transmit Request
                                                     queue Space Available
                                                     mask.                  */
#define HPTXSTS_PTxQSpcAvail(n)     ((n)<<16)   /**< periodic transmit Request
                                                     queue Space Available
                                                     value.                 */
#define HPTXSTS_PTxFSpcAvail_MASK   (0xFFFF<<0) /**< periodic transmit Data
                                                     FIFO Space Available
                                                     mask.                  */
#define HPTXSTS_PTxFSpcAvail(n)     ((n)<<0)    /**< periodic transmit Data
                                                     FIFO Space Available
                                                     value.                 */

/*
 * HAINT register bit definitions.
 */
#define HAINT_HAINT_MASK            (0xFFFF<<0) /**< channel interrupts
                                                     mask.                  */
#define HAINT_HAINT(n)              ((n)<<0)    /**< channel interrupts
                                                     value.                 */

/*
 * HAINTMSK register bit definitions.
 */
#define HAINTMSK_HAINTMsk_MASK      (0xFFFF<<0) /**< channel interrupt mask
                                                     mask.                  */
#define HAINTMSK_HAINTMsk(n)        ((n)<<0)    /**< channel interrupt mask
                                                     value.                 */

/*
 * HPRT register bit definitions.
 */
#define HPRT_PrtSpd_MASK            (3<<17)     /**< port Speed mask.       */
#define HPRT_PrtSpd_HS              (0<<17)     /**< High Speed value.      */
#define HPRT_PrtSpd_FS              (1<<17)     /**< Full Speed value.      */
#define HPRT_PrtSpd_LS              (2<<17)     /**< Low Speed value.       */
#define HPRT_PrtTstCtl_MASK         (15<<13)    /**< port Test control mask.*/
#define HPRT_PrtTstCtl(n)           ((n)<<13)   /**< port Test control
                                                     value.                 */
#define HPRT_PrtPwr                 (1<<12)     /**< port Power.            */
#define HPRT_PrtLnSts_MASK          (3<<11)     /**< port Line status mask. */
#define HPRT_PrtLnSts_DM            (1<<11)     /**< Logic level of D-.     */
#define HPRT_PrtLnSts_DP            (1<<10)     /**< Logic level of D+.     */
#define HPRT_PrtRst                 (1<<8)      /**< port Reset.            */
#define HPRT_PrtSusp                (1<<7)      /**< port Suspend.          */
#define HPRT_PrtRes                 (1<<6)      /**< port Resume.           */
#define HPRT_PrtOvrCurrChng         (1<<5)      /**< port Overcurrent
                                                     Change.                */
#define HPRT_PrtOvrCurrAct          (1<<4)      /**< port Overcurrent
                                                     Active.                */
#define HPRT_PrtEnChng              (1<<3)      /**< port Enable/Disable
                                                     Change.                */
#define HPRT_PrtEna                 (1<<2)      /**< port Enable.           */
#define HPRT_PrtConnDet             (1<<1)      /**< port Connect Detected. */
#define HPRT_PrtConnSts             (1<<0)      /**< .*/

/*
 * HCCHAR register bit definitions.
 */
#define HCCHAR_ChEna                (1u<<31)    /**< channel Enable.        */
#define HCCHAR_ChDis                (1<<30)     /**< channel Disable.       */
#define HCCHAR_OddFrm               (1<<29)     /**< Odd frame.             */
#define HCCHAR_DevAddr_MASK         (0x7F<<22)  /**< Device Address mask.   */
#define HCCHAR_DevAddr(n)           ((n)<<22)   /**< Device Address value.  */
#define HCCHAR_MC_EC_MASK           (3<<20)     /**< Multi Count (MC) / Error
                                                     Count mask.            */
#define HCCHAR_MC_EC(n)             ((n)<<20)   /**< Multi Count (MC) / Error
                                                     Count value.           */
#define HCCHAR_EPType_MASK          (3<<18)         /**< .*/
#define HCCHAR_EPType(n)            ((n)<<18)   /**< endpoint Type mask.    */
#define HCCHAR_EPType_control       (0<<18)     /**< control endpoint value.*/
#define HCCHAR_EPType_Isochronous   (1<<18)     /**< Isochronous endpoint
                                                     value.                 */
#define HCCHAR_EPType_Bulk          (2<<18)     /**< Bulk endpoint value.   */
#define HCCHAR_EPType_interrupt     (3<<18)     /**< interrupt endpoint
                                                     value.                 */
#define HCCHAR_LSpdDev              (1<<17)     /**< Low-Speed Device.      */
#define HCCHAR_EPDir                (1<<15)     /**< endpoint Direction.    */
#define HCCHAR_EPNum_MASK           (15<<11)    /**< endpoint number mask.  */
#define HCCHAR_EPNum(n)             ((n)<<11)   /**< endpoint number value. */
#define HCCHAR_MPS_MASK             (11<<0)     /**< Maximum Packet size
                                                     mask.                  */
#define HCCHAR_MPS(n)               (11<<0)     /**< Maximum Packet size
                                                     value.                 */

/*
 * HCSPLT register bit definitions.
 */
#define HCSPLT_SpltEna              (1u<<31)    /**< Split Enable.          */
#define HCSPLT_CompSplt             (1<<16)     /**< Do Complete Split.     */
#define HCSPLT_XactPos_MASK         (3<<14)     /**< Transaction Position.  */
#define HCSPLT_XactPos_Mid          (0<<14)     /**< Middle.                */
#define HCSPLT_XactPos_End          (1<<14)     /**< End.                   */
#define HCSPLT_XactPos_Begin        (2<<14)     /**< Begin.                 */
#define HCSPLT_XactPos_all          (3<<14)     /**< all.                   */
#define HCSPLT_HubAddr_MASK         (0x7F<<6)   /**< Hub Address mask.      */
#define HCSPLT_HubAddr(n)           ((n)<<n)    /**< Hub Address value.     */
#define HCSPLT_PrtAddr_MASK         (0x7F<<0)   /**< port Address mask.     */
#define HCSPLT_PrtAddr(n)           ((n)<<0)    /**< port Address value.    */

/*
 * HCINT register bit definitions.
 */
#define HCINT_DataTglErr            (1<<10)     /**< Data Toggle Error.     */
#define HCINT_FrmOvrun              (1<<9)      /**< frame Overrun.         */
#define HCINT_BblErr                (1<<8)      /**< Babble Error.          */
#define HCINT_XactErr               (1<<7)      /**< Transaction Error.     */
#define HCINT_NYET                  (1<<6)      /**< NYET Response Received
                                                     interrupt.             */
#define HCINT_ACK                   (1<<5)      /**< ACK Response
                                                     Received/transmitted
                                                     interrupt.             */
#define HCINT_NAK                   (1<<4)      /**< NAK Response Received
                                                     interrupt.             */
#define HCINT_STALL                 (1<<3)      /**< STALL Response Received
                                                     interrupt.             */
#define HCINT_ChHltd                (1<<1)      /**< channel Halted.        */
#define HCINT_XferCompl             (1<<0)      /**< transfer Completed.    */

/*
 * HCINTMSK register bit definitions.
 */
#define HCINTMSK_DataTglErrMsk      (1<<10)     /**< Data Toggle Error mask.*/
#define HCINTMSK_FrmOvrunMsk        (1<<9)      /**< frame Overrun mask.    */
#define HCINTMSK_BblErrMsk          (1<<8)      /**< Babble Error mask.     */
#define HCINTMSK_XactErrMsk         (1<<7)      /**< Transaction Error mask.*/
#define HCINTMSK_NyetMsk            (1<<6)      /**< NYET Response Received
                                                     interrupt mask.        */
#define HCINTMSK_AckMsk             (1<<5)      /**< ACK Response
                                                     Received/transmitted
                                                     interrupt mask.        */
#define HCINTMSK_NakMsk             (1<<4)      /**< NAK Response Received
                                                     interrupt mask.        */
#define HCINTMSK_StallMsk           (1<<3)      /**< STALL Response Received
                                                     interrupt mask.        */
#define HCINTMSK_ChHltdMsk          (1<<1)      /**< channel Halted mask.   */
#define HCINTMSK_XferComplMsk       (1<<0)      /**< transfer Completed
                                                     mask.*/

/*
 * HCTSIZ register bit definitions.
 */
#define HCTSIZ_DoPng                (1<<31)     /**< Do Ping.               */
#define HCTSIZ_Pid_MASK             (3<<29)     /**< PID mask.              */
#define HCTSIZ_Pid_DATA0            (0<<29)     /**< DATA0.                 */
#define HCTSIZ_Pid_DATA2            (1<<29)     /**< DATA2.                 */
#define HCTSIZ_Pid_DATA1            (2<<29)     /**< DATA1.                 */
#define HCTSIZ_Pid_MDATA            (3<<29)     /**< MDATA.                 */
#define HCTSIZ_Xfersize_MASK        (0x7FFFF<<0)/**< transfer size mask.    */
#define HCTSIZ_Xfersize(n)          ((n)<<0)    /**< transfer size value.   */

/*
 * DCFG register bit definitions.
 */
#define DCFG_ResValid_MASK          (0x3F<<26)  /**< Resume Validation Period
                                                     mask.                  */
#define DCFG_ResValid(n)            ((n)<<26)   /**< Resume Validation Period
                                                     value.                 */
#define DCFG_EPMisCnt_MASK          (0x1F<<18)  /**< IN endpoint Mismatch
                                                     Count mask.            */
#define DCFG_EPMisCnt(n)            ((n)<<18)   /**< IN endpoint Mismatch
                                                     Count value.           */
#define DCFG_PerFrInt_MASK          (3<<11)     /**< periodic frame interval
                                                     mask.                  */
#define DCFG_PerFrInt(n)            ((n)<<11)   /**< periodic frame interval
                                                     value.                 */
#define DCFG_DevAddr_MASK           (0x7F<<4)   /**< Device Address mask.   */
#define DCFG_DevAddr(n)             ((n)<<4)    /**< Device Address value.  */
#define DCFG_Ena32KHzS              (1<<3)      /**< Enable 32-KHz Suspend
                                                     Mode.                  */
#define DCFG_NZStsOUTHShk           (1<<2)      /**< Non-Zero-Length status
                                                     OUT Handshake.         */
#define DCFG_DevSpd_MASK            (3<<0)      /**< Device Speed mask.     */
#define DCFG_DevSpd_HS20            (0<<0)      /**< High speed (USB 2.0 PHY
                                                     clock is 30 MHz or 60
                                                     MHz).                  */
#define DCFG_DevSpd_FS20            (1<<0)      /**< Full speed (USB 2.0 PHY
                                                     clock is 30 MHz or 60
                                                     MHz).                  */
#define DCFG_DevSpd_Reserved        (2<<0)      /**< Reserved.              */
#define DCFG_DevSpd_FS11            (3<<0)      /**< Full speed (USB 1.1
                                                     transceiver clock is 48
                                                     MHz).                  */

/*
 * DCTL register bit definitions.
 */
#define DCTL_NakOnBble              (1<<16)     /**< Set NAK automatically on
                                                     babble.                */
#define DCTL_PWROnPrgDone           (1<<11)     /**< Power-On Programming
                                                     Done.                  */
#define DCTL_CGOUTNak               (1<<10)     /**< Clear Global OUT NAK.  */
#define DCTL_SGOUTNak               (1<<9)      /**< Set Global OUT NAK.    */
#define DCTL_CGNPInNak              (1<<8)      /**< Clear Global Non-periodic
                                                     IN NAK.                */
#define DCTL_SGNPInNak              (1<<7)      /**< Set Global Non-periodic
                                                     IN NAK.                */
#define DCTL_TstCtl_MASK            (7<<4)      /**< Test control mask.     */
#define DCTL_TstCtl(n)              ((n)<<4)    /**< Test control value.    */
#define DCTL_GOUTNakSts             (1<<3)      /**< Global OUT NAK status. */
#define DCTL_GNPINNakSts            (1<<2)      /**< Global Non-periodic IN
                                                     NAK status.            */
#define DCTL_SftDiscon              (1<<1)      /**< Soft Disconnect.       */
#define DCTL_RmtWkUpSig             (1<<0)      /**< Remote Wakeup
                                                     Signaling.             */

/*
 * DSTS register bit definitions.
 */
#define DSTS_SOFFN_MASK             (0x3FFF<<8) /**< frame or Microframe
                                                     number of the Received
                                                     SOF mask.              */
#define DSTS_SOFFN(n)               ((n)<<8)    /**< frame or Microframe
                                                     number of the Received
                                                     SOF value.             */
#define DSTS_ErrticErr              (1<<3)      /**< Erratic Error.         */
#define DSTS_EnumSpd_MASK           (3<<1)      /**< Enumerated Speed mask. */
#define DSTS_EnumSpd_HS_30_60       (0<<1)      /**< High speed (PHY clock is
                                                     running at 30 or 60
                                                     MHz).                  */
#define DSTS_EnumSpd_FS_30_60       (1<<1)      /**< Full speed (PHY clock is
                                                     running at 30 or 60
                                                     MHz).                  */
#define DSTS_EnumSpd_LS_48_6        (2<<1)      /**< Low speed (PHY clock is
                                                     running at 48 MHz,
                                                     internal phy_clk at 6
                                                     MHz).                  */
#define DSTS_EnumSpd_FS_48          (3<<1)      /**< Full speed (PHY clock is
                                                     running at 48 MHz).    */
#define DSTS_SuspSts                (1<<0)      /**< Suspend status.        */

/*
 * DIEPMSK register bit definitions.
 */
#define DIEPMSK_NAKMsk              (1<<13)     /**< NAK interrupt mask.    */
#define DIEPMSK_BNAInIntrMsk        (1<<9)      /**< BNA interrupt mask.    */
#define DIEPMSK_TxfifoUndrnMsk      (1<<8)      /**< Fifo Underrun mask.    */
#define DIEPMSK_INEPNakEffMsk       (1<<6)      /**< IN endpoint NAK Effective
                                                     mask.                  */
#define DIEPMSK_INTknEPMisMsk       (1<<5)      /**< IN Token received with
                                                     EP Mismatch mask.      */
#define DIEPMSK_INTknTXFEmpMsk      (1<<4)      /**< IN Token Received When
                                                     TxFIFO Empty mask.     */
#define DIEPMSK_timeOUTMsk          (1<<3)      /**< timeout Condition mask.*/
#define DIEPMSK_AHBErrMsk           (1<<2)      /**< AHB Error mask.        */
#define DIEPMSK_EPDisbldMsk         (1<<1)      /**< endpoint Disabled
                                                     interrupt mask.        */
#define DIEPMSK_XferComplMsk        (1<<0)      /**< transfer Completed
                                                     interrupt mask.        */

/*
 * DOEPMSK register bit definitions.
 */
#define DOEPMSK_NYETMsk             (1<<14)     /**< NYET interrupt mask.   */
#define DOEPMSK_NAKMsk              (1<<13)     /**< NAK interrupt mask.    */
#define DOEPMSK_BbleErrMsk          (1<<12)     /**< Babble interrupt mask. */
#define DOEPMSK_BnaOutIntrMsk       (1<<9)      /**< BNA interrupt mask.    */
#define DOEPMSK_OutPktErrMsk        (1<<8)      /**< OUT Packet Error mask. */
#define DOEPMSK_Back2BackSETup      (1<<6)      /**< Back-to-Back SETUP
                                                     Packets Received mask. */
#define DOEPMSK_OUTTknEPdisMsk      (1<<4)      /**< OUT Token Received when
                                                     endpoint Disabled mask.*/
#define DOEPMSK_SetUPMsk            (1<<3)      /**< SETUP Phase Done mask. */
#define DOEPMSK_AHBErrMsk           (1<<2)      /**< AHB Error.             */
#define DOEPMSK_EPDisbldMsk         (1<<1)      /**< endpoint Disabled
                                                     interrupt mask.        */
#define DOEPMSK_XferComplMsk        (1<<0)      /**< transfer Completed
                                                     interrupt mask.        */

/*
 * DAINT register bit definitions.
 */
#define DAINT_OutEPInt_MASK         (0xFFFF<<16)/**< OUT endpoint interrupt
                                                     Bits mask.             */
#define DAINT_OutEPInt(n)           ((n)<<16)   /**< OUT endpoint interrupt
                                                     Bits value.            */
#define DAINT_InEpInt_MASK          (0xFFFF<<0) /**< IN endpoint interrupt
                                                     Bits mask.             */
#define DAINT_InEpInt(n)            ((n)<<0)    /**< IN endpoint interrupt
                                                     Bits value.            */

/*
 * DAINTMSK register bit definitions.
 */
#define DAINTMSK_OutEpMsk_MASK      (0xFFFF<<16)/**< OUT EP interrupt mask
                                                     Bits mask.             */
#define DAINTMSK_OutEpMsk(n)        (1<<(16+(n)))/**< OUT EP interrupt mask
                                                     Bits value.            */
#define DAINTMSK_InEpMsk_MASK       (0xFFFF<<0) /**< IN EP interrupt mask
                                                     Bits mask.             */
#define DAINTMSK_InEpMsk(n)         (1<<(n))    /**< IN EP interrupt mask
                                                     Bits value.            */

/*
 * DVBUSDIS register bit definitions.
 */
#define DVBUSDIS_DVBUSDis_MASK      (0xFFFF<<0) /**< Device VBUS Discharge
                                                     time mask.             */
#define DVBUSDIS_DVBUSDis(n)        ((n)<<0)    /**< Device VBUS Discharge
                                                     time value.            */

/*
 * DVBUSPULSE register bit definitions.
 */
#define DVBUSPULSE_DVBUSPulse_MASK  (0xFFF<<0)  /**< Device VBUS Pulsing time
                                                     mask.                  */
#define DVBUSPULSE_DVBUSPulse(n)    ((n)<<0)    /**< Device VBUS Pulsing time
                                                     value.                 */

/*
 * DIEPCTL register bit definitions.
 */
#define DIEPCTL_EPEna               (1u<<31)    /**< endpoint Enable.       */
#define DIEPCTL_EPDis               (1<<30)     /**< endpoint Disable.      */
#define DIEPCTL_SetD1PID            (1<<29)     /**< Set DATA1 PID.         */
#define DIEPCTL_SetD0PID            (1<<28)     /**< Set DATA0 PID.         */
#define DIEPCTL_SNAK                (1<<27)     /**< Set NAK.               */
#define DIEPCTL_CNAK                (1<<26)     /**< Clear NAK.             */
#define DIEPCTL_TxFNum_MASK         (15<<22)    /**< TxFIFO number mask.    */
#define DIEPCTL_TxFNum(n)           ((n)<<22)   /**< TxFIFO number value.   */
#define DIEPCTL_Stall               (1<<21)     /**< STALL Handshake.       */
#define DIEPCTL_Snp                 (1<<20)     /**< Snoop Mode.            */
#define DIEPCTL_EPType_MASK         (3<<18)     /**< endpoint Type mask.    */
#define DIEPCTL_EPType_control      (0<<18)     /**< control.               */
#define DIEPCTL_EPType_Isochronous  (1<<18)     /**< Isochronous.           */
#define DIEPCTL_EPType_Bulk         (2<<18)     /**< Bulk.                  */
#define DIEPCTL_EPType_interrupt    (3<<18)     /**< interrupt.             */
#define DIEPCTL_NAKSts              (1<<17)     /**< NAK status.            */
#define DIEPCTL_DPID                (1<<16)     /**< endpoint Data PID.     */
#define DIEPCTL_USBActEP            (1<<15)     /**< USB Active endpoint.   */
#define DIEPCTL_NextEp_MASK         (15<<11)    /**< Next endpoint mask.    */
#define DIEPCTL_NextEp(n)           ((n)<<11)   /**< Next endpoint value.   */
#define DIEPCTL_MPS_MASK            (0x3FF<<0)  /**< Maximum Packet size
                                                     mask.                  */
#define DIEPCTL_MPS(n)              ((n)<<0)    /**< Maximum Packet size
                                                     value.                 */

/*
 * DIEPINT register bit definitions.
 */
#define DIEPINT_NYETIntrpt          (1<<14)     /**< NYET interrupt.        */
#define DIEPINT_NAKIntrpt           (1<<13)     /**< NAK interrupt.         */
#define DIEPINT_BbleErrIntrpt       (1<<12)     /**< BbleErr (Babble Error)
                                                     interrupt.             */
#define DIEPINT_TxFEmp              (1<<7)      /**< transmit FIFO Empty.   */
#define DIEPINT_INEPNakEff          (1<<6)      /**< IN endpoint NAK
                                                     Effective.             */
#define DIEPINT_INTknEPMis          (1<<5)      /**< IN Token Received with
                                                     EP Mismatch.           */
#define DIEPINT_INTknTXFEmp         (1<<4)      /**< IN Token Received When
                                                     TxFIFO is Empty.       */
#define DIEPINT_timeOUT             (1<<3)      /**< timeout Condition.     */
#define DIEPINT_AHBErr              (1<<2)      /**< AHB Error.             */
#define DIEPINT_EPDisbld            (1<<1)      /**< endpoint Disabled
                                                     interrupt.             */
#define DIEPINT_XferCompl           (1<<0)      /**< transfer Completed     */

/*
 * DIEPTSIZ register bit definitions.
 */
#define DIEPTSIZ_MC_MASK            (3<<29)     /**< Multi Count mask.      */
#define DIEPTSIZ_MC(n)              ((n)<<29)   /**< Multi Count value.     */
#define DIEPTSIZ_PktCnt_MASK        (0x3FF<<19) /**< Packet Count mask.     */
#define DIEPTSIZ_PktCnt(n)          ((n)<<19)   /**< Packet Count value.    */
#define DIEPTSIZ_Xfersize_MASK      (0x7FFFF<<0)/**< transfer size mask.    */
#define DIEPTSIZ_Xfersize(n)        ((n)<<0)    /**< transfer size value.   */

/*
 * DOEPCTL register bit definitions.
 */
#define DOEPCTL_EPEna               (1u<<31)    /**< endpoint Enable.       */
#define DOEPCTL_EPDis               (1<<30)     /**< endpoint Disable.      */
#define DOEPCTL_SetOddFr            (1<<29)     /**< Set Odd (micro)frame.  */
#define DOEPCTL_SetEvenFr           (1<<28)     /**< Set Even (micro)frame. */
#define DOEPCTL_SNAK                (1<<27)     /**< Set NAK.               */
#define DOEPCTL_CNAK                (1<<26)     /**< Clear NAK.             */
#define DOEPCTL_Stall               (1<<21)     /**< STALL Handshake.       */
#define DOEPCTL_Snp                 (1<<20)     /**< Snoop Mode.            */
#define DOEPCTL_EPType_MASK         (3<<18)     /**< endpoint Type mask.    */
#define DOEPCTL_EPType_control      (0<<18)     /**< control.               */
#define DOEPCTL_EPType_Isochronous  (1<<18)     /**< Isochronous.           */
#define DOEPCTL_EPType_Bulk         (2<<18)     /**< Bulk.                  */
#define DOEPCTL_EPType_interrupt    (3<<18)     /**< interrupt.             */
#define DOEPCTL_NAKSts              (1<<17)     /**< NAK status.            */
#define DOEPCTL_EO_FrNum            (1<<16)     /**< Even/Odd (Micro)frame. */
#define DOEPCTL_USBActEP            (1<<15)     /**< USB Active endpoint.   */
#define DOEPCTL_NextEp_MASK         (15<<11)    /**< Next endpoint mask.    */
#define DOEPCTL_NextEp(n)           ((n)<<11)   /**< Next endpoint value.   */
#define DOEPCTL_MPS_MASK            (0x3FF<<0)  /**< Maximum Packet size
                                                     mask.                  */
#define DOEPCTL_MPS(n)              ((n)<<0)    /**< Maximum Packet size
                                                     value.                 */

/*
 * DOEPINT register bit definitions.
 */
#define DOEPINT_NYETIntrpt          (1<<14)     /**< NYET interrupt.        */
#define DOEPINT_NAKIntrpt           (1<<13)     /**< NAK interrupt.         */
#define DOEPINT_BbleErrIntrpt       (1<<12)     /**< BbleErr (Babble Error)
                                                     interrupt.             */
#define DOEPINT_Back2BackSETup      (1<<6)      /**< Back-to-Back SETUP
                                                     Packets Received.      */
#define DOEPINT_OUTTknEPdis         (1<<4)      /**< OUT Token Received When
                                                     endpoint Disabled.     */
#define DOEPINT_SetUp               (1<<3)      /**< SETUP Phase Done.      */
#define DOEPINT_AHBErr              (1<<2)      /**< AHB Error.             */
#define DOEPINT_EPDisbld            (1<<1)      /**< endpoint Disabled
                                                     interrupt.             */
#define DOEPINT_XferCompl           (1<<0)      /**< transfer Completed
                                                     interrupt.             */

/*
 * DOEPTSIZ register bit definitions.
 */
#define DOEPTSIZ_SUPCnt_MASK        (3<<29)     /**< SETUP Packet Cnt mask. */
#define DOEPTSIZ_SUPCnt(n)          ((n)<<29)   /**< SETUP Packet Cnt value.*/
#define DOEPTSIZ_PktCnt_MASK        (0x3FF<<19) /**< Packet Count mask.     */
#define DOEPTSIZ_PktCnt(n)          ((n)<<19)   /**< Packet Count value.    */
#define DOEPTSIZ_Xfersize_MASK      (0x7FFFF<<0)/**< transfer size mask.    */
#define DOEPTSIZ_Xfersize(n)        ((n)<<0)    /**< transfer size value.   */

/*
 * PCGCCTL register bit definitions.
 */
#define PCGCCTL_ResetAfterSusp      (1<<8)      /**< Reset After Suspend.   */
#define PCGCCTL_L1Suspended         (1<<7)      /**< Deep Sleep.            */
#define PCGCCTL_PhySleep            (1<<6)      /**< PHY in Sleep.          */
#define PCGCCTL_Enbl_L1Gating       (1<<5)      /**< Enable Sleep Clock
                                                     Gating.                */
#define PCGCCTL_RstPdwnModule       (1<<3)      /**< Reset Power-Down
                                                     Modules.               */
#define PCGCCTL_PwrClmp             (1<<2)      /**< Power Clamp.           */
#define PCGCCTL_GateHclk            (1<<1)      /**< Gate Hclk.             */
#define PCGCCTL_StopPclk            (1<<0)      /**< Stop Pclk.             */

/**
 * @brief   OTG registers block memory address.
 */
#define OTG_ADDR                    0x50000000

/**
 * @brief   Accesses to the OTG registers block.
 */
#define OTG                         ((stm32_otg_t *)OTG_ADDR)

/**
 * @brief   Returns a FIFO address.
 */
#define OTG_FIFO(n)                 ((volatile uint32_t *)(OTG_ADDR +       \
                                                           0x1000 +
                                                           (0x1000 * (n))))

#endif /* _STM32_OTG_H_ */

/** @} */
