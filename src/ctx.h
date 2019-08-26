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

#include <stdbool.h>

#include "os.h"
#include "cx.h"

#include "os_io_seproxyhal.h"

#include "global.h"

typedef struct publicKeyContext_t {
    cx_ecfp_public_key_t publicKey;
    uint8_t address[41];
    uint8_t chainCode[HASH_32_LENGTH];
    bool getChaincode;
} publicKeyContext_t;

typedef struct transactionContext_t {
    cx_curve_t curve;
    uint8_t pathLength;
    uint32_t bip32Path[ADDRESS_MAX_BIP32_PATH];
    uint8_t rawTx[MAX_RAW_TX];
    uint32_t rawTxLength;
} transactionContext_t;

typedef struct txContent_t {
    uint8_t type;
    uint8_t voteSize;
    uint32_t timestamp;
    uint8_t senderPublicKey[PUBLICKEY_COMPRESSED_LENGTH];
    uint8_t recipientId[ADDRESS_HASH_LENGTH];
    uint32_t vendorFieldOffset;
    uint64_t amount;
    uint64_t fee;
    uint32_t assetOffset;
    uint8_t assetlength;
} txContent_t;
