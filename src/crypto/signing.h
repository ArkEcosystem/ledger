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

#ifndef ARK_CRYPTO_SIGNING_H
#define ARK_CRYPTO_SIGNING_H

#include <stdbool.h>
#include <stddef.h>
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
    bool        isSchnorr;
} SigningContext;

////////////////////////////////////////////////////////////////////////////////
uint32_t signEcdsa(const cx_ecfp_private_key_t *privateKey,
                   const uint8_t *hash,
                   uint8_t *signature,
                   size_t signatureSize);

////////////////////////////////////////////////////////////////////////////////
uint32_t signSchnorr(const cx_ecfp_private_key_t *privateKey,
                     const uint8_t *hash,
                     uint8_t *signature);

#endif  // #define ARK_CRYPTO_SIGNING_H
