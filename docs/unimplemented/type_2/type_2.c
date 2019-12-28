
////////////////////////////////////////////////////////////////////////////////

// The Following are only examples and places where this code could be implemented.
//  It is not final or guaranteed working.
//  This should only serve as a reference for implementing.

////////////////////////////////////////////////////////////////////////////////


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

#include "transactions/types/type_2.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Deserialize Transfer (Type 2) - 4 <=> 21 Bytes
//
// @param DelegateRegistrationAsset *registration: The Delegate Registration (Type 2) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Username Length - 1 Byte:
// - registration->length = buffer[0];
//
// Username - 3 <=> 20 Bytes:
// - bytecpy(registration->username, &buffer[1], registration->length)
//
// ---
bool deserializeDelegateRegistration(DelegateRegistration *registration,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
    // usernameLength + username
    if (length < 4U || length > 21U) {
        return false;  // Incorrect Username Length
    }

    registration->length = (int)buffer[0];
    bytecpy((void *)registration->username, &buffer[1], (int)registration->length);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static bool internalDeserializeAsset(Transaction *transaction,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
/////////
    case TRANSACTION_TYPE_DELEGATE_REGISTRATION:
        status = deserializeDelegateRegistration(
                        &transaction->asset.delegateRegistration,
                        &buffer[assetOffset],
                        assetSize);
        break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setDisplayDelegateRegistration(const Transaction *transaction,) {
    bytecpy((char *)displayCtx.operation, "Delegate Registration", 22);
    bytecpy((char *)displayCtx.title[0], "Username", 9);
    bytecpy((char *)displayCtx.title[1], "Fees", 5);

    // Username
    bytecpy((char *)displayCtx.text[0],
            transaction->asset.delegateRegistration.username,
            transaction->asset.delegateRegistration.length);
    displayCtx.text[0][transaction->asset.delegateRegistration.length] = '\0';

    // Fee
    printAmount(transaction->fee,
                (uint8_t *)displayCtx.text[1], sizeof(displayCtx.text[1]),
                TOKEN_NAME, TOKEN_NAME_SIZE, TOKEN_DECIMALS);
}

////////////////////////////////////////////////////////////////////////////////

void setDisplay(const Transaction *transaction) {
/////////
    case TRANSACTION_TYPE_SECOND_SIGNATURE:
        setDisplaySecondSignature(transaction);
        setDisplaySteps(2U);
        break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
