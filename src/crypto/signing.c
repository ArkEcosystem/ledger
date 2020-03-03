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

#include "crypto/signing.h"

#include <stdint.h>

#include <os.h>
#include <cx.h>

#include "constants.h"

#include "schnorr_bcrypto_410.h"

////////////////////////////////////////////////////////////////////////////////
uint32_t signEcdsa(const cx_ecfp_private_key_t *privateKey,
                   const uint8_t *hash,
                   uint8_t *signature,
                   size_t signatureSize) {
    if (privateKey == NULL || hash == NULL || signature == NULL ||
        signatureSize < SIG_ECDSA_MIN_LEN ||
        signatureSize > SIG_ECDSA_MAX_LEN) {
        return 0;
    }

    return cx_ecdsa_sign(privateKey,
                         CX_RND_RFC6979 | CX_LAST, CX_SHA256,
                         hash, HASH_32_LEN,
                         signature, signatureSize,
                         NULL);
} 

////////////////////////////////////////////////////////////////////////////////
uint32_t signSchnorr(const cx_ecfp_private_key_t *privateKey,
                     const uint8_t *hash,
                     uint8_t *signature) {
    if (privateKey == NULL || hash == NULL || signature == NULL) {
        return 0;
    }

    return schnorr_sign_bcrypto_410(privateKey->d, hash, signature);
}
