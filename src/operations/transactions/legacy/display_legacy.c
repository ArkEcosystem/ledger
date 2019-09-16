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

#include <os.h>

#include <stdint.h>

#include "transactions/legacy/display_legacy.h"

#include "constants.h"

#include "crypto/keys.h"

#include "utils/base58.h"
#include "utils/hex.h"
#include "utils/print.h"

#include "transactions/transaction.h"

#include "ux.h"
#include "ux/display_context.h"

////////////////////////////////////////////////////////////////////////////////

void setTransferLegacy(const Transaction *transaction) {
    os_memmove((char*)displayCtx.operation, "Transfer\0", 9U);
    os_memmove((char*)displayCtx.title[0], "To\0", 3U);
    os_memmove((char*)displayCtx.title[1], "Amount\0", 7U);
    os_memmove((char*)displayCtx.title[2], "Fees\0", 5U);

    encodeBase58PublicKey((uint8_t*)transaction->recipient,
                          ADDRESS_HASH_LENGTH,
                          (uint8_t*)displayCtx.var[0],
                          sizeof(displayCtx.var[0]),
                          transaction->recipient[0],
                          1U);
    // somehow prevents displaying bad chars?
    // legacy, so let's not spend too much time on it.
    displayCtx.var[0][ADDRESS_LENGTH]  = ' ';
    displayCtx.var[0][ADDRESS_LENGTH + 1]  = '\0';

    printAmount(transaction->amount,
                displayCtx.var[1],
                sizeof(displayCtx.var[1]),
                TOKEN_NAME,
                TOKEN_NAME_LENGTH,
                TOKEN_DECIMALS);

    printAmount(transaction->fee,
                displayCtx.var[2],
                sizeof(displayCtx.var[2]),
                TOKEN_NAME, TOKEN_NAME_LENGTH,
                TOKEN_DECIMALS);

    setDisplaySteps(3U);
}

////////////////////////////////////////////////////////////////////////////////

void setVoteLegacy(const Transaction *transaction) {
    os_memmove((char*)displayCtx.operation, "Vote\0", 5U);
    os_memmove((char*)displayCtx.title[0], "Vote\0", 7U);
    os_memmove((char*)displayCtx.title[1], "Fees\0", 5U);

    // V1
    if (transaction->version == TRANSACTION_VERSION_TYPE_1) {
        displayCtx.var[0][0] = (transaction->assetPtr[0] == 1U) ? '+' : '-';

        bytesToHex((char*)&displayCtx.var[0][1],
                    &transaction->assetPtr[1],
                    PUBLICKEY_COMPRESSED_LENGTH);
    }

    // Legacy
    else {
        os_memmove((char*)displayCtx.var[0], transaction->assetPtr, 67U);
        displayCtx.var[0][67] = '\0';
    }

    printAmount(transaction->fee,
                displayCtx.var[1],
                sizeof(displayCtx.var[1]),
                TOKEN_NAME,
                TOKEN_NAME_LENGTH,
                TOKEN_DECIMALS);

    setDisplaySteps(2U);
}

////////////////////////////////////////////////////////////////////////////////

// #endif  // TARGET_NANOS
