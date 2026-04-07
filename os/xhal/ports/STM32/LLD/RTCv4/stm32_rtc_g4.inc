/**
 * @brief   RTC tamper/time-stamp interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_TAMP_STAMP_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   RTC wakeup interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_WKUP_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   RTC alarm interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_ALARM_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}
