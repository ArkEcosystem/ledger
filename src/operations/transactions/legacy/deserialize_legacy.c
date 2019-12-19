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

#include "transactions/offsets.h"
#include "transactions/transaction.h"

#include "ux.h"

#include "utils/unpack.h"
#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////

void deserializeCommonLegacy(Transaction *transaction,
                             const uint8_t *buffer,
                             size_t size) {
    // Deserialize Common
    transaction->version        = TRANSACTION_VERSION_LEGACY;
    transaction->type           = buffer[TYPE_OFFSET_LEGACY];

    bytecpy(transaction->recipientId,
            &buffer[RECIPIENT_OFFSET_LEGACY],
            ADDRESS_HASH_LEN);

    transaction->vendorField = (uint8_t *)&buffer[VF_OFFSET];
    if (buffer[VF_OFFSET] != 0) {
        uint8_t *ptr = transaction->vendorField;
        while (*ptr++) {
            ++transaction->vendorFieldLength;
        }
    }
    else {
        transaction->vendorFieldLength = 0U;
    }

    transaction->amount         = U8LE(buffer, AMOUNT_OFFSET_LEGACY);
    transaction->fee            = U8LE(buffer, FEE_OFFSET_LEGACY);

    transaction->assetOffset    = ASSET_OFFSET_LEGACY;
    transaction->assetSize      = size - transaction->assetOffset;
}

////////////////////////////////////////////////////////////////////////////////
