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

#ifndef ARK_OPERATIONS_TRANSACTION_ASSETS_HTLC_LOCK_H
#define ARK_OPERATIONS_TRANSACTION_ASSETS_HTLC_LOCK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////

static const size_t TRANSACTION_TYPE_HTLC_LOCK_SIZE = 66;

////////////////////////////////////////////////////////////////////////////////

typedef struct htlc_lock_asset_t {
    uint64_t    amount;
    uint8_t     expirationType;
    uint32_t    expiration;
    uint8_t     secretHash[HASH_32_LEN];
    uint8_t     recipientId[ADDRESS_HASH_LEN];
} HtlcLock;

////////////////////////////////////////////////////////////////////////////////

bool deserializeHtlcLock(HtlcLock *lock, const uint8_t *buffer, size_t size);

////////////////////////////////////////////////////////////////////////////////

#endif
