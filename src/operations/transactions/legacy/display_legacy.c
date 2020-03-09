/*******************************************************************************
 * This file is part of the ARK Ledger App.
 *
 * Copyright (c) ARK Ecosystem <info@ark.io>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "transactions/legacy/display_legacy.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "constants.h"

#include "crypto/keys.h"

#include "transactions/transaction.h"

#include "transactions/types/types.h"

#include "transactions/ux/vendorfield_ux.h"

#include "utils/base58.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"
#include "display/display.h"

////////////////////////////////////////////////////////////////////////////////
extern void SetUxDisplay(size_t steps, bool isExtended);

////////////////////////////////////////////////////////////////////////////////
void setTransferLegacy(const Transaction *transaction) {
    SPRINTF(displayCtx.operation, "Transfer");
    SPRINTF(displayCtx.title[0], "Recipient:");
    SPRINTF(displayCtx.title[1], "Amount:");
    SPRINTF(displayCtx.title[2], "Fee:");

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
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS,
                        transaction->amount,
                        displayCtx.text[1], sizeof(displayCtx.text[1]));

    // Fee
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[2], sizeof(displayCtx.text[2]));

    // VendorField
    if (transaction->vendorFieldLength > 0) {
        setVendorField(transaction);
    }
}

////////////////////////////////////////////////////////////////////////////////
static void setVoteLegacy(const Transaction *transaction) {
    SPRINTF(displayCtx.operation, "Vote");
    SPRINTF(displayCtx.title[0], "Vote:");
    SPRINTF(displayCtx.title[1], "Fee:");

    const size_t voteOffset = 67;
    bytecpy((char*)displayCtx.text[0], transaction->assetPtr, voteOffset);

    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[1], sizeof(displayCtx.text[1]));
}

////////////////////////////////////////////////////////////////////////////////
void SetUxLegacy(const Transaction *transaction) {
    explicit_bzero(&displayCtx, sizeof(displayCtx));

    switch (transaction->type) {
        case TRANSFER_TYPE:
            setTransferLegacy(transaction);
            const bool hasVendorField = transaction->vendorFieldLength > 0;
            SetUxDisplay(3U + hasVendorField, hasVendorField);
            break;

        case VOTE_TYPE:
            setVoteLegacy(transaction);
            SetUxDisplay(2U, false);
            break;

        default: break;
    };
}
