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

#include "hashing.h"

#include "os.h"
#include "cx.h"

#include "global.h"

// Generate a Ripemd160 hash from a PublicKey
void public_key_hash160(unsigned char WIDE *in,
                        unsigned short inlen,
                        unsigned char *out) {
    union {
        cx_sha256_t shasha;
        cx_ripemd160_t riprip;
    } u;

    cx_ripemd160_init(&u.riprip);
    cx_hash(&u.riprip.header, CX_LAST, in, inlen, out, HASH_32_LENGTH);
}

//////////////////////////////////////////////////////////////////////

void hash_sha256(cx_sha256_t *ctx,
                 unsigned char WIDE *in,
                 unsigned short inlen,
                 unsigned char *out) {
    cx_sha256_init(ctx);
    cx_hash(&ctx->header, CX_LAST, in, inlen, out, HASH_32_LENGTH);
}
