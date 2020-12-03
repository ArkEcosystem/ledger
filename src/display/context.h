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

#ifndef ARK_DISPLAY_CONTEXT_H
#define ARK_DISPLAY_CONTEXT_H

#include <stddef.h>

#include "platform.h"

////////////////////////////////////////////////////////////////////////////////
static const size_t DISPLAY_CTX_TEXT_SIZE_EXT   = 256;

#if defined(SUPPORTS_LARGE_OPERATIONS)
    static const size_t DISPLAY_CTX_OP_SIZE     = 21;
    static const size_t DISPLAY_CTX_STEP_COUNT  = 40;
    static const size_t DISPLAY_CTX_TITLE_SIZE  = DISPLAY_CTX_OP_SIZE - 1;
    static const size_t DISPLAY_CTX_TEXT_SIZE   = 130;
#else
    static const size_t DISPLAY_CTX_OP_SIZE     = 18;
    static const size_t DISPLAY_CTX_STEP_COUNT  = 5;
    static const size_t DISPLAY_CTX_TITLE_SIZE  = DISPLAY_CTX_OP_SIZE - 3;
    static const size_t DISPLAY_CTX_TEXT_SIZE   = 68;
#endif

////////////////////////////////////////////////////////////////////////////////
typedef struct display_context_t {
    char    operation   [DISPLAY_CTX_OP_SIZE];
    char    title       [DISPLAY_CTX_STEP_COUNT] [DISPLAY_CTX_TITLE_SIZE];
    char    text        [DISPLAY_CTX_STEP_COUNT] [DISPLAY_CTX_TEXT_SIZE];

    // For large display texts.
    // Should always be the last step in a UX display flow.
    //
    // e.g. Message, VendorField, IPFS DAG
    char    title_ext   [DISPLAY_CTX_TITLE_SIZE];
    char    text_ext    [DISPLAY_CTX_TEXT_SIZE_EXT];
} DisplayContext;

////////////////////////////////////////////////////////////////////////////////
extern DisplayContext displayCtx;

#endif  // ARK_DISPLAY_CONTEXT_H