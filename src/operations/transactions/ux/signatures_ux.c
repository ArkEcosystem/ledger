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

#include "platform.h"

#if defined(SUPPORTS_MULTISIGNATURE)

#include "transactions/ux/signatures_ux.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"

#include "constants.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
size_t SetUxSignatures(const Transaction *transaction, size_t offset) {
    const char *FMT = "%s: %d/%d";
    for (size_t i = 0; i < transaction->signatures.count; ++i) {
        SPRINTF(displayCtx.title[offset + i], FMT,
                UX_SIGNATURES_LABEL, i + 1U, transaction->signatures.count);

        BytesToHex(transaction->signatures.data[i], SIG_SCHNORR_LEN,
                   displayCtx.text[offset + i],
                   sizeof(displayCtx.text[offset + i]));
    }

    return transaction->signatures.count;
}

#endif  // SUPPORTS_MULTISIGNATURE
