/*******************************************************************************
*   Ark Wallet
*   (c) 2017 Ledger
*   (c) ARK Ecosystem
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#ifndef ARK_UTILS_BASE58_H
#define ARK_UTILS_BASE58_H

#include <stdint.h>

#include <os.h>

////////////////////////////////////////////////////////////////////////////////

uint8_t encodeBase58(uint8_t WIDE *in,
                     uint8_t length,
                     uint8_t *out,
                     uint8_t maxoutlen);

uint16_t encodeBase58PublicKey(uint8_t WIDE *in,
                               uint16_t inLength,
                               uint8_t *out,
                               uint16_t outLength,
                               uint16_t version,
                               uint8_t alreadyHashed);

////////////////////////////////////////////////////////////////////////////////

#endif
