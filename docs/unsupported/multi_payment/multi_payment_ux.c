
////////////////////////////////////////////////////////////////////////////////

// The Following are only examples and places where this code could be implemented.
//  It is not final or guaranteed working.
//  This should only serve as a reference for implementing.

////////////////////////////////////////////////////////////////////////////////

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

#include "transactions/ux/multi_payment_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/str.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
//
// Incomplete. Needs more consideration on safe logic behind displaying values.
//
void displayMultiPayment(const Transaction *transaction) {
    const char *const LABEL     = "MultiPayment";
    const size_t LABEL_SIZE     = 13;

    const char *const LABEL_COUNT       = "Payment Count";
    const size_t LABEL_COUNT_SIZE       = 14;

    const char *const LABEL_TOTAL_AMOUNT    = "Total Amount";
    const size_t LABEL_TOTAL_AMOUNT_SIZE    = 13;

    MEMCOPY(displayCtx.operation, LABEL, LABEL_SIZE);
    MEMCOPY(displayCtx.title[0], LABEL_COUNT, LABEL_COUNT_SIZE);
    MEMCOPY(displayCtx.title[1], LABEL_TOTAL_AMOUNT, LABEL_TOTAL_AMOUNT_SIZE);
    MEMCOPY(displayCtx.title[2], LABEL_FEE, LABEL_FEE_SIZE);

    // Payment Count

    // Total Amount
}
