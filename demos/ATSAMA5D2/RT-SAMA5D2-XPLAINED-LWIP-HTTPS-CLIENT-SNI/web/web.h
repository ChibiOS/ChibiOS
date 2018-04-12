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

/**
 * @file web.h
 * @brief HTTP client.
 * @{
 */

#ifndef WEB_H
#define WEB_H

//#define CONFIG_WEBSITE     "www.howsmyssl.com"
//#define CONFIG_RESOURCE    "/"

/* "httpbin.org" server use some sort of load-balancing which means the load balancers need to know which server
 * sitting behind them to forward your traffic to.
 * For this to work the load balancers are requiring the Server Name Indication extension be sent.
 * #define HAVE_SNI in user settings if use "httpbin.org" server
 */
#define CONFIG_WEBSITE     "httpbin.org"
#define CONFIG_RESOURCE    "/user-agent"

#define WEB_PORT           "443"

#ifdef __cplusplus
extern "C" {
#endif

int https_client(void);

#ifdef __cplusplus
}
#endif

#endif /* WEB_H */

/** @} */
