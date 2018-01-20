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
#ifndef CRYPTOLIB_LLD_SAMA_SHA_H_
#define CRYPTOLIB_LLD_SAMA_SHA_H_



cryerror_t sama_sha_lld_process(CRYDriver *cryp,
										shaparams_t *params,
										const uint8_t *in,
										uint8_t *out,
										size_t indata_len
										);



#endif /* CRYPTOLIB_LLD_SAMA_SHA_H_ */
