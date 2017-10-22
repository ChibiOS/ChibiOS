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
 * @file    hal_crypto.c
 * @brief   Cryptographic Driver code.
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

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Crypto Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void cryInit(void) {

  cry_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p CRYDriver structure.
 *
 * @param[out] cryp     pointer to the @p CRYDriver object
 *
 * @init
 */
void cryObjectInit(CRYDriver *cryp) {

  cryp->state    = CRY_STOP;
  cryp->config   = NULL;
#if defined(CRY_DRIVER_EXT_INIT_HOOK)
  CRY_DRIVER_EXT_INIT_HOOK(cryp);
#endif
}

/**
 * @brief   Configures and activates the CRY peripheral.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] config    pointer to the @p CRYConfig object. Depending on
 *                      the implementation the value can be @p NULL.
 *
 * @api
 */
void cryStart(CRYDriver *cryp, const CRYConfig *config) {

  osalDbgCheck(cryp != NULL);

  osalSysLock();
  osalDbgAssert((cryp->state == CRY_STOP) || (cryp->state == CRY_READY),
                "invalid state");
  cryp->config = config;
  cry_lld_start(cryp);
  cryp->state = CRY_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the CRY peripheral.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 *
 * @api
 */
void cryStop(CRYDriver *cryp) {

  osalDbgCheck(cryp != NULL);

  osalSysLock();

  osalDbgAssert((cryp->state == CRY_STOP) || (cryp->state == CRY_READY),
                "invalid state");

  cry_lld_stop(cryp);
  cryp->config = NULL;
  cryp->state  = CRY_STOP;

  osalSysUnlock();
}

/**
 * @brief   Initializes the transient key for a specific algorithm.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] algorithm the algorithm identifier
 * @param[in] size      key size in bytes
 * @param[in] keyp      pointer to the key data
 * @return              The operation status.
 * @retval CRY_NOERROR          if the operation succeeded.
 * @retval CRY_ERR_INV_ALGO     if the specified algorithm is unknown or
 *                              unsupported.
 * @retval CRY_ERR_INV_KEY_SIZE if the specified key size is invalid.
 *
 * @api
 */
cryerror_t cryLoadTransientKey(CRYDriver *cryp,
                               cryalgorithm_t algorithm,
                               size_t size,
                               const uint8_t *keyp) {
  cryerror_t err;

  /* Storing the transient key metadata.*/
  cryp->key0_type = algorithm;
  cryp->key0_size = size;

  /* Key setup in the low level driver.*/
  err = cry_lld_loadkey(cryp, algorithm, size, keyp);
  if (err != CRY_NOERROR) {
    cryp->key0_type = cry_algo_none;
    cryp->key0_size = (size_t)0;
  }

  return err;
}

#if (CRY_LLD_SUPPORTS_AES_ECB == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Encryption operation using AES-ECB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryEncryptAES_ECB(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out) {

  osalDbgCheck((in != NULL) && (out != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_encrypt_AES_ECB(cryp, key_id, size, in, out);
}

/**
 * @brief   Decryption operation using AES-ECB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryDecryptAES_ECB(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out) {

  osalDbgCheck((in != NULL) && (out != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_decrypt_AES_ECB(cryp, key_id, size, in, out);
}
#endif /* CRY_LLD_SUPPORTS_AES_ECB == TRUE */

#if (CRY_LLD_SUPPORTS_AES_CBC == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Encryption operation using AES-CBC.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryEncryptAES_CBC(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out,
                             const uint8_t *iv) {

  osalDbgCheck((in != NULL) && (out != NULL) && (iv != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_encrypt_AES_CBC(cryp, key_id, size, in, out, iv);
}

/**
 * @brief   Decryption operation using AES-CBC.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryDecryptAES_CBC(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out,
                             const uint8_t *iv) {

  osalDbgCheck((in != NULL) && (out != NULL) && (iv != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_decrypt_AES_CBC(cryp, key_id, size, in, out, iv);
}
#endif /* CRY_LLD_SUPPORTS_AES_CBC == TRUE */

#if (CRY_LLD_SUPPORTS_AES_CFB == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Encryption operation using AES-CFB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryEncryptAES_CFB(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out,
                             const uint8_t *iv) {

  osalDbgCheck((in != NULL) && (out != NULL) && (iv != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_encrypt_AES_CBC(cryp, key_id, size, in, out, iv);
}

/**
 * @brief   Decryption operation using AES-CFB.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryDecryptAES_CFB(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out,
                             const uint8_t *iv) {

  osalDbgCheck((in != NULL) && (out != NULL) && (iv != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_decrypt_AES_CBC(cryp, key_id, size, in, out, iv);
}
#endif /* CRY_LLD_SUPPORTS_AES_CFB == TRUE */

#if (CRY_LLD_SUPPORTS_AES_CTR == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Encryption operation using AES-CTR.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryEncryptAES_CTR(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out,
                             const uint8_t *nonce,
                             uint8_t *cnt) {

  osalDbgCheck((in != NULL) && (out != NULL) &&
               (nonce != NULL) && (cnt != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_encrypt_AES_CTR(cryp, key_id, size, in, out, nonce, cnt);
}

/**
 * @brief   Decryption operation using AES-CTR.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] key_id    the key to be used for the operation, zero is the
 *                      transient key, other values are keys stored in an
 *                      unspecified way
 * @param[in] size      size of the plaintext buffer, this number must be a
 *                      multiple of 16
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
cryerror_t cryDecryptAES_CTR(CRYDriver *cryp,
                             crykey_t key_id,
                             size_t size,
                             const uint8_t *in,
                             uint8_t *out,
                             const uint8_t *nonce,
                             uint8_t *cnt) {

  osalDbgCheck((in != NULL) && (out != NULL) &&
               (nonce != NULL) && (cnt != NULL) &&
               ((size & (size_t)15) == (size_t)0));

  osalDbgAssert(cryp->state == CRY_READY, "not ready");

  return cry_lld_decrypt_AES_CTR(cryp, key_id, size, in, out, nonce, cnt);
}
#endif /* CRY_LLD_SUPPORTS_AES_CTR == TRUE */

#endif /* HAL_USE_CRY == TRUE */

/** @} */
