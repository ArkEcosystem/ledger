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

#include "transactions/legacy/display_legacy.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "constants.h"

#include "crypto/keys.h"

#include "transactions/transaction.h"

#include "transactions/types/types.h"

#include "utils/base58.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"
#include "display/display.h"

////////////////////////////////////////////////////////////////////////////////
// Externally Declared Methods.
extern void setDisplaySteps(uint8_t steps);

////////////////////////////////////////////////////////////////////////////////

static void setVendorField(const Transaction *transaction) {
    bytecpy((char *)displayCtx.title[1], "VendorField", 12);

    bytecpy((char *)displayCtx.var[1],
            (uint8_t *)transaction->vendorField,
            MIN(transaction->vendorFieldLength, HASH_64_LEN));

    if (transaction->vendorFieldLength > HASH_64_LEN) {
        bytecpy((char *)&displayCtx.var[1][HASH_64_LEN], (char *)"...", 4);
    }
}

////////////////////////////////////////////////////////////////////////////////

void setTransferLegacy(const Transaction *transaction) {
    bytecpy((char*)displayCtx.operation, "Transfer", 9);
    bytecpy((char*)displayCtx.title[0], "To", 3);

    // Let's offset the screen variables if there's a VendorField.
    // offset == 1 if VendorField; otherwise, 0.
    size_t offset = (transaction->vendorFieldLength > 0);

    bytecpy((char*)displayCtx.title[1 + offset], "Amount", 7);
    bytecpy((char*)displayCtx.title[2 + offset], "Fees", 5);

    // RecipientId
    encodeBase58PublicKey((uint8_t*)transaction->recipientId,
                          ADDRESS_HASH_LEN,
                          (uint8_t*)displayCtx.var[0],
                          sizeof(displayCtx.var[0]),
                          transaction->recipientId[0],
                          1U);
    // somehow prevents displaying bad chars?
    // legacy, so let's not spend too much time on it.
    displayCtx.var[0][ADDRESS_LEN]  = ' ';

    // VendorField
    if (offset) {
        setVendorField(transaction);
    }

    // Amount
    printAmount(transaction->amount,
                displayCtx.var[1 + offset],
                sizeof(displayCtx.var[1 + offset]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.var[2 + offset],
                sizeof(displayCtx.var[2 + offset]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

static void setVoteLegacy(const Transaction *transaction) {
    bytecpy((char*)displayCtx.operation, "Vote", 5);
    bytecpy((char*)displayCtx.title[0], "Vote", 5);
    bytecpy((char*)displayCtx.title[1], "Fees", 5);

    const size_t voteOffset = 67;
    bytecpy((char*)displayCtx.var[0], transaction->assetPtr, voteOffset);

    printAmount(transaction->fee,
                displayCtx.var[1],
                sizeof(displayCtx.var[1]),
                TOKEN_NAME,
                TOKEN_NAME_SIZE,
                TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

void setDisplayLegacy(const Transaction *transaction) {
    explicit_bzero(&displayCtx, sizeof(displayCtx));

    switch (transaction->type) {
        case TRANSFER_TYPE:
            setTransferLegacy(transaction);
            setDisplaySteps(3U + (transaction->vendorFieldLength != 0U));
            break;

        case VOTE_TYPE:
            setVoteLegacy(transaction);
            setDisplaySteps(2U);
            break;

        default: break;
    };
}
