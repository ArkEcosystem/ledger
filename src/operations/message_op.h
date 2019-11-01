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

#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "operations/status.h"

#include "utils/hex.h"
#include "utils/print.h"

#include "ux.h"
#include "ux/display_context.h"

////////////////////////////////////////////////////////////////////////////////

static void internalHandleMessage(const uint8_t *buffer, const uint32_t length) {
    if (length == 0 || length > MAX_DISPLAY_BUFFER) {
        THROW(EXCEPTION_OVERFLOW);
    }

    os_memmove((char *)displayCtx.operation, "Message", 8U);

    // Set the Message Length for display.
    os_memmove((char *)displayCtx.title[0], "length:", 8U);
    printAmount(length,
                displayCtx.var[0], sizeof(displayCtx.var[0]),
                "", 0U, 0U);

    uint8_t steps = 0U;

    // Set the first part of the Message.
    os_memmove((char *)displayCtx.title[1], "message:", 9U);
    os_memmove((char *)displayCtx.var[1],
               buffer,
               length < HASH_64_LENGTH ? length : HASH_64_LENGTH);

    displayCtx.var[1][HASH_64_LENGTH] = '\0';

    steps = 2U;
    
    // - add ellipses to the end of the first var.
    // - set the second var.
    if (length > HASH_64_LENGTH) {
        os_memmove((char *)&displayCtx.var[1][HASH_64_LENGTH], "...", 4U);

        os_memmove((char *)displayCtx.title[2], "message pt 2:", 14U);
        os_memmove((char *)displayCtx.var[2], "...", 3U);

        os_memmove((char *)&displayCtx.var[2][3],
                   buffer + HASH_64_LENGTH,
                   length < HASH_64_LENGTH * 2U
                            ? length - HASH_64_LENGTH
                            : HASH_64_LENGTH * 2U);

        displayCtx.var[2][3U + length - HASH_64_LENGTH] = '\0';

        steps = 3U;
    }

    // if the buffer length > 128
    // - add ellipses to the end of the second var.
    // - set the third var.
    if (length > HASH_64_LENGTH * 2U) {
        os_memmove((char *)&displayCtx.var[2][HASH_64_LENGTH], "...", 4U);

        os_memmove((char *)displayCtx.title[3], "message pt 3:", 14U);
        os_memmove((char *)displayCtx.var[3], "...", 3U);

        os_memmove((char *)&displayCtx.var[3][3],
                   buffer + HASH_64_LENGTH * 2U,
                   length < HASH_64_LENGTH * 3U
                            ? length - HASH_64_LENGTH * 2U
                            : HASH_64_LENGTH * 3U);

        displayCtx.var[3][3U + length - HASH_64_LENGTH * 2U] = '\0';

        steps = 4U;
    }

    // if the buffer length > 192
    // - add ellipses to the end of the third var.
    // - set the fourth var.
    if (length > HASH_64_LENGTH * 3U) {
        os_memmove((char *)&displayCtx.var[3][HASH_64_LENGTH], "...", 4U);

        os_memmove((char *)displayCtx.title[4], "message pt 4:", 14U);
        os_memmove((char *)displayCtx.var[4], "...", 3U);

        os_memmove((char *)&displayCtx.var[4][3],
                   buffer + HASH_64_LENGTH * 3U,
                   length - HASH_64_LENGTH * 3U);

        displayCtx.var[4][3U + length - HASH_64_LENGTH * 3U] = '\0';

        steps = 5U;
    }

    setDisplaySteps(steps);
}

////////////////////////////////////////////////////////////////////////////////

// Prepare a Message Operation for Display.
//
// - UTF8 Encoded
// - 255 Byte Max
// - Multiple display steps dependent on length
// - Multiple steps prepended/appended by ellipses (...)
//
// ---
void handleMessage(const uint8_t *buffer, const uint32_t length) {
    BEGIN_TRY {
        TRY {
            internalHandleMessage(buffer, length);
        }

        CATCH_OTHER(e) {
            explicit_bzero(&displayCtx, sizeof(displayCtx));
        }

        FINALLY {}
    }
    END_TRY;
}

////////////////////////////////////////////////////////////////////////////////

#endif
