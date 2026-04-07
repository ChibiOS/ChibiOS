/**
 * @brief   RTC global interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_GLOBAL_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   RTC tamper interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_TAMP_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}
