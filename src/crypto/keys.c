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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////

bool compressPublicKey(uint8_t *publicKey, uint8_t *out, size_t outSize) {
    if (outSize != PUBLICKEY_COMPRESSED_LEN) {
        return false;
    }

    out[0] = ((publicKey[HASH_64_LEN] & 1U) ? 0x03 : 0x02);
    bytecpy(out + 1U, publicKey + 1U, HASH_32_LEN);

    return true;
}

////////////////////////////////////////////////////////////////////////////////

uint32_t setPublicKeyContext(PublicKeyContext *ctx, uint8_t *apduBuffer) {
    uint32_t tx = 0;
    apduBuffer[tx++] = PUBLICKEY_COMPRESSED_LEN;

    compressPublicKey(ctx->data, &apduBuffer[tx], PUBLICKEY_COMPRESSED_LEN);

    tx += PUBLICKEY_COMPRESSED_LEN;

    apduBuffer[tx++] = ADDRESS_LEN;

    bytecpy(&apduBuffer[tx], ctx->address, ADDRESS_LEN);

    tx += ADDRESS_LEN;

    if (ctx->needsChainCode) {
        bytecpy(&apduBuffer[tx], ctx->chainCode, HASH_32_LEN);
        tx += HASH_32_LEN;
    }

    return tx;
}

////////////////////////////////////////////////////////////////////////////////
