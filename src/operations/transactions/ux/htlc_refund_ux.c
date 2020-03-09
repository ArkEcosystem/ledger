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

#include "transactions/ux/htlc_refund_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
void displayHtlcRefund(const Transaction *transaction) {
    const char *const LABEL     = "HTLC Refund";
    const size_t LABEL_SIZE     = 12;

    const char *const LABEL_LOCK_ID     = "Lock Id";
    const size_t LABEL_LOCK_ID_SIZE     = 8;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_LOCK_ID, LABEL_LOCK_ID_SIZE);

    // Lock Id
    BytesToHex(transaction->asset.htlcRefund.id, HASH_32_LEN,
               displayCtx.text[0], sizeof(displayCtx.text[0]));
}
