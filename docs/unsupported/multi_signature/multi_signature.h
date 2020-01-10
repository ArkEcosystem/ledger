
////////////////////////////////////////////////////////////////////////////////

// The Following are only examples and places where this code could be implemented.
//  It is not final or guaranteed working.
//  This should only serve as a reference for implementing.

////////////////////////////////////////////////////////////////////////////////

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

#ifndef ARK_OPERATIONS_TRANSACTIONS_TYPES_MULTI_SIGNATURE_H
#define ARK_OPERATIONS_TRANSACTIONS_TYPES_MULTI_SIGNATURE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
typedef struct multi_signature_asset_t {
    uint8_t     min;
    uint8_t     count;
    uint8_t     keys[528];  // 16 * 33 keys max
} MultiSignature;

////////////////////////////////////////////////////////////////////////////////
bool deserializeMultiSignature(MultiSignature *multiSignature,
                               const uint8_t *buffer,
                               size_t length);

#endif  // #define ARK_OPERATIONS_TRANSACTIONS_TYPES_MULTI_SIGNATURE_H
