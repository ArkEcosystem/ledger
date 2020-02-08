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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////
static bool adjustDecimals(char *src,
                           size_t srcSize,
                           char *target,
                           size_t targetSize,
                           uint8_t decimals) {
    size_t startOffset;
    size_t lastZeroOffset = 0;
    size_t offset = 0;

    if ((srcSize == 1) && (*src == '0')) {
        if (targetSize < 2) {
            return false;
        }
        target[offset++] = '0';
        target[offset++] = '\0';
        return true;
    }

    if (srcSize <= decimals) {
        size_t delta = decimals - srcSize;
        if (targetSize < srcSize + 1 + 2 + delta) {
            return false;
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
            return false;
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

    return true;
}

////////////////////////////////////////////////////////////////////////////////
size_t printAmount(uint64_t amount,
                   uint8_t *out,
                   size_t outSize,
                   const char *tokenName,
                   size_t tokenNameSize,
                   uint8_t decimals) {
    uint8_t tmp[ADDRESS_HASH_LEN];
    uint8_t tmp2[sizeof(tmp) + tokenNameSize];
    size_t numDigits = 0, i;
    uint64_t base = 1ULL;

    while (base <= amount) {
        base *= 0x0A;
        numDigits++;
    }

    if (numDigits > sizeof(tmp) - 1) {
        return 0;
    }

    base /= 0x0A;
    for (i = 0; i < numDigits; i++) {
        tmp[i] = '0' + ((amount / base) % 0x0A);
        base /= 0x0A;
    }

    tmp[i] = '\0';

    if (tokenNameSize > 0) {
        bytecpy(tmp2, tokenName, tokenNameSize);
    }

    adjustDecimals((char *)tmp, i,
                   (char *)tmp2 + tokenNameSize,
                   sizeof(tmp2) - tokenNameSize,
                   decimals);

    if (sizeof(tmp2) < outSize - 1) {
        bytecpy(out, tmp2, sizeof(tmp2));
    } else {
        out[0] = '\0';
    }

    return sizeof(tmp2);
}
