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

#ifndef ARK_OPERATIONS_TRANSACTION_TYPE_8_H
#define ARK_OPERATIONS_TRANSACTION_TYPE_8_H

#include <stdint.h>

#include "constants.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct htlc_lock_asset_t {
    uint64_t    amount;
    uint8_t     expirationType;
    uint32_t    expiration;
    uint8_t     secretHash[HASH_32_LENGTH];
    uint8_t     recipient[ADDRESS_HASH_LENGTH];
} HtlcLock;

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserializeHtlcLock(HtlcLock *lock,
                                 const uint8_t *buffer,
                                 uint32_t length);

////////////////////////////////////////////////////////////////////////////////

#endif
