/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    seloops.c
 * @brief   Safety Extensions Loops module code.
 *
 * @addtogroup SELOOPS
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
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
bool seRegWaitAllSet8X(volatile uint8_t *p, uint8_t mask,
                       rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAllSet16X(volatile uint16_t *p, uint16_t mask,
                        rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAllSet32X(volatile uint32_t *p, uint32_t mask,
                        rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAnySet8X(volatile uint8_t *p, uint8_t mask,
                       rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAnySet16X(volatile uint16_t *p, uint16_t mask,
                        rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAnySet32X(volatile uint32_t *p, uint32_t mask,
                        rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAllClear8X(volatile uint8_t *p, uint8_t mask,
                         rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAllClear16X(volatile uint16_t *p, uint32_t mask,
                          rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAllClear32X(volatile uint32_t *p, uint32_t mask,
                          rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAnyClear8X(volatile uint8_t *p, uint8_t mask,
                         rtcnt_t tmo, uint8_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAnyClear16X(volatile uint16_t *p, uint16_t mask,
                          rtcnt_t tmo, uint16_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

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
bool seRegWaitAnyClear32X(volatile uint32_t *p, uint32_t mask,
                          rtcnt_t tmo, uint32_t *valp) {
  rtcnt_t start, end;

  /* Time window for the operation to complete.*/
  start = chSysGetRealtimeCounterX();
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
  } while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end));

  return true;
}

/** @} */
