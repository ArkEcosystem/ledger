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

#ifndef ARK_CONSTANTS_H
#define ARK_CONSTANTS_H

#include <stddef.h>
#include <stdint.h>

#include "platform.h"

////////////////////////////////////////////////////////////////////////////////
// General
#if defined(SUPPORTS_LARGE_OPERATIONS)
    static const size_t MAX_RAW_OPERATION   = 2496;     // 64-byte-aligned
#else
    static const size_t MAX_RAW_OPERATION   = 704;      // 64-byte-aligned
#endif

static const size_t MAX_TEXT_LEN = 255;

////////////////////////////////////////////////////////////////////////////////
// Hash
static const size_t HASH_20_LEN     = 20;
static const size_t HASH_32_LEN     = 32;
static const size_t HASH_64_LEN     = 64;

////////////////////////////////////////////////////////////////////////////////
// Identities
static const size_t ADDRESS_LEN                     = 34;
static const size_t ADDRESS_HASH_LEN                = 21;
static const size_t ADDRESS_MAX_BIP32_PATH          = 10;
static const size_t PUBLICKEY_COMPRESSED_LEN        = 33;
static const size_t PUBLICKEY_UNCOMPRESSED_LEN      = 65;

////////////////////////////////////////////////////////////////////////////////
// Labels
static const char *const UX_LABEL_AMOUNT        = "Amount";
static const char *const UX_LABEL_EXPIRATION    = "Expiration";
static const char *const UX_LABEL_FEE           = "Fee";
static const char *const UX_LABEL_RECIPIENT     = "Recipient";

////////////////////////////////////////////////////////////////////////////////
// Numbers
static const uint64_t UINT64_BASE_10 = 10U;

////////////////////////////////////////////////////////////////////////////////
// Signatures
static const size_t SIG_ECDSA_MIN_LEN   = 64;
static const size_t SIG_ECDSA_MAX_LEN   = 72;
static const size_t SIG_SCHNORR_LEN     = HASH_64_LEN;

////////////////////////////////////////////////////////////////////////////////
// Token
static const size_t TOKEN_DECIMALS          = 8U;
static const char *const TOKEN_NAME         = "ARK";
static const size_t TOKEN_NAME_LEN          = 3U;       // strlen("ARK")
static const uint8_t TOKEN_NETWORK_BYTE     = 0x1e;     // ARK Mainnet

////////////////////////////////////////////////////////////////////////////////
// Transactions
static const uint8_t TRANSACTION_HEADER             = 0xFF;

static const uint8_t TRANSACTION_VERSION_TYPE_1     = 1;
static const uint8_t TRANSACTION_VERSION_TYPE_2     = 2;

static const uint8_t VENDORFIELD_V1_MAX_LEN = 64;
static const uint8_t VENDORFIELD_V2_MAX_LEN = 255;

#endif  // #define ARK_CONSTANTS_H
