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

#ifndef ARK_OPERATIONS_TRANSACTION_H
#define ARK_OPERATIONS_TRANSACTION_H

#include <stdint.h>

#include "constants.h"

#include "transactions/assets/types.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct transaction_t {
    uint8_t     header;
    uint8_t     version;
    uint16_t    type;
    uint8_t     senderPublicKey[PUBLICKEY_COMPRESSED_LENGTH];
    uint64_t    fee;
    uint8_t     vendorFieldLength;
    uint8_t     *vendorField;
    union {
        struct {  // v1 or Legacy
            uint8_t     recipient[ADDRESS_HASH_LENGTH];
            uint64_t    amount;
            uint32_t    assetOffset;
            uint8_t     assetLength;
            uint8_t     *assetPtr;
        };
        struct {  // v2
            tx_asset_t  asset;
        };
    };
} Transaction;

////////////////////////////////////////////////////////////////////////////////

#endif
