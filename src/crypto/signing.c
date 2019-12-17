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

#include "crypto/signing.h"

#include <stddef.h>

#include <os.h>
#include <cx.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////

uint32_t signEcdsa(const cx_ecfp_private_key_t *privateKey,
                   const uint8_t *hash,
                   uint8_t *signature,
                   size_t signatureSize) {
    #if CX_APILEVEL >= 8U
        return cx_ecdsa_sign(privateKey,
                             CX_RND_RFC6979 | CX_LAST, CX_SHA256,
                             hash, HASH_32_LENGTH,
                             signature, signatureSize,
                             NULL);
    #else
        UNUSED(signatureLength);
        return cx_ecdsa_sign(&privateKey,
                             CX_RND_RFC6979 | CX_LAST, CX_SHA256,
                             hash, HASH_32_LENGTH,
                             signature);
        signature[0] = 0x30;
    #endif
} 

////////////////////////////////////////////////////////////////////////////////
