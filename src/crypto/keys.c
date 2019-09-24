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

#include "crypto/keys.h"

#include <os.h>
#include <cx.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////

void compressPublicKey(const cx_ecfp_public_key_t *publicKey,
                       uint8_t *out,
                       uint8_t outLength) {
    if (outLength != PUBLICKEY_COMPRESSED_LENGTH) {
        THROW(EXCEPTION_OVERFLOW);
    }

    if (publicKey->curve == CX_CURVE_256K1) {
        out[0] = ((publicKey->W[HASH_64_LENGTH] & 1U) ? 0x03 : 0x02);
        os_memmove(out + 1U, publicKey->W + 1U, HASH_32_LENGTH);
    }
    else {
        THROW(EXCEPTION);
    }
}

////////////////////////////////////////////////////////////////////////////////

uint32_t setPublicKeyContext(PublicKeyContext *ctx, uint8_t *apduBuffer) {
    uint32_t tx = 0;
    apduBuffer[tx++] = PUBLICKEY_COMPRESSED_LENGTH;

    compressPublicKey(&ctx->data,
                      &apduBuffer[tx],
                      PUBLICKEY_COMPRESSED_LENGTH);

    tx += PUBLICKEY_COMPRESSED_LENGTH;

    apduBuffer[tx++] = ADDRESS_LENGTH;

    os_memmove(&apduBuffer[tx], ctx->address, ADDRESS_LENGTH);
    tx += ADDRESS_LENGTH;

    if (ctx->needsChainCode) {
        os_memmove(&apduBuffer[tx], ctx->chainCode, HASH_32_LENGTH);
        tx += HASH_32_LENGTH;
    }

    return tx;
}

////////////////////////////////////////////////////////////////////////////////
