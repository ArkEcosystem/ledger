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

#include "utils/print.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <os.h>

#include "constants.h"

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
    uint8_t tmp[21];
    uint8_t tmp2[sizeof(tmp) + tokenNameSize];
    size_t numDigits = 0, i;
    uint64_t base = 1ULL;

    while (base <= amount) {
        base *= 0x0A;
        numDigits++;
    }

    if (numDigits > sizeof(tmp) - 1) {
        THROW(EXCEPTION);
    }

    base /= 0x0A;
    for (i = 0; i < numDigits; i++) {
        tmp[i] = '0' + ((amount / base) % 0x0A);
        base /= 0x0A;
    }

    tmp[i] = '\0';

    if (tokenNameSize > 0) {
        os_memmove(tmp2, tokenName, tokenNameSize);
    }

    adjustDecimals((char *)tmp, i,
                   (char *)tmp2 + tokenNameSize,
                   sizeof(tmp2) - tokenNameSize,
                   decimals);


    if (sizeof(tmp2) < outSize - 1) {
        os_memmove(out, tmp2, sizeof(tmp2));
    } else {
        out[0] = '\0';
    }

    return sizeof(tmp2);
}

////////////////////////////////////////////////////////////////////////////////
