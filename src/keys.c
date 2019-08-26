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

#include "keys.h"

#include "os.h"
#include "cx.h"

#include "global.h"

#include "encoding.h"

unsigned short compress_public_key(cx_ecfp_public_key_t *publicKey,
                                   uint8_t *out,
                                   uint32_t outlen) {
    if (outlen < PUBLICKEY_COMPRESSED_LENGTH) {
        THROW(EXCEPTION_OVERFLOW);
    }

    if (publicKey->curve == CX_CURVE_256K1) {
        out[0] = ((publicKey->W[HASH_64_LENGTH] & 1) ? 0x03 : 0x02);
        os_memmove(out + 1, publicKey->W + 1, HASH_32_LENGTH);
    }
    else if (publicKey->curve == CX_CURVE_Ed25519) {
        uint8_t i;
        out[0] = 0xED;

        for (i = 0U; i < HASH_32_LENGTH; i++) {
            out[i + 1] = publicKey->W[HASH_64_LENGTH - i];
        }

        if ((publicKey->W[HASH_32_LENGTH] & 1) != 0) {
            out[HASH_32_LENGTH] |= 0x80;
        }
    }
    else {
        THROW(EXCEPTION);
    }

    return PUBLICKEY_COMPRESSED_LENGTH;
}
