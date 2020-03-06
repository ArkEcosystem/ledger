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

#include "utils/print.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
static size_t adjustDecimals(const char *src, size_t srcSize,
                             char *target, size_t targetSize,
                             size_t decimals) {
    size_t startOffset;
    size_t lastZeroOffset = 0;
    size_t offset = 0;

    if ((srcSize == 1) && (*src == '0')) {
        if (targetSize < 2) {
            return 0;
        }
        target[offset++] = '0';
        target[offset++] = '\0';
        return offset;
    }

    if (srcSize <= decimals) {
        size_t delta = decimals - srcSize;
        if (targetSize < srcSize + 1 + 2 + delta) {
            return offset;
        }

        target[offset++] = '0';
        if (decimals > 0) {
          target[offset++] = '.';
        }
        
        for (size_t i = 0; i < delta; i++) {
            target[offset++] = '0';
        }

        startOffset = offset;
        for (size_t i = 0; i < srcSize; i++) {
            target[offset++] = src[i];
        }

        target[offset] = '\0';
    }
    else {
        size_t sourceOffset = 0;
        size_t delta = srcSize - decimals;

        if (targetSize < srcSize + 1 + 1) {
            return offset;
        }

        while (offset < delta) {
            target[offset++] = src[sourceOffset++];
        }

        if (decimals != 0) {
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
            if (lastZeroOffset == 0) {
                lastZeroOffset = i;
            }
        }
        else {
            lastZeroOffset = 0;
        }
    }

    if (lastZeroOffset != 0) {
        target[lastZeroOffset] = '\0';
        if (target[lastZeroOffset - 1] == '.') {
            target[lastZeroOffset - 1] = '\0';
        }
    }

    return offset + lastZeroOffset;
}

////////////////////////////////////////////////////////////////////////////////
size_t UintToString(uint64_t value, char *dst, size_t maxLen) {
    if (dst == NULL || maxLen < 2) {
        return 0;
    }

    if (value == 0) {
        dst[0] = '0';
        dst[1] = '\0';
        return 2;
    }

    uint64_t base = 1;
    size_t n = 0;
    size_t i = 0;

    const size_t maxN = 20;

    // count how many characters are needed
    while (base <= value && n <= UINT64_MAX_STRING_SIZE) {
        base *= UINT64_BASE_10;
        n++;
    }

    if (n > maxLen - 1) {
        dst[0] = '\0';
        return 0;
    }

    base /= UINT64_BASE_10;
    while (i < n) {
        dst[i++] = '0' + ((value / base) % UINT64_BASE_10);
        base /= UINT64_BASE_10;
    }

    dst[i] = '\0';

    return n + 1;
}

////////////////////////////////////////////////////////////////////////////////
size_t TokenAmountToString(uint64_t amount,
                           char *dst, size_t maxLen,
                           const char *tokenName, size_t tokenNameSize,
                           size_t decimals) {
    if (dst == NULL) {
        return 0;
    }

    if (tokenNameSize > 0) {
        bytecpy(dst, tokenName, tokenNameSize);
    }

    if (decimals == 0) {
        return tokenNameSize + UintToString(amount, dst + tokenNameSize, maxLen);
    }
    else {
        char buffer[TOKEN_AMOUNT_MAX_CHARS];
        const size_t len = UintToString(amount, buffer, maxLen);
        return tokenNameSize +
               adjustDecimals(buffer, len,
                              dst + tokenNameSize, maxLen,
                              decimals + 1);
    }
}
