/*******************************************************************************
 * This file is part of the ARK Ledger App.
 *
 * Copyright (c) ARK Ecosystem <info@ark.io>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#ifndef ARK_OPERATIONS_MESSAGE_H
#define ARK_OPERATIONS_MESSAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/print.h"
#include "utils/str.h"
#include "utils/utils.h"

#include "display/context.h"
#include "display/display.h"

////////////////////////////////////////////////////////////////////////////////
static const char *const UX_MESSAGE_LABELS[]    = { "Message", "Length" };
static const size_t UX_MESSAGE_STEPS            = 2;

////////////////////////////////////////////////////////////////////////////////
extern void SetUxDisplay(size_t steps, size_t extendedStep);

////////////////////////////////////////////////////////////////////////////////
// Prepare a Message Operation for Display.
//
// - UTF8 Encoded
// - 255 Byte Max
//
// ---
bool handleMessage(const uint8_t *buffer, size_t length) {
    MEMSET_TYPE_BZERO(&displayCtx, DisplayContext);

    if (length == 0 || length > MAX_TEXT_LEN) {
        return false;
    }

    SPRINTF(displayCtx.operation, "%s", UX_MESSAGE_LABELS[0]);
    SPRINTF(displayCtx.title[0], "%s:", UX_MESSAGE_LABELS[1]);
    SPRINTF(displayCtx.title_ext, "%s:", UX_MESSAGE_LABELS[0]);

    // Message Length
    UintToString(length, displayCtx.text[0], sizeof(displayCtx.text[0]));

    // Message Text
    if (!IsPrintableAscii((const char*)buffer, length, false)) {
        return false;
    }

    SNPRINTF(displayCtx.text_ext, length + 1, "%.*s", (int)length, buffer);

    SetUxDisplay(UX_MESSAGE_STEPS, UX_MESSAGE_STEPS);

    return true;
}

#endif  // #define ARK_OPERATIONS_MESSAGE_H
