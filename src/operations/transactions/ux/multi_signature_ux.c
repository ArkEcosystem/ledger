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

#include "transactions/ux/multi_signature_ux.h"

#include <stddef.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/str.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
void SetUxMultiSignature(const Transaction *transaction) {
    SPRINTF(displayCtx.operation, "%s", MULTI_SIG_LABELS[0]);
    SPRINTF(displayCtx.title[0], "%s:", UX_LABEL_FEE);

    // Fee
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_LEN, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[0], sizeof(displayCtx.text[0]));

    const char* const KFMT = "%s: %d/%d";
    const size_t count = transaction->asset.multiSignature.count;
    for (size_t i = 0U, n = 0U; n < count; i += 2U, ++n) {
        // Key(N)
        SPRINTF(displayCtx.title[i + 1U], KFMT, MULTI_SIG_LABELS[1], n, count);
        BytesToHex(transaction->asset.multiSignature.keys[n],
                   PUBLICKEY_COMPRESSED_LEN,
                   displayCtx.text[i + 1U], sizeof(displayCtx.text[i + 1U]));

        // Signature(N)
        SPRINTF(displayCtx.title[i + 2U], KFMT, MULTI_SIG_LABELS[2], n, count);
        BytesToHex(transaction->asset.multiSignature.signatures[n],
                   SIG_SCHNORR_LEN,
                   displayCtx.text[i + 2U], sizeof(displayCtx.text[i + 2U]));
    }
}

#endif  // SUPPORTS_MULTISIGNATURE
