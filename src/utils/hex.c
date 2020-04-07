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

#include "utils/hex.h"

#include <stddef.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Convert Bytes to a Hex string, with null-terminator at N+1.
//
// @param const uint8_t *src:   source byte-array.
// @param size_t srcLen:        length of the byte-array.
// @param char *dst:            destination hex-string buffer.
// @param size_t dstMax:        max length of writable space.
//
// @return size_t: final length w/null-terminator if successful, otherwise '0'.
//
// ---
size_t BytesToHex(const uint8_t *src, size_t srcLen, char *dst, size_t dstMax) {
    if (src == NULL || dst == NULL || (srcLen * 2U) + 1U > dstMax) {
        return 0U;
    }

    const char *const HEX_DIGITS = "0123456789abcdef";

    size_t len = srcLen;

    do {
        *dst++ = HEX_DIGITS[(*src >> 4U) & 0xF];
        *dst++ = HEX_DIGITS[*src & 0xF];
        ++src;
    } while (len-- > 1U);

    *dst = '\0';

    return (srcLen * 2U) + 1U;
}
