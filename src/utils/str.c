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

#include "utils/str.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
#define PRINTABLE_CHAR_MIN 32
#define PRINTABLE_CHAR_MAX 126

////////////////////////////////////////////////////////////////////////////////
// Verifies that a string of a given length contains printable Ascii text.
//
// e.g.
// - IsPrintableAscii("AbCd", 5, true) == true
// - IsPrintableAscii((char[]){ 'A', 'b', 'C', 'd'}, 4, false) == true
// - IsPrintableAscii("\
//  ", 50, false) == false
//
// @param const char *str:          the string to be checked.
// @param size_t length:            string length.
// @param bool isNullTerminated:    whether to account for the null-terminator.
//
// @return bool: true if valid.
//
// ---
bool IsPrintableAscii(const char *str, size_t length, bool isNullTerminated) {
    const int target = length - (int)isNullTerminated;
    for (int i = 0; i < target; ++i) {
      int c = (int)str[i];
      if ((c - PRINTABLE_CHAR_MIN) * (PRINTABLE_CHAR_MAX - c) < 0) {
        return false;
      }
    }

    return isNullTerminated ? (str[target] == '\0') : true;
}

////////////////////////////////////////////////////////////////////////////////
// Copy a Number-String, inserting a decimal '.' at a given location.
//
// e.g.
// - adjustDecimals("10000", 5, buffer, 6, 2);
// - "100.00"
//
// @param const char *src:      original string.
// @param size_t srcSize:       size of the original string, w/null-terminator.
// @param char *target:         destination where result will be copied.
// @param size_t targetSize:    max length of writable space in destination.
// @param size_t decimals:      decimal precision / how many values after '.'.
//
// @return size_t: final string length if successful, otherwise '0'.
//
// ---
static size_t adjustDecimals(const char *src, size_t srcSize,
                             char *target, size_t targetSize,
                             size_t decimals) {
    size_t startOffset;
    size_t lastZeroOffset = 0U;
    size_t offset = 0U;

    if ((srcSize == 1U) && (*src == '0')) {
        if (targetSize < 2U) {
            return 0U;
        }
        target[offset++] = '0';
        target[offset++] = '\0';
        return offset;
    }

    if (srcSize <= decimals) {
        size_t delta = decimals - srcSize;
        if (targetSize < srcSize + 1U + 2U + delta) {
            return offset;
        }

        target[offset++] = '0';
        if (decimals > 0U) {
          target[offset++] = '.';
        }
        
        for (size_t i = 0U; i < delta; i++) {
            target[offset++] = '0';
        }

        startOffset = offset;
        for (size_t i = 0U; i < srcSize; i++) {
            target[offset++] = src[i];
        }

        target[offset] = '\0';
    }
    else {
        size_t sourceOffset = 0U;
        size_t delta = srcSize - decimals;

        if (targetSize < srcSize + 1U + 1U) {
            return offset;
        }

        while (offset < delta) {
            target[offset++] = src[sourceOffset++];
        }

        if (decimals != 0U) {
            target[offset++] = '.';
        }

        startOffset = offset;
        while (sourceOffset < srcSize) {
            target[offset++] = src[sourceOffset++];
        }

        target[offset] = '\0';
    }

    for (size_t i = startOffset; i < offset; i++) {
        if (target[i] == '0') {
            if (lastZeroOffset == 0U) {
                lastZeroOffset = i;
            }
        }
        else {
            lastZeroOffset = 0U;
        }
    }

    if (lastZeroOffset != 0U) {
        target[lastZeroOffset] = '\0';
        if (target[lastZeroOffset - 1U] == '.') {
            target[lastZeroOffset - 1U] = '\0';
        }
    }

    return offset + lastZeroOffset;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Convert a Unsigned Integer to a String using a Token Name and Decimal count.
//
// e.g.
// - UintToString(1ULL, (char[])buffer, 25);
// - "1"
//
// @param uint64_t value:   unsigned value to be converted.
// @param char *dst:        destination char buffer.
// @param size_t maxLen:    max length of writable space.
//
// @return size_t: final length w/null-terminator if successful, otherwise '0'.
//
// ---
size_t UintToString(uint64_t value, char *dst, size_t maxLen) {
    if (dst == NULL || maxLen < 2U) {
        return 0U;
    }

    if (value == 0U) {
        dst[0] = '0';
        dst[1] = '\0';
        return 2U;
    }

    uint64_t base = 1U;
    size_t n = 0U;
    size_t i = 0U;

    // count how many characters are needed
    while (base <= value && n <= UINT64_MAX_STRING_SIZE) {
        base *= UINT64_BASE_10;
        n++;
    }

    if (n > maxLen - 1U) {
        dst[0] = '\0';
        return 0U;
    }

    base /= UINT64_BASE_10;
    while (i < n) {
        dst[i++] = '0' + ((value / base) % UINT64_BASE_10);
        base /= UINT64_BASE_10;
    }

    dst[i] = '\0';

    return n + 1U;
}

////////////////////////////////////////////////////////////////////////////////
// Convert an Amount to a String using a Token Name and Decimal count.
//
// e.g.
// - TokenAmountToString("ARK ", 4, 8, 1ULL, 25);
// - "ARK: 0.00000001"
//
// @param const char *token:    token/ticker name.
// @param size_t tokenLen:      length of token, excluding the null-terminator.
// @param size_t decimals:      decimal precision / how many values after '.'.
// @param uint64_t amount:      unsigned value to be converted.
// @param char *dst:            destination char buffer.
// @param size_t maxLen:        max length of writable space.
//
// @return size_t: final length w/null-terminator if successful, otherwise '0'.
//
// ---
size_t TokenAmountToString(const char *token, size_t tokenLen, size_t decimals,
                           uint64_t amount,
                           char *dst, size_t maxLen) {
    if (dst == NULL) {
        return 0U;
    }

    size_t prefixLen = tokenLen;

    if (prefixLen > 0U) {
        const char *const separator = ": ";
        const size_t separatorLen = 2U;

        MEMCOPY(dst, token, tokenLen);
        MEMCOPY(dst + tokenLen, separator, separatorLen);
        prefixLen += separatorLen;
    }

    if (decimals == 0U) {
        return prefixLen + UintToString(amount, dst + prefixLen, maxLen);
    }
    else {
        char buffer[25];
        const size_t len = UintToString(amount, buffer, maxLen);
        return prefixLen +
               adjustDecimals(buffer, len,
                              dst + prefixLen, maxLen,
                              decimals + 1U);
    }
}
