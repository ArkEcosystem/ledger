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

#include "transactions/display.h"

#include <string.h>

#include "transactions/transaction.h"

#include "transactions/assets/transfer_display.h"
#include "transactions/assets/second_signature_display.h"
#include "transactions/assets/vote_display.h"
#include "transactions/assets/ipfs_display.h"
#include "transactions/assets/htlc_lock_display.h"
#include "transactions/assets/htlc_claim_display.h"
#include "transactions/assets/htlc_refund_display.h"

#include "ux/display_context.h"

#include "ux.h"

////////////////////////////////////////////////////////////////////////////////

extern void setDisplaySteps(uint8_t steps);

////////////////////////////////////////////////////////////////////////////////

void setDisplay(const Transaction *transaction) {
    explicit_bzero(&displayCtx, sizeof(displayCtx));

    switch (transaction->type) {
        case TRANSFER_TYPE:
            displayTransfer(transaction);
            setDisplaySteps(STEPS_TRANSFER_MIN +
                            (uint8_t)(transaction->vendorFieldLength != 0U));
            break;

        case SECOND_SIGNATURE_TYPE:
            displaySecondSignature(transaction);
            setDisplaySteps(STEPS_SECOND_SIGNATURE);
            break;

        case VOTE_TYPE:
            displayVote(transaction);
            setDisplaySteps(STEPS_VOTE);
            break;

        case IPFS_TYPE:
            displayIpfs(transaction);
            setDisplaySteps(STEPS_IPFS);
            break;

        case HTLC_LOCK_TYPE:
            displayHtlcLock(transaction);
            setDisplaySteps(STEPS_HTLC_LOCK);
            break;

        case HTLC_CLAIM_TYPE:
            displayHtlcClaim(transaction);
            setDisplaySteps(STEPS_HTLC_CLAIM);
            break;

        case HTLC_REFUND_TYPE:
            displayHtlcRefund(transaction);
            setDisplaySteps(STEPS_HTLC_REFUND);
            break;

        default: break;
    };
}

////////////////////////////////////////////////////////////////////////////////
