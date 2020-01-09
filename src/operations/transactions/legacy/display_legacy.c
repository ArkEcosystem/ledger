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

#include <stdbool.h>
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

extern void setDisplaySteps(uint8_t steps, bool isExtended);

////////////////////////////////////////////////////////////////////////////////

static void setVendorField(const Transaction *transaction) {
    bytecpy((char *)displayCtx.title[DISPLAY_CTX_EXTENDED_TITLE_INDEX],
            "VendorField",
            12);

    bytecpy((char *)displayCtx.extended_text,
            (uint8_t *)transaction->vendorField,
            transaction->vendorFieldLength);
}

////////////////////////////////////////////////////////////////////////////////

void setTransferLegacy(const Transaction *transaction) {
    bytecpy((char*)displayCtx.operation, "Transfer", 9);
    bytecpy((char*)displayCtx.title[0], "To", 3);

    bytecpy((char*)displayCtx.title[1], "Amount", 7);
    bytecpy((char*)displayCtx.title[2], "Fees", 5);

    // RecipientId
    encodeBase58PublicKey((uint8_t*)transaction->recipientId,
                          ADDRESS_HASH_LEN,
                          (uint8_t*)displayCtx.text[0],
                          sizeof(displayCtx.text[0]),
                          transaction->recipientId[0],
                          1U);
    // somehow prevents displaying bad chars?
    // legacy, so let's not spend too much time on it.
    displayCtx.text[0][ADDRESS_LEN]  = ' ';

    // Amount
    printAmount(transaction->amount,
                displayCtx.text[1],
                sizeof(displayCtx.text[1]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.text[2],
                sizeof(displayCtx.text[2]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // VendorField
    if (transaction->vendorFieldLength > 0) {
        setVendorField(transaction);
    }
}

////////////////////////////////////////////////////////////////////////////////

static void setVoteLegacy(const Transaction *transaction) {
    bytecpy((char*)displayCtx.operation, "Vote", 5);
    bytecpy((char*)displayCtx.title[0], "Vote", 5);
    bytecpy((char*)displayCtx.title[1], "Fees", 5);

    const size_t voteOffset = 67;
    bytecpy((char*)displayCtx.text[0], transaction->assetPtr, voteOffset);

    printAmount(transaction->fee,
                displayCtx.text[1],
                sizeof(displayCtx.text[1]),
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
            bool hasVendorField = transaction->vendorFieldLength > 0;
            setDisplaySteps(3U + (uint8_t)hasVendorField, hasVendorField);
            break;

        case VOTE_TYPE:
            setVoteLegacy(transaction);
            setDisplaySteps(2U, false);
            break;

        default: break;
    };
}
