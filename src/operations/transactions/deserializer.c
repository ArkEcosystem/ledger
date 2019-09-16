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

#include "transactions/deserializer.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"
#include "utils/unpack.h"

#include "transactions/status.h"

#include "transactions/transaction.h"

#include "transactions/legacy/deserialize_legacy.h"
#include "transactions/legacy/display_legacy.h"

////////////////////////////////////////////////////////////////////////////////

Transaction transaction;

////////////////////////////////////////////////////////////////////////////////

static StreamStatus internalDeserialize(Transaction *transaction,
                                        const uint8_t *buffer,
                                        const uint32_t length) {
    StreamStatus status = USTREAM_FAULT;

    status = deserializeLegacy(transaction, buffer, length);

    // Transfer
    if (transaction->type == 0U) {
        setTransferLegacy(transaction);
    }

    // Vote
    else if (transaction->type == 3U) {
        setVoteLegacy(transaction);
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserialize(const uint8_t *buffer, const uint32_t length) {
    StreamStatus result;
    BEGIN_TRY {
        TRY {
            os_memset(&transaction, 0U, sizeof(Transaction));
            result = internalDeserialize(&transaction, buffer, length);
        }

        CATCH_OTHER(e) {
            os_memset(&transaction, 0U, sizeof(Transaction));
            result = USTREAM_FAULT;
        }

        FINALLY {}
    }
    END_TRY;

    return result;
}

////////////////////////////////////////////////////////////////////////////////
