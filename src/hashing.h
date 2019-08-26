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

#pragma once

#include "os.h"
#include "cx.h"

//////////////////////////////////////////////////////////////////////

#define RIPEMD160_HASH_LENGTH 20U
#define RIPEMD160_TMP_LENGTH  4U

//////////////////////////////////////////////////////////////////////

void public_key_hash160(unsigned char WIDE *in,
                        unsigned short inlen,
                        unsigned char *out);

void hash_sha256(cx_sha256_t *ctx,
                 unsigned char WIDE *in,
                 unsigned short inlen,
                 unsigned char *out);