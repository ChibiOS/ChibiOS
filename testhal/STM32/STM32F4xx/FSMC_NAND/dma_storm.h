#ifndef DMA_STORM_H_
#define DMA_STORM_H_

#ifdef __cplusplus
extern "C" {
#endif
  void dma_storm_spi_start(void);
  uint32_t dma_storm_spi_stop(void);
  void dma_storm_adc_start(void);
  uint32_t dma_storm_adc_stop(void);
  void dma_storm_uart_start(void);
  uint32_t dma_storm_uart_stop(void);
#ifdef __cplusplus
}
#endif

#endif /* DMA_STORM_H_ */
