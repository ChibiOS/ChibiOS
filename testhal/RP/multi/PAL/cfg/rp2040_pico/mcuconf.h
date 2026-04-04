#ifndef MCUCONF_H
#define MCUCONF_H

#define RP2040_MCUCONF

/*
 * HAL driver system settings.
 */
#define RP_NO_INIT                          FALSE
#define RP_CORE1_START                      FALSE
#define RP_CORE1_VECTORS_TABLE              _vectors
#define RP_CORE1_ENTRY_POINT                _crt0_c1_entry
#define RP_CORE1_STACK_END                  __c1_main_stack_end__

/*
 * IRQ system settings.
 */
#define RP_IRQ_SYSTICK_PRIORITY             2
#define RP_IRQ_TIMER0_ALARM0_PRIORITY       2
#define RP_IRQ_TIMER0_ALARM1_PRIORITY       2
#define RP_IRQ_TIMER0_ALARM2_PRIORITY       2
#define RP_IRQ_TIMER0_ALARM3_PRIORITY       2
#define RP_IRQ_UART0_PRIORITY               3
#define RP_IRQ_UART1_PRIORITY               3
#define RP_IRQ_SPI0_PRIORITY                2
#define RP_IRQ_SPI1_PRIORITY                2

#endif /* MCUCONF_H */
