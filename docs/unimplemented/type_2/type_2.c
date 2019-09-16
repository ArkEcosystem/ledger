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

#include "transactions/assets/type_2.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "operations/status.h"

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
// - os_memmove(registration->username, &buffer[1], registration->length)
//
// ---
ParserStatus deserializeDelegateRegistration(DelegateRegistrationAsset *registration,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
    // usernameLength + username
    if (length < 4U || length > 21U) {
        return USTREAM_FAULT;  // Incorrect Username Length
    }

    registration->length = (int)buffer[0];
    os_memmove((void *)registration->username, &buffer[1], (int)registration->length);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const char * const ui_menu_delegate_registration[][2] = {
    { "Operation",  (const char *const)amountBuffer },
    { "Username",   (const char *const)screenBuffer },
    { "Fees",       (const char *const)amountBuffer },
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static ParserStatus internalDeserializeAsset(Transaction *transaction,
                                             const uint8_t *buffer,
                                             const uint32_t length) {
/////////
    case TRANSACTION_TYPE_DELEGATE_REGISTRATION:
        return deserializeDelegateRegistration(
                        &transaction->asset.delegateRegistration,
                        &buffer[assetOffset],
                        assetLength);
        break;
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void displayTransaction(uint8_t step, bagl_element_t *element) {
/////////
        // Delegate Registration / Type 2
        else if (transaction.type == TRANSACTION_TYPE_DELEGATE_REGISTRATION) {
            displayDelegateRegistration(&transaction, step);
            element->text =
            ui_menu_delegate_registration[step][(element->component.userid) >> 4U];
        }
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setDisplaySteps(const Transaction *transaction) {
/////////
    // Delegate Registration / Type 2
    else if (transaction->type == TRANSACTION_TYPE_DELEGATE_REGISTRATION) {
        operation_set_steps(4U);
    }
/////////
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void displayDelegateRegistration(const Transaction *transaction, uint8_t step);

void displayDelegateRegistration(const Transaction *transaction, uint8_t step) {
    switch(step) {
        // Operation type
        case 0:
            os_memmove((void *)amountBuffer, "Delegate Registration\0", 22U);
            break;

        // Delegate Username
        case 1:
            os_memset((void *)screenBuffer, '\0', sizeof(screenBuffer));
            os_memmove((char *)screenBuffer,
                       transaction->asset.delegateRegistration.username,
                       (int)transaction->asset.delegateRegistration.length);
            break;

        // Fees
        case 2:
            printAmount(transaction->fee,
                        (uint8_t *)amountBuffer, sizeof(amountBuffer),
                        TOKEN_NAME, TOKEN_NAME_LENGTH,
                        TOKEN_DECIMALS);
            break;

        default: break;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
