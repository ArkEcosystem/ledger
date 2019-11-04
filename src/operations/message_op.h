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

// Sets the titles and variables to display a Message.
//
// Message must be: 0 < Msg <= 255
//
// -------
// Display:
//
// Message > 0 && Message <= 64
// - screen 1: Message Length
// - screen 2: \"message\"
//
// ---
//
// Message > 64 && Message <= 128
// - screen 1: Message Length
// - screen 2: \"message...\"
// - screen 3: \"...message\"
//
// ---
//
// Message > 128 && Message <= 192
// - screen 1: Message Length
// - screen 2: \"message...\"
// - screen 3: \"...message...\"
// - screen 4: \"...message\"
//
// ---
//
// Message > 192 && Message <= 255
// - screen 1: Message Length
// - screen 2: \"message...\"
// - screen 3: \"...message...\"
// - screen 4: \"...message...\"
// - screen 5: \"...message\"
//
// ---
static void internalHandleMessage(const uint8_t *buffer, const uint32_t length) {
    if (length == 0 || length > MAX_DISPLAY_BUFFER) {
        THROW(0x6A80);
    }

    // Set the operation title.
    os_memmove((char *)displayCtx.operation, "Message", 8U);

    // Set the Message Length for display.
    os_memmove((char *)displayCtx.title[0], "length:", 8U);

    printAmount(length,
                displayCtx.var[0], sizeof(displayCtx.var[0]),
                "", 0U, 0U);

    // Set the first part of the Message.
    os_memmove((char *)displayCtx.title[1], "message:", 9U);
    os_memmove((char *)displayCtx.var[1],
                buffer,
                MIN(length, HASH_64_LENGTH));
    displayCtx.var[1][MIN(length, HASH_64_LENGTH)] = '\0';

    // - 1 step for Length Display.
    // - 2..5 Steps for Message Length.
    const uint8_t steps = 1U + ((length - 1U) / HASH_64_LENGTH) + 1U;

    // Loop only executed if more than 1 screen is needed.
    // Max of 4 screens.
    //
    // - append ellipses to prior var.
    // - begin current var with ellipses.
    // - set current &message[pos] to: '&var[step][3]'
    // - null-terminate the current var: '...' + msg + '\0'
    uint8_t step    = 0U;
    uint8_t pos     = 0U;
    for (uint8_t i = 0; i < steps - 2U; ++i) {
        step    = i + 2U;
        pos     =  HASH_64_LENGTH * (i + 1U);

        // - append ellipses to the end of the last var.
        //   - if last var is screen 1:     (msg) + '...'
        //   - if last var is screen 2..3:  ('...' + msg) + '...' 
        os_memmove((char *)&displayCtx.var[i + 1U][HASH_64_LENGTH + (i ? 3U : 0U)],
                    "...", 4U);

        // Set the current title step.
        os_memmove((char *)displayCtx.title[step], "message pt  :", 14U);
        // Step to ascii
        // - add '48' to step(uint8_t) for ASCII char.
        //   - uint8_t(1)   + 48 == '1'
        //   - uint8_t(2)   + 48 == '2'
        //   - uint8_t(10)  + 48 == '10'
        displayCtx.title[step][11] = step + 48U;

        // Prepend the current var with ellipses.
        os_memmove((char *)displayCtx.var[step], "...", 3U);

        // Set the current var.
        os_memmove((char *)&displayCtx.var[step][3],
                    buffer + pos,
                    length - pos);

        // Null-terminate the prepended step var.
        // - (... + var + \0)
        displayCtx.var[step][3U + length - pos] = '\0';
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
