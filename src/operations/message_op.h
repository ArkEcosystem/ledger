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
// Externally Declared Methods.
extern void setDisplaySteps(uint8_t steps);

////////////////////////////////////////////////////////////////////////////////

static const char *const LABEL_MESSAGE = "Message";
static const size_t LABEL_MESSAGE_SIZE = 8;

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
static void setMessageTitle(uint8_t *dst, uint8_t msgScreen) {
    bytecpy((char *)dst, LABEL_MESSAGE, LABEL_MESSAGE_SIZE);

    if (msgScreen > 1U) {
        const char *const LABEL_EXTRA       = " pt  :";
        const size_t LABEL_EXTRA_SIZE       = 7;

        bytecpy(&dst[LABEL_MESSAGE_SIZE - 1], LABEL_EXTRA, LABEL_EXTRA_SIZE);

        // Step to ascii
        // - add '48' to step(uint8_t) for ASCII char.
        //   - uint8_t(1)   + 48 == '1'
        //   - uint8_t(4)   + 48 == '4'
        //   - uint8_t(16)  + 48 == '16'
        const size_t ASCII_OFFSET = 48;
        const size_t MSG_NUM_OFFSET = LABEL_MESSAGE_SIZE - 1 +
                                      LABEL_EXTRA_SIZE - 3;
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
static bool internalHandleMessage(const uint8_t *buffer, size_t length) {
    if (length == 0 || length > MAX_DISPLAY_BUFFER) {
        explicit_bzero(&displayCtx, sizeof(displayCtx));
        return false;
    }

    const char *const LABEL_LENGTH = "length:";
    const size_t LABEL_LENGTH_SIZE = 8;

    const size_t MSG_ELLIPSES_SIZE = LABEL_ELLIPSES_SIZE - 1;

    // Set the operation title.
    bytecpy(displayCtx.operation, LABEL_MESSAGE, LABEL_MESSAGE_SIZE);

    // Set the Message Length for display.
    bytecpy(displayCtx.title[0], LABEL_LENGTH, LABEL_LENGTH_SIZE);
    printAmount(length,
                displayCtx.text[0], sizeof(displayCtx.text[0]),
                "", 0U, 0U);

    // Calculate the usable space inside a screen var.
    const size_t usableSize = sizeof(displayCtx.text[1]) -
                              (2 * LABEL_ELLIPSES_SIZE - 1);

    // Set the first Message Title.
    setMessageTitle(displayCtx.title[1], 1);

    // Set the first part of the Message.
    // First screen will always have at least usableSize + 3.
    bytecpy(displayCtx.text[1],
            buffer,
            MIN(length, usableSize + MSG_ELLIPSES_SIZE));
    displayCtx.text[1][MIN(length, usableSize + MSG_ELLIPSES_SIZE)] = '\0';

    // - 1 step for Length Display.
    // - 2..5 Steps for Message.
    const uint8_t steps = 1 + ((length - 1) / usableSize) + 1;

    // Loop only executed if more than 1 screen is needed.
    // Max of 4 screens.
    //
    // - append ellipses to the last screen.
    // - begin current screen with ellipses.
    // - set current screen('&message[pos]') to: '&text[step][3]'
    // - null-terminate the current screen: '...' + msg + '\0'
    uint8_t step    = 0U;
    size_t pos      = 0;
    for (uint8_t i = 0; i < steps - 2U; ++i) {
        step    = i + 2U;
        pos     =  usableSize * (i + 1);

        // - append ellipses to the end of the last var.
        //   - if last var is screen 1:     (msg) + '...'
        //   - if last var is screen 2..3:  ('...' + msg) + '...'
        bytecpy(&displayCtx
                    .text[i + 1][usableSize + (i ? MSG_ELLIPSES_SIZE : 0)],
                LABEL_ELLIPSES,
                LABEL_ELLIPSES_SIZE);

        // Set the current title.
        setMessageTitle(displayCtx.title[step], step);

        // Prepend the current var with ellipses.
        bytecpy((char *)displayCtx.text[step], LABEL_ELLIPSES,
                                              MSG_ELLIPSES_SIZE);

        // Set the current var.
        bytecpy((char *)&displayCtx.text[step][MSG_ELLIPSES_SIZE],
                buffer + pos,
                length - pos);

        // Null-terminate the prepended step var.
        // - (... + var + \0)
        displayCtx.text[step][MSG_ELLIPSES_SIZE + length - pos] = '\0';
    }

    setDisplaySteps(steps);

    return true;
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
bool handleMessage(const uint8_t *buffer, size_t length) {
    return internalHandleMessage(buffer, length);
}

////////////////////////////////////////////////////////////////////////////////

#endif
