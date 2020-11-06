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

#include "transactions/ux/vote_ux.h"

#include <stdbool.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/str.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////
void SetUxVote(const Transaction *transaction) {
    SPRINTF(displayCtx.operation, "%s", UX_VOTE_LABELS[1]);

    // Fee
    SPRINTF(displayCtx.title[0], "%s:", UX_LABEL_FEE);
    TokenAmountToString(TOKEN_NAME, TOKEN_NAME_LEN, TOKEN_DECIMALS,
                        transaction->fee,
                        displayCtx.text[0], sizeof(displayCtx.text[0]));

    // Vote(s)
    for (uint8_t i = 0U; i < transaction->asset.vote.count; ++i) {
        const char* voteTag = transaction->asset.vote.data[i][0] == 1U
                ? UX_VOTE_LABELS[1]     // vote
                : UX_VOTE_LABELS[0];    // unvote

        SPRINTF(displayCtx.title[UX_VOTE_BASE_STEPS + i], "%s: ", voteTag);
        BytesToHex(&transaction->asset.vote.data[i][1], PUBLICKEY_COMPRESSED_LEN,
                   displayCtx.text[UX_VOTE_BASE_STEPS + i],
                   sizeof(displayCtx.text[UX_VOTE_BASE_STEPS + i]));
    }
}
