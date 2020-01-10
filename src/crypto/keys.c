/*******************************************************************************
 * This file is part of the ARK Ledger App.
 *
 * Copyright (c) ARK Ecosystem <info@ark.io>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "crypto/keys.h"

#include <stddef.h>
#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
void compressPublicKey(const cx_ecfp_public_key_t *publicKey,
                       uint8_t *out,
                       size_t outSize) {
    if (outSize != PUBLICKEY_COMPRESSED_LEN) {
        THROW(EXCEPTION_OVERFLOW);
    }

    if (publicKey->curve == CX_CURVE_256K1) {
        out[0] = ((publicKey->W[HASH_64_LEN] & 1U) ? 0x03 : 0x02);
        bytecpy(out + 1U, publicKey->W + 1U, HASH_32_LEN);
    }
    else {
        THROW(EXCEPTION);
    }
}

////////////////////////////////////////////////////////////////////////////////
uint32_t setPublicKeyContext(PublicKeyContext *ctx, uint8_t *apduBuffer) {
    uint32_t tx = 0;
    apduBuffer[tx++] = PUBLICKEY_COMPRESSED_LEN;

    compressPublicKey(&ctx->data,
                      &apduBuffer[tx],
                      PUBLICKEY_COMPRESSED_LEN);

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
