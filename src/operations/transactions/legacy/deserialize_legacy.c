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

#include "transactions/legacy/deserialize_legacy.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"
#include "utils/unpack.h"

#include "operations/status.h"
#include "transactions/transaction.h"

#include "ux.h"

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserializeLegacy(Transaction *transaction,
                               const uint8_t *buffer,
                               size_t size) {
    // V1
    if (buffer[0] == 0xFF) {
        // Deserialize Common
        transaction->header             = buffer[0];
        transaction->version            = buffer[1];
        transaction->type               = buffer[3];
        transaction->fee                = U8LE(buffer, 41U);

        transaction->vendorFieldLength  = buffer[49];
        transaction->vendorField        = (uint8_t *)&buffer[50];

        transaction->assetOffset        = (49U + buffer[49U] + 1U);
        transaction->assetSize          = size - (transaction->assetOffset);

        // Type 0: Transfer
        if (transaction->type == 0U) {
            if (transaction->assetSize != 33) {
                return USTREAM_FAULT;
            }

            transaction->amount = U8LE(
                    buffer, transaction->assetOffset);

            os_memmove(transaction->recipient,
                       &buffer[transaction->assetOffset + 12U],
                       ADDRESS_HASH_LENGTH);
        }
        // Type 3: Vote
        else if (transaction->type == 3U) { 
            if (transaction->assetSize != 35) {
                return USTREAM_FAULT;
            }
        }

        // Unknown Transaction Type
        else {
            return USTREAM_FAULT;
        }
    }

    // Legacy
    else {
        // Deserialize Common
        transaction->version        = 0U;
        transaction->type           = buffer[0];

        os_memmove(transaction->recipient, &buffer[38], ADDRESS_HASH_LENGTH);

        transaction->vendorField = (uint8_t *)&buffer[59];
        if (transaction->vendorField != 0) {
            uint8_t *ptr = transaction->vendorField;
            while (*ptr++) {
                ++transaction->vendorFieldLength;
            }
        }
        else {
            transaction->vendorFieldLength = 0U;
        }

        transaction->amount         = U8LE(buffer, 123);
        transaction->fee            = U8LE(buffer, 131);

        transaction->assetOffset    = 139;
        transaction->assetSize      = size - transaction->assetOffset;

        // Type 0: Transfer
        if (transaction->type == 0U) {
            if (transaction->assetSize != 0U) {
                return USTREAM_FAULT;
            }
        }

        // Type 3: Vote
        else if (transaction->type == 3U){
            if (transaction->assetSize != 67) {
                return USTREAM_FAULT;
            }
        }

        // Unknown Transaction Type
        else {
            return USTREAM_FAULT;
        }
    }

    transaction->assetPtr = (uint8_t*)&buffer[transaction->assetOffset];

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
