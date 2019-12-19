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

#ifndef ARK_OPERATIONS_TRANSACTIONS_ASSETS_TRANSFER_H
#define ARK_OPERATIONS_TRANSACTIONS_ASSETS_TRANSFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////

static const size_t TRANSACTION_TYPE_TRANSFER_SIZE = 33;

////////////////////////////////////////////////////////////////////////////////

typedef struct transfer_asset_t {
    uint64_t    amount;
    uint32_t    expiration;
    uint8_t     recipientId[ADDRESS_HASH_LEN];
} Transfer;

////////////////////////////////////////////////////////////////////////////////

bool deserializeTransfer(Transfer *transfer, const uint8_t *buffer, size_t size);

////////////////////////////////////////////////////////////////////////////////

#endif
