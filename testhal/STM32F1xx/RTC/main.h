#ifndef MAIN_H_
#define MAIN_H_

/******************************************************************
 * глобальные флаги
 ******************************************************************/
/* надо ли фильтровать данные с датчиков */
#define GET_FILTERED_DATA TRUE

/* включить стрессовое тестирование */
#define ENABLE_IRQ_STORM FALSE

// usefull macros
#define WATCHDOG_INIT {\
    DBGMCU->CR |= DBGMCU_CR_DBG_IWDG_STOP; /* stop watchdog timer in debugging mode */\
    IWDG->KR = 0x5555;/*unlock PR register*/\
    IWDG->PR = 16;/*set 1.6384s timeout*/\
    IWDG->KR = 0xCCCC;/*start watchdog*/}

#define WATCHDOG_RELOAD {IWDG->KR = 0xAAAA;}


#endif /* MAIN_H_ */
