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

#include "transactions/ux/vote_ux.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////

void displayVote(const Transaction *transaction) {
    const char *const LABEL_VOTE    = "Vote";
    const size_t LABEL_VOTE_SIZE    = 5;

    const char *const LABEL_UNVOTE  = "Unvote";
    const size_t LABEL_UNVOTE_SIZE  = 7;

    const char SYMBOL_PLUS      = '+';
    const char SYMBOL_MINUS     = '-';

    bool isVote = (transaction->asset.vote.data[0] == 1U);
    const char *voteLabel       = isVote ? LABEL_VOTE : LABEL_UNVOTE;
    const size_t voteLabelSize  = isVote ? LABEL_VOTE_SIZE : LABEL_UNVOTE_SIZE;
    const char voteSymbol       = isVote ? SYMBOL_PLUS : SYMBOL_MINUS;

    bytecpy((char *)displayCtx.operation, LABEL_VOTE, LABEL_VOTE_SIZE);
    bytecpy((char *)displayCtx.title[0], voteLabel, voteLabelSize);
    bytecpy((char *)displayCtx.title[1], LABEL_FEE, LABEL_FEE_SIZE);

    // Vote
    displayCtx.var[0][0] = voteSymbol;
    bytesToHex((char *)&displayCtx.var[0][1],
                &transaction->asset.vote.data[1],
                PUBLICKEY_COMPRESSED_LEN);

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.var[1], sizeof(displayCtx.var[1]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////
