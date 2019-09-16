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

#ifndef ARK_CRYPTO_SIGNING_CONTEXT_H
#define ARK_CRYPTO_SIGNING_CONTEXT_H

#include <stdint.h>

#include <os.h>
#include <cx.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct signing_context_t {
    cx_curve_t  curve;
    uint8_t     pathLength;
    uint32_t    bip32Path[ADDRESS_MAX_BIP32_PATH];
    uint8_t     data[MAX_RAW_OPERATION];
    uint32_t    dataLength;
} SigningContext;

////////////////////////////////////////////////////////////////////////////////

uint32_t signEcdsa(const cx_ecfp_private_key_t *privateKey,
                   const uint8_t *hash,
                   uint8_t *signature,
                   unsigned int signatureLength);

////////////////////////////////////////////////////////////////////////////////

#endif
