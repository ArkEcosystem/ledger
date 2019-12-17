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

#include <stddef.h>
#include <stdint.h>

#include <os.h>

#include "constants.h"

#include "operations/status.h"

#include "utils/hex.h"
#include "utils/print.h"

#include "ux.h"
#include "ux/display_context.h"

////////////////////////////////////////////////////////////////////////////////
// Externally Declared Methods.
extern void setDisplaySteps(uint8_t steps);

////////////////////////////////////////////////////////////////////////////////

#define ELLIPSES            "..."
#define ELLIPSES_SIZE       sizeof(ELLIPSES)

#define MSG_TITLE           "message:"
#define MSG_TITLE_SIZE      sizeof(MSG_TITLE)

#define MSG_TITLE_EXTRA         " pt  :"
#define MSG_TITLE_EXTRA_SIZE    sizeof(MSG_TITLE_EXTRA)

#define MSG_NUM_OFFSET      MSG_TITLE_SIZE - 2 + MSG_TITLE_EXTRA_SIZE - 3
#define ASCII_OFFSET        48

////////////////////////////////////////////////////////////////////////////////

// Set the Screen Title for a given Message display segment.
//
// @param: uint8_t *dst
// @param: uint8_t msgScreen
//
// -------
// Title:
//
// - screen 1:    \"message:\"
// - screen 2..N: \"message pt N:\"
//
// ---
static void internalSetMessageTitle(uint8_t *dst, uint8_t msgScreen) {
    os_memmove((char *)dst, MSG_TITLE, MSG_TITLE_SIZE);

    if (msgScreen > 1U) {
        os_memmove(&dst[MSG_TITLE_SIZE - 2U],
                    MSG_TITLE_EXTRA,
                    MSG_TITLE_EXTRA_SIZE);
        // Step to ascii
        // - add '48' to step(uint8_t) for ASCII char.
        //   - uint8_t(1)   + 48 == '1'
        //   - uint8_t(16)  + 48 == '16'
        dst[MSG_NUM_OFFSET] = msgScreen + ASCII_OFFSET;
    }
}

////////////////////////////////////////////////////////////////////////////////

// Sets the titles and variables to display a Message.
//
// Message must be: 0 < Msg <= 255
//
// @param: const uint8_t *buffer
// @param: size_t length
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
static void internalHandleMessage(const uint8_t *buffer, size_t length) {
    if (length == 0 || length > MAX_DISPLAY_BUFFER) {
        THROW(0x6A80);
    }

    // Set the operation title.
    os_memmove(displayCtx.operation, "Message", 8);

    // Set the Message Length for display.
    os_memmove(displayCtx.title[0], "length:", 8);
    printAmount(length,
                displayCtx.var[0], sizeof(displayCtx.var[0]),
                "", 0U, 0U);

    // Calculate the usable space inside a screen var.
    const size_t usableSize = sizeof(displayCtx.var[1]) -
                              (2 * ELLIPSES_SIZE - 1);

    // Set the first Message Title.
    internalSetMessageTitle(displayCtx.title[1], 1);

    // Set the first part of the Message.
    // First screen will always have at least usableSize + 3.
    os_memmove(displayCtx.var[1],
               buffer,
               MIN(length, usableSize + ELLIPSES_SIZE - 1));
    displayCtx.var[1][MIN(length, usableSize + ELLIPSES_SIZE - 1)] = '\0';

    // - 1 step for Length Display.
    // - 2..5 Steps for Message.
    const uint8_t steps = 1 + ((length - 1) / usableSize) + 1;

    // Loop only executed if more than 1 screen is needed.
    // Max of 4 screens.
    //
    // - append ellipses to the last screen.
    // - begin current screen with ellipses.
    // - set current screen('&message[pos]') to: '&var[step][3]'
    // - null-terminate the current screen: '...' + msg + '\0'
    uint8_t step    = 0U;
    size_t pos      = 0;
    for (uint8_t i = 0; i < steps - 2U; ++i) {
        step    = i + 2U;
        pos     =  usableSize * (i + 1);

        // - append ellipses to the end of the last var.
        //   - if last var is screen 1:     (msg) + '...'
        //   - if last var is screen 2..3:  ('...' + msg) + '...'
        os_memmove(&displayCtx
                        .var[i + 1][usableSize + (i ? ELLIPSES_SIZE - 1 : 0)],
                   ELLIPSES,
                   ELLIPSES_SIZE);

        // Set the current title.
        internalSetMessageTitle(displayCtx.title[step], step);

        // Prepend the current var with ellipses.
        os_memmove((char *)displayCtx.var[step], ELLIPSES, ELLIPSES_SIZE - 1);

        // Set the current var.
        os_memmove((char *)&displayCtx.var[step][ELLIPSES_SIZE - 1],
                    buffer + pos,
                    length - pos);

        // Null-terminate the prepended step var.
        // - (... + var + \0)
        displayCtx.var[step][ELLIPSES_SIZE - 1 + length - pos] = '\0';
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
void handleMessage(const uint8_t *buffer, size_t length) {
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
