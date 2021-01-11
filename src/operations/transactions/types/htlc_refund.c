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

#include "transactions/types/htlc_refund.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
// Htlc Refund (Type 10) - 32 Bytes
//
// @param HtlcRefund *refund
// @param uint8_t *buffer: The serialized buffer at the Assets offset.
// @param size_t size: The Asset Buffer Size.
//
// @return   0: error
// @return > 0: asset size
//
// ---
// Internals:
//
// Lock Transaction Id - 32 Bytes:
// - MEMCOPY(refund->id, &buffer[0], 32);
//
// ---
size_t deserializeHtlcRefund(HtlcRefund *refund,
                             const uint8_t *buffer,
                             size_t size) {
    if (size < HASH_32_LEN) {
        return 0U;
    }

    MEMCOPY(refund->id, &buffer[0], HASH_32_LEN);               // 32 Bytes

    return HASH_32_LEN;
}
