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

#ifndef ARK_OPERATIONS_MESSAGE_H
#define ARK_OPERATIONS_MESSAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/hex.h"
#include "utils/print.h"
#include "utils/utils.h"

#include "display/context.h"
#include "display/display.h"

////////////////////////////////////////////////////////////////////////////////

extern void setDisplaySteps(uint8_t steps, bool isExtended);

////////////////////////////////////////////////////////////////////////////////

// Prepare a Message Operation for Display.
//
// - UTF8 Encoded
// - 255 Byte Max
//
// ---
bool handleMessage(const uint8_t *buffer, size_t length) {
    if (length == 0 || length > MAX_TEXT_LEN) {
        return false;
    }

    explicit_bzero(&displayCtx, sizeof(displayCtx));

    const char *const LABEL     = "Message";
    const size_t LABEL_SIZE     = 8;

    const char *const LABEL_LENGTH      = "length:";
    const size_t LABEL_LENGTH_SIZE      = 8;

    const size_t MESSAGE_DISPLAY_STEPS = 2;

    bytecpy((char *)displayCtx.operation, LABEL, LABEL_SIZE);
    bytecpy((char *)displayCtx.title[0], LABEL_LENGTH, LABEL_LENGTH_SIZE);
    bytecpy((char *)displayCtx.title[DISPLAY_CTX_EXTENDED_TITLE_INDEX],
            LABEL,
            LABEL_SIZE);

    // Message Length
    printAmount(length,
                displayCtx.text[0],
                sizeof(displayCtx.text[0]),
                "", 0U, 0U);

    // Message Text
    bytecpy((char *)displayCtx.extended_text, buffer, length);

    setDisplaySteps(MESSAGE_DISPLAY_STEPS, true);

    return true;
}

////////////////////////////////////////////////////////////////////////////////

#endif
