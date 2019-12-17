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
// Externally Declared Methods.
extern void setDisplaySteps(uint8_t steps);

////////////////////////////////////////////////////////////////////////////////

static void setVendorField(const Transaction *transaction) {
    os_memmove((char *)displayCtx.title[1], "VendorField", 12U);

    os_memmove((char *)displayCtx.var[1],
               (uint8_t *)transaction->vendorField,
               MIN(transaction->vendorFieldLength, HASH_64_LENGTH));

    if (transaction->vendorFieldLength > HASH_64_LENGTH) {
        os_memmove((char *)&displayCtx.var[1][HASH_64_LENGTH], (char *)"...", 4U);
    }
}

////////////////////////////////////////////////////////////////////////////////

void setTransferLegacy(const Transaction *transaction) {
    os_memset(&displayCtx, 0, sizeof(displayCtx));

    os_memmove((char*)displayCtx.operation, "Transfer", 9U);
    os_memmove((char*)displayCtx.title[0], "To", 3U);

    // Let's offset the screen variables if there's a VendorField.
    // offset == 1 if VendorField; otherwise, 0.
    uint8_t offset = (transaction->vendorFieldLength != 0U);

    os_memmove((char*)displayCtx.title[1U + offset], "Amount", 7U);
    os_memmove((char*)displayCtx.title[2U + offset], "Fees", 5U);

    // Recipient
    encodeBase58PublicKey((uint8_t*)transaction->recipient,
                          ADDRESS_HASH_LENGTH,
                          (uint8_t*)displayCtx.var[0],
                          sizeof(displayCtx.var[0]),
                          transaction->recipient[0],
                          1U);
    // somehow prevents displaying bad chars?
    // legacy, so let's not spend too much time on it.
    displayCtx.var[0][ADDRESS_LENGTH]  = ' ';

    // VendorField
    if (offset) {
        setVendorField(transaction);
    }

    // Amount
    printAmount(transaction->amount,
                displayCtx.var[1U + offset],
                sizeof(displayCtx.var[1U + offset]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.var[2U + offset],
                sizeof(displayCtx.var[2U + offset]),
                TOKEN_NAME, TOKEN_NAME_LENGTH, TOKEN_DECIMALS);

    setDisplaySteps(3U  + (transaction->vendorFieldLength != 0U));
}

////////////////////////////////////////////////////////////////////////////////

void setVoteLegacy(const Transaction *transaction) {
    os_memset(&displayCtx, 0, sizeof(displayCtx));

    os_memmove((char*)displayCtx.operation, "Vote", 5U);
    os_memmove((char*)displayCtx.title[0], "Vote", 5U);
    os_memmove((char*)displayCtx.title[1], "Fees", 5U);

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
