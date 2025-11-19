/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    hal_safety.c
 * @brief   Safety manager code.
 *
 * @addtogroup HAL_SAFETY
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline uint32_t get_frequency(void) {

#if defined(HAL_LLD_GET_FREQUENCY)
  return HAL_LLD_GET_FREQUENCY();
#else
  return 1000000L;
#endif
}

static inline rtcnt_t get_counter(void) {

#if defined(HAL_LLD_GET_COUNTER)
  return HAL_LLD_GET_COUNTER();
#else
  static rtcnt_t counter = (rtcnt_t)0;
  rtcnt_t current;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  current = counter++;
  osalSysRestoreStatusX(sts);

  return current;
#endif
}

static inline bool is_counter_within(rtcnt_t start, rtcnt_t end) {
  rtcnt_t cnt = get_counter();

  return (bool)(((rtcnt_t)cnt - (rtcnt_t)start) <
                ((rtcnt_t)end - (rtcnt_t)start));
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Wait for all specified bits to be set or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAllSet8X(volatile uint8_t *p, uint8_t mask,
                        rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint8_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) == mask) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for all specified bits to be set or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAllSet16X(volatile uint16_t *p, uint16_t mask,
                         rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint16_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) == mask) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for all specified bits to be set or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAllSet32X(volatile uint32_t *p, uint32_t mask,
                         rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint32_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) == mask) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for any of specified bits to be set or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAnySet8X(volatile uint8_t *p, uint8_t mask,
                        rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint8_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) != (uint8_t)0) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for any of specified bits to be set or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAnySet16X(volatile uint16_t *p, uint16_t mask,
                         rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint16_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) != (uint16_t)0) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for any of specified bits to be set or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAnySet32X(volatile uint32_t *p, uint32_t mask,
                         rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint32_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) != (uint32_t)0) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for all specified bits to be cleared or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAllClear8X(volatile uint8_t *p, uint8_t mask,
                          rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint8_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) == (uint8_t)0) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for all specified bits to be cleared or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAllClear16X(volatile uint16_t *p, uint16_t mask,
                           rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint16_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) == (uint16_t)0) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for all specified bits to be cleared or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAllClear32X(volatile uint32_t *p, uint32_t mask,
                           rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint32_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) == (uint32_t)0) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for any of specified bits to be cleared or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAnyClear8X(volatile uint8_t *p, uint8_t mask,
                          rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint8_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) != mask) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for any of specified bits to be cleared or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAnyClear16X(volatile uint16_t *p, uint16_t mask,
                           rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint16_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) != mask) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/**
 * @brief   Wait for any of specified bits to be cleared or a timeout.
 *
 * @param[in] p         address of the register
 * @param[in] mask      mask of bits to be checked
 * @param[in] tmo       timeout in counter cycles
 * @param[in] valp      pointer to where to store the last read value
 *                      or @p NULL
 * @return              The final result.
 * @retval false        if the condition has been verified.
 * @retval true         if a timeout occurred.
 *
 * @xclass
 */
bool halRegWaitAnyClear32X(volatile uint32_t *p, uint32_t mask,
                           rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = get_counter();
  end = start + tmo;

  /* Testing the condition continuously until it becomes true or the
     timeout expires, it is done at least once.*/
  do {
    /* Getting register value and storing it outside if required.*/
    uint32_t v = *p;
    if (valp != NULL) {
      *valp = v;
    }

    /* Condition check and end of the loop if met.*/
    if ((v & mask) != mask) {
      return false;
    }
  } while (is_counter_within(start, end));

  return true;
}

/** @} */
