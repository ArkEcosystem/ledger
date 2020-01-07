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

#include "transactions/ux/display_ux.h"

#include <stdbool.h>
#include <string.h>

#include "transactions/transaction.h"

#include "transactions/types/types.h"

#include "transactions/ux/transfer_ux.h"
#include "transactions/ux/second_signature_ux.h"
#include "transactions/ux/vote_ux.h"
#include "transactions/ux/ipfs_ux.h"
#include "transactions/ux/htlc_lock_ux.h"
#include "transactions/ux/htlc_claim_ux.h"
#include "transactions/ux/htlc_refund_ux.h"

#include "display/context.h"
#include "display/display.h"

////////////////////////////////////////////////////////////////////////////////

extern void setDisplaySteps(uint8_t steps, bool isExtended);

////////////////////////////////////////////////////////////////////////////////

void setDisplay(const Transaction *transaction) {
    explicit_bzero(&displayCtx, sizeof(displayCtx));

    switch (transaction->type) {
        case TRANSFER_TYPE:
            displayTransfer(transaction);
            bool hasVendorField = transaction->vendorFieldLength > 0;
            setDisplaySteps(STEPS_TRANSFER_MIN + (uint8_t)hasVendorField,
                            hasVendorField);
            break;

        case SECOND_SIGNATURE_TYPE:
            displaySecondSignature(transaction);
            setDisplaySteps(STEPS_SECOND_SIGNATURE, false);
            break;

        case VOTE_TYPE:
            displayVote(transaction);
            setDisplaySteps(STEPS_VOTE, false);
            break;

        case IPFS_TYPE:
            displayIpfs(transaction);
            setDisplaySteps(STEPS_IPFS, true);
            break;

        case HTLC_LOCK_TYPE:
            displayHtlcLock(transaction);
            setDisplaySteps(STEPS_HTLC_LOCK,false);
            break;

        case HTLC_CLAIM_TYPE:
            displayHtlcClaim(transaction);
            setDisplaySteps(STEPS_HTLC_CLAIM, false);
            break;

        case HTLC_REFUND_TYPE:
            displayHtlcRefund(transaction);
            setDisplaySteps(STEPS_HTLC_REFUND, false);
            break;

        default: break;
    };
}

////////////////////////////////////////////////////////////////////////////////
