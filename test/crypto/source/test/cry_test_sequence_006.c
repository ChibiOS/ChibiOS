/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

#include "hal.h"
#include "cry_test_root.h"

/**
 * @file    cry_test_sequence_006.c
 * @brief   Test Sequence 006 code.
 *
 * @page cry_test_sequence_006 [6] SHA
 *
 * File: @ref cry_test_sequence_006.c
 *
 * <h2>Description</h2>
 * SHA testing.
 *
 * <h2>Test Cases</h2>
 * - @subpage cry_test_006_001
 * - @subpage cry_test_006_002
 * - @subpage cry_test_006_003
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include <string.h>
#include "ref_sha.h"

#define MAX_DIGEST_SIZE_INBYTE  64
#define MAX_DIGEST_SIZE_INWORD  (MAX_DIGEST_SIZE_INBYTE/4)
static uint32_t digest[MAX_DIGEST_SIZE_INWORD];

static const CRYConfig configSHA_Polling=
{
    TRANSFER_POLLING,
    0,
    0
};



/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page cry_test_006_001 [6.1] SHA1 Polling
 *
 * <h2>Description</h2>
 * testing SHA1 in polled mode.
 *
 * <h2>Test Steps</h2>
 * - [6.1.1] Digest.
 * .
 */

static void cry_test_006_001_setup(void) {
  memset(msg_clear, 0, TEST_MSG_DATA_BYTE_LEN);
  memset(digest, 0, MAX_DIGEST_SIZE_INWORD);
  memcpy((char*) msg_clear, sha_msg0, SHA_LEN_0);
  cryStart(&CRYD1, &configSHA_Polling);


}

static void cry_test_006_001_teardown(void) {
  cryStop(&CRYD1);
}

static void cry_test_006_001_execute(void) {
    cryerror_t ret;
    uint32_t *ref;

  /* [6.1.1] Digest.*/
  test_set_step(1);
  {
    //---- One Block Test
    ret = crySHA1(&CRYD1,SHA_LEN_0,(uint8_t*)msg_clear,(uint8_t*)digest);


    test_assert(ret == CRY_NOERROR, "sha1 failed");


    SHOW_DATA(digest,5);

    ref = (uint32_t*)refSHA_SHA1_3;
    for (int i = 0; i < 5; i++) {
      test_assert(digest[i] == ref[i], "sha1 digest mismatch");
    }

    //---- Multi Block Test
    memset(msg_clear, 0, TEST_MSG_DATA_BYTE_LEN);
    memcpy((char*) msg_clear, sha_msg1, SHA_LEN_1);

    ret = crySHA1(&CRYD1,SHA_LEN_1,(uint8_t*)msg_clear,(uint8_t*)digest);

    test_assert(ret == CRY_NOERROR, "sha1 failed");

     SHOW_DATA(digest,5);


    ref = (uint32_t*)refSHA_SHA1_56;
    for (int i = 0; i < 5; i++) {
      test_assert(digest[i] == ref[i], "sha1 digest mismatch");
    }


  }
}

static const testcase_t cry_test_006_001 = {
  "SHA1 Polling",
  cry_test_006_001_setup,
  cry_test_006_001_teardown,
  cry_test_006_001_execute
};

/**
 * @page cry_test_006_002 [6.2] SHA256 Polling
 *
 * <h2>Description</h2>
 * testing SHA256 in polled mode.
 *
 * <h2>Test Steps</h2>
 * - [6.2.1] Digest.
 * .
 */

static void cry_test_006_002_setup(void) {
  memset(msg_clear, 0, TEST_MSG_DATA_BYTE_LEN);
  memset(digest, 0, MAX_DIGEST_SIZE_INWORD);
  memcpy((char*) msg_clear, sha_msg0, SHA_LEN_0);
  cryStart(&CRYD1, &configSHA_Polling);


}

static void cry_test_006_002_teardown(void) {
  cryStop(&CRYD1);
}

static void cry_test_006_002_execute(void) {
    cryerror_t ret;
    uint32_t *ref;

  /* [6.2.1] Digest.*/
  test_set_step(1);
  {

    //---- One Block Test
    ret = crySHA256(&CRYD1,SHA_LEN_0,(uint8_t*)msg_clear,(uint8_t*)digest);

    test_assert(ret == CRY_NOERROR, "sha256 failed");

    SHOW_DATA(digest,8);

    ref = (uint32_t*)refSHA_SHA256_3;
    for (int i = 0; i < 8; i++) {
      test_assert(digest[i] == ref[i], "sha256 digest mismatch");
    }

    //---- Multi Block Test
    memset(msg_clear, 0, TEST_MSG_DATA_BYTE_LEN);
    memcpy((char*) msg_clear, sha_msg1, SHA_LEN_1);

    ret = crySHA256(&CRYD1,SHA_LEN_1,(uint8_t*)msg_clear,(uint8_t*)digest);

    test_assert(ret == CRY_NOERROR, "sha256 failed");


     SHOW_DATA(digest,8);

    ref = (uint32_t*)refSHA_SHA256_56;
    for (int i = 0; i < 8; i++) {
      test_assert(digest[i] == ref[i], "sha256 digest mismatch");
    }


  }
}

static const testcase_t cry_test_006_002 = {
  "SHA256 Polling",
  cry_test_006_002_setup,
  cry_test_006_002_teardown,
  cry_test_006_002_execute
};

/**
 * @page cry_test_006_003 [6.3] SHA512 Polling
 *
 * <h2>Description</h2>
 * testing SHA512 in polled mode.
 *
 * <h2>Test Steps</h2>
 * - [6.3.1] Digest.
 * .
 */

static void cry_test_006_003_setup(void) {
  memset(msg_clear, 0, TEST_MSG_DATA_BYTE_LEN);
  memset(digest, 0, MAX_DIGEST_SIZE_INWORD);
  memcpy((char*) msg_clear, sha_msg0, SHA_LEN_0);
  cryStart(&CRYD1, &configSHA_Polling);


}

static void cry_test_006_003_teardown(void) {
  cryStop(&CRYD1);
}

static void cry_test_006_003_execute(void) {
    cryerror_t ret;
    uint32_t *ref;

  /* [6.3.1] Digest.*/
  test_set_step(1);
  {
    //---- One Block Test
    ret = crySHA512(&CRYD1,SHA_LEN_0,(uint8_t*)msg_clear,(uint8_t*)digest);

    test_assert(ret == CRY_NOERROR, "sha512 failed");

    SHOW_DATA(digest,16);

    ref = (uint32_t*)refSHA_SHA512_3;
    for (int i = 0; i < 16; i++) {
      test_assert(digest[i] == ref[i], "sha512 digest mismatch");
    }

    //---- Multi Block Test
    memset(msg_clear, 0, TEST_MSG_DATA_BYTE_LEN);
    memcpy((char*) msg_clear, sha_msg1, SHA_LEN_1);

    ret = crySHA512(&CRYD1,SHA_LEN_1,(uint8_t*)msg_clear,(uint8_t*)digest);

    test_assert(ret == CRY_NOERROR, "sha512 failed");


     SHOW_DATA(digest,16);

    ref = (uint32_t*)refSHA_SHA512_56;
    for (int i = 0; i < 16; i++) {
      test_assert(digest[i] == ref[i], "sha512 digest mismatch");
    }


  }
}

static const testcase_t cry_test_006_003 = {
  "SHA512 Polling",
  cry_test_006_003_setup,
  cry_test_006_003_teardown,
  cry_test_006_003_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Array of test cases.
 */
const testcase_t * const cry_test_sequence_006_array[] = {
  &cry_test_006_001,
  &cry_test_006_002,
  &cry_test_006_003,
  NULL
};

/**
 * @brief   SHA.
 */
const testsequence_t cry_test_sequence_006 = {
  "SHA",
  cry_test_sequence_006_array
};
