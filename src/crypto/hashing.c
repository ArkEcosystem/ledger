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

#include "crypto/hashing.h"

#include <stddef.h>
#include <stdint.h>

#include <os.h>
#include <cx.h>

////////////////////////////////////////////////////////////////////////////////

// Generate a Ripemd160 hash
void hash160(uint8_t WIDE *in, size_t inSize, uint8_t *out) {
    cx_ripemd160_t ripeHash;
    cx_ripemd160_init(&ripeHash);
    cx_hash(&ripeHash.header, CX_LAST, in, inSize, out, CX_SHA256_SIZE);
}

////////////////////////////////////////////////////////////////////////////////

void hash256(cx_sha256_t *ctx,
             uint8_t WIDE *in,
             size_t inSize,
             uint8_t *out) {
    cx_sha256_init(ctx);
    cx_hash(&ctx->header, CX_LAST, in, inSize, out, CX_SHA256_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
