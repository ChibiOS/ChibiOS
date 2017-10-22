/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    CRYPv1/hal_crypto_lld.c
 * @brief   STM32 cryptographic subsystem low level driver source.
 *
 * @addtogroup CRYPTO
 * @{
 */

#include "hal.h"

#if (HAL_USE_CRY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CRY1 driver identifier.*/
#if STM32_CRY_USE_CRYP1 || defined(__DOXYGEN__)
CRYDriver CRYD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level crypto driver initialization.
 *
 * @notapi
 */
void cry_lld_init(void) {

}

/**
 * @brief   Configures and activates the crypto peripheral.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 *
 * @notapi
 */
void cry_lld_start(CRYDriver *cryp) {

  if (cryp->state == CRY_STOP) {

  }
}

/**
 * @brief   Deactivates the crypto peripheral.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 *
 * @notapi
 */
void cry_lld_stop(CRYDriver *cryp) {

  if (cryp->state == CRY_READY) {

  }
}

/**
 * @brief   Initializes the transient key for a specific algorithm.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] keyp      pointer to the key data
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the specified algorithm is unknown or
 *                              unsupported.
 * @retval CRY_ERR_INV_KEY_SIZE if the specified key size is invalid.
 *
 * @notapi
 */
cryerror_t cry_lld_loadkey(CRYDriver *cryp,
                           cryalgorithm_t algorithm,
                           size_t size,
                           const uint8_t *keyp) {

  (void)cryp;
  (void)algorithm;
  (void)size;
  (void)keyp;

  return CRY_NOERROR;
}

/**
 * @brief   Encryption operation using AES-ECB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @api
 */
cryerror_t cry_lld_encrypt_AES_ECB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-ECB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @api
 */
cryerror_t cry_lld_decrypt_AES_ECB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-CBC.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        input vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @api
 */
cryerror_t cry_lld_encrypt_AES_CBC(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-CBC.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        input vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @api
 */
cryerror_t cry_lld_decrypt_AES_CBC(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-CFB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        input vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @api
 */
cryerror_t cry_lld_encrypt_AES_CFB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-CFB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] iv        input vector
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @api
 */
cryerror_t cry_lld_decrypt_AES_CFB(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *iv) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)iv;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Encryption operation using AES-CTR.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] nonce     the "nonce" constant
 * @param[in,out] cnt   the initial value of the counter, normally zero
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @api
 */
cryerror_t cry_lld_encrypt_AES_CTR(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *nonce,
                                   uint8_t *cnt) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)nonce;
  (void)cnt;

  return CRY_ERR_INV_ALGO;
}

/**
 * @brief   Decryption operation using AES-CTR.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of the selected key size
 * @param[in] in        buffer containing the input plaintext
 * @param[out] out      buffer for the output cyphertext
 * @param[in] nonce     the "nonce" constant
 * @param[in,out] cnt   the initial value of the counter, normally zero
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the operation is unsupported on this
 *                              device instance.
 * @retval CRY_ERR_INV_KEY_TYPE the selected key is invalid for this operation.
 * @retval CRY_ERR_INV_KEY_ID   if the specified key identifier is invalid
 *                              or refers and empty key slot.
 *
 * @notapi
 */
cryerror_t cry_lld_decrypt_AES_CTR(CRYDriver *cryp,
                                   crykey_t key_id,
                                   size_t size,
                                   const uint8_t *in,
                                   uint8_t *out,
                                   const uint8_t *nonce,
                                   uint8_t *cnt) {

  (void)cryp;
  (void)key_id;
  (void)size;
  (void)in;
  (void)out;
  (void)nonce;
  (void)cnt;

  return CRY_ERR_INV_ALGO;
}

#endif /* HAL_USE_CRY == TRUE */

/** @} */
