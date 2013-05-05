/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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
 * @file    DSPI_v1/spc5_dspi.h
 * @brief   SPC5xx DSPI header file.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef _SPC5_DSPI_H_
#define _SPC5_DSPI_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

struct spc5_dspi {
  union {
    vuint32_t R;
    struct {
      vuint32_t MSTR :1;
      vuint32_t CONT_SCKE :1;
      vuint32_t DCONF :2;
      vuint32_t FRZ :1;
      vuint32_t MTFE :1;
      vuint32_t PCSSE :1;
      vuint32_t ROOE :1;
      vuint32_t PCSIS7 :1;
      vuint32_t PCSIS6 :1;
      vuint32_t PCSIS5 :1;
      vuint32_t PCSIS4 :1;
      vuint32_t PCSIS3 :1;
      vuint32_t PCSIS2 :1;
      vuint32_t PCSIS1 :1;
      vuint32_t PCSIS0 :1;
      vuint32_t :1;
      vuint32_t MDIS :1;
      vuint32_t DIS_TXF :1;
      vuint32_t DIS_RXF :1;
      vuint32_t CLR_TXF :1;
      vuint32_t CLR_RXF :1;
      vuint32_t SMPL_PT :2;
      vuint32_t :7;
      vuint32_t HALT :1;
    } B;
  } MCR; /* Module Configuration Register */

  uint32_t dspi_reserved1;

  union {
    vuint32_t R;
    struct {
      vuint32_t TCNT :16;
      vuint32_t :16;
    } B;
  } TCR;

  union {
    vuint32_t R;
    struct {
      vuint32_t DBR :1;
      vuint32_t FMSZ :4;
      vuint32_t CPOL :1;
      vuint32_t CPHA :1;
      vuint32_t LSBFE :1;
      vuint32_t PCSSCK :2;
      vuint32_t PASC :2;
      vuint32_t PDT :2;
      vuint32_t PBR :2;
      vuint32_t CSSCK :4;
      vuint32_t ASC :4;
      vuint32_t DT :4;
      vuint32_t BR :4;
    } B;
  } CTAR[8]; /* Clock and Transfer Attributes Registers */

  union {
    vuint32_t R;
    struct {
      vuint32_t TCF :1;
      vuint32_t TXRXS :1;
      vuint32_t :1;
      vuint32_t EOQF :1;
      vuint32_t TFUF :1;
      vuint32_t :1;
      vuint32_t TFFF :1;
      vuint32_t :5;
      vuint32_t RFOF :1;
      vuint32_t :1;
      vuint32_t RFDF :1;
      vuint32_t :1;
      vuint32_t TXCTR :4;
      vuint32_t TXNXTPTR :4;
      vuint32_t RXCTR :4;
      vuint32_t POPNXTPTR :4;
    } B;
  } SR; /* Status Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t TCFRE :1;
      vuint32_t :2;
      vuint32_t EOQFRE :1;
      vuint32_t TFUFRE :1;
      vuint32_t :1;
      vuint32_t TFFFRE :1;
      vuint32_t TFFFDIRS :1;
      vuint32_t :4;
      vuint32_t RFOFRE :1;
      vuint32_t :1;
      vuint32_t RFDFRE :1;
      vuint32_t RFDFDIRS :1;
      vuint32_t :16;
    } B;
  } RSER; /* DMA/Interrupt Request Select and Enable Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t CONT :1;
      vuint32_t CTAS :3;
      vuint32_t EOQ :1;
      vuint32_t CTCNT :1;
      vuint32_t :2;
      vuint32_t PCS7 :1;
      vuint32_t PCS6 :1;
      vuint32_t PCS5 :1;
      vuint32_t PCS4 :1;
      vuint32_t PCS3 :1;
      vuint32_t PCS2 :1;
      vuint32_t PCS1 :1;
      vuint32_t PCS0 :1;
      vuint32_t TXDATA :16;
    } B;
  } PUSHR; /* PUSH TX FIFO Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t RXDATA :16;
    } B;
  } POPR; /* POP RX FIFO Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t TXCMD :16;
      vuint32_t TXDATA :16;
    } B;
  } TXFR[5]; /* Transmit FIFO Registers */

  vuint32_t DSPI_reserved_txf[11];

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t RXDATA :16;
    } B;
  } RXFR[5]; /* Receive FIFO Registers */

  vuint32_t DSPI_reserved_rxf[12];

  union {
    vuint32_t R;
    struct {
      vuint32_t MTOE :1;
      vuint32_t :1;
      vuint32_t MTOCNT :6;
      vuint32_t :4;
      vuint32_t TXSS :1;
      vuint32_t TPOL :1;
      vuint32_t TRRE :1;
      vuint32_t CID :1;
      vuint32_t DCONT :1;
      vuint32_t DSICTAS :3;
      vuint32_t :6;
      vuint32_t DPCS5 :1;
      vuint32_t DPCS4 :1;
      vuint32_t DPCS3 :1;
      vuint32_t DPCS2 :1;
      vuint32_t DPCS1 :1;
      vuint32_t DPCS0 :1;
    } B;
  } DSICR; /* DSI Configuration Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t SER_DATA :16;
    } B;
  } SDR; /* DSI Serialization Data Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t ASER_DATA :16;
    } B;
  } ASDR; /* DSI Alternate Serialization Data Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t COMP_DATA :16;
    } B;
  } COMPR; /* DSI Transmit Comparison Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t DESER_DATA :16;
    } B;
  } DDR; /* DSI deserialization Data Register */

};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    DSPI units references
 * @{
 */
#if SPC5_HAS_DSPI0 || defined(__DOXYGEN__)
#define SPC5_DSPI0      (*(struct spc5_dspi *)0xFFF90000U)
#endif

#if SPC5_HAS_DSPI1 || defined(__DOXYGEN__)
#define SPC5_DSPI1      (*(struct spc5_dspi *)0xFFF94000U)
#endif

#if SPC5_HAS_DSPI2 || defined(__DOXYGEN__)
#define SPC5_DSPI2      (*(struct spc5_dspi *)0xFFF98000U)
#endif

#if SPC5_HAS_DSPI3 || defined(__DOXYGEN__)
#define SPC5_DSPI3      (*(struct spc5_dspi *)0xFFF9C000U)
#endif
/** @} */

#endif /* _SPC5_DSPI_H_ */

/** @} */
