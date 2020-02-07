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

#include "transactions/ux/transfer_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "transactions/ux/vendorfield_ux.h"

#include "utils/base58.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
void displayTransfer(const Transaction *transaction) {
    const char *const LABEL     = "Transfer";
    const size_t LABEL_SIZE     = 9;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_TO, LABEL_TO_SIZE);

    bytecpy((char *)displayCtx.title[1], LABEL_EXPIRATION, LABEL_EXPIRATION_SIZE);
    bytecpy((char *)displayCtx.title[2], LABEL_AMOUNT, LABEL_AMOUNT_SIZE);
    bytecpy((char *)displayCtx.title[3], LABEL_FEE, LABEL_FEE_SIZE);

    // RecipientId
    encodeBase58PublicKey((uint8_t *)transaction->asset.transfer.recipientId,
                          ADDRESS_HASH_LEN,
                          (uint8_t *)displayCtx.text[0],
                          sizeof(displayCtx.text[0]),
                          transaction->asset.transfer.recipientId[0],
                          1U);

    // Expiration
    printAmount(transaction->asset.transfer.expiration,
                displayCtx.text[1],
                sizeof(displayCtx.text[1]),
                NULL, 0U, 0U);

    // Amount
    printAmount(transaction->asset.transfer.amount,
                displayCtx.text[2],
                sizeof(displayCtx.text[2]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // Fee
    printAmount(transaction->fee,
                displayCtx.text[3],
                sizeof(displayCtx.text[3]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);

    // VendorField
    if (transaction->vendorFieldLength > 0U) {
        setVendorField(transaction);
    }
}
