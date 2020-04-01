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

#include "transactions/ux/htlc_lock_ux.h"

#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "transactions/ux/vendorfield_ux.h"

#include "utils/base58.h"
#include "utils/hex.h"
#include "utils/print.h"
#include "utils/str.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
void SetUxHtlcLock(const Transaction *transaction) {
    SPRINTF(displayCtx.operation, "%s", UX_HTLC_LOCK_LABELS[0]);
    SPRINTF(displayCtx.title[0], "%s:", UX_LABEL_RECIPIENT);
    SPRINTF(displayCtx.title[1], "%s:", UX_HTLC_LOCK_LABELS[1]);
    SPRINTF(displayCtx.title[2], "%s:", UX_LABEL_EXPIRATION);
    SPRINTF(displayCtx.title[3], "%s:", UX_LABEL_AMOUNT);
    SPRINTF(displayCtx.title[4], "%s:", UX_LABEL_FEE);

    // RecipientId
    Base58CheckEncode(transaction->asset.htlcLock.recipientId, ADDRESS_HASH_LEN,
                      displayCtx.text[0], sizeof(displayCtx.text[0]));

    // Secret Hash
    BytesToHex(transaction->asset.htlcLock.secretHash, HASH_32_LEN,
               displayCtx.text[1], sizeof(displayCtx.text[1]));

    // Expiration
    const bool isTime = transaction->asset.htlcLock.expirationType == 1U;
    SPRINTF(displayCtx.text[2], "%s: ", isTime ? UX_HTLC_LOCK_SUB_LABELS[0]
                                               : UX_HTLC_LOCK_SUB_LABELS[1]);
    const size_t expirationOffset = strlen(displayCtx.text[2]);
    UintToString(transaction->asset.htlcLock.expiration,
                 &displayCtx.text[2][expirationOffset],
                 sizeof(displayCtx.text[2]));

    // Amount
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_LEN, TOKEN_DECIMALS,
                        transaction->asset.htlcLock.amount,
                        displayCtx.text[3], sizeof(displayCtx.text[3]));

    // Fees
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_LEN, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[4], sizeof(displayCtx.text[4]));

    // VendorField
    if (transaction->vendorFieldLength > 0U) {
        setVendorField(transaction);
    }
}
