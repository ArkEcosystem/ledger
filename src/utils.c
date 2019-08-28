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

#include "utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "global.h"

//////////////////////////////////////////////////////////////////////

bool adjustDecimals(char *src,
                    uint32_t srcLength,
                    char *target,
                    uint32_t targetLength,
                    uint8_t decimals) {
    uint32_t startOffset;
    uint32_t lastZeroOffset = 0U;
    uint32_t offset = 0U;

    if ((srcLength == 1U) && (*src == '0')) {
        if (targetLength < 2U) {
            return false;
        }
        target[offset++] = '0';
        target[offset++] = '\0';
        return true;
    }

    if (srcLength <= decimals) {
        uint32_t delta = decimals - srcLength;
        if (targetLength < srcLength + 1U + 2U + delta) {
            return false;
        }

        target[offset++] = '0';
        target[offset++] = '.';
        for (uint32_t i = 0U; i < delta; i++) {
            target[offset++] = '0';
        }

        startOffset = offset;
        for (uint32_t i = 0U; i < srcLength; i++) {
            target[offset++] = src[i];
        }

        target[offset] = '\0';
    } else {
        uint32_t sourceOffset = 0U;
        uint32_t delta = srcLength - decimals;

        if (targetLength < srcLength + 1U + 1U) {
            return false;
        }

        while (offset < delta) {
            target[offset++] = src[sourceOffset++];
        }

        if (decimals != 0U) {
            target[offset++] = '.';
        }

        startOffset = offset;
        while (sourceOffset < srcLength) {
            target[offset++] = src[sourceOffset++];
        }

        target[offset] = '\0';
    }

    for (uint32_t i = startOffset; i < offset; i++) {
        if (target[i] == '0') {
            if (lastZeroOffset == 0U) {
                lastZeroOffset = i;
            }
        } else {
            lastZeroOffset = 0;
        }
    }

    if (lastZeroOffset != 0U) {
        target[lastZeroOffset] = '\0';
        if (target[lastZeroOffset - 1U] == '.') {
            target[lastZeroOffset - 1U] = '\0';
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////

unsigned short print_amount(uint64_t amount,
                            uint8_t *out,
                            uint32_t outlen,
                            const char *tokenName,
                            uint8_t tokenNameLength,
                            uint8_t decimals) {
    uint8_t tmp[UINT64_DEC_MAX_STRING_LENGTH + 1];
    uint8_t tmp2[sizeof(tmp) + tokenNameLength];
    uint32_t numDigits = 0UL, i;
    uint64_t base = 1ULL;

    while (base <= amount) {
        base *= BASE10_FLAG;
        numDigits++;
    }

    if (numDigits > sizeof(tmp) - 1U) {
        THROW(EXCEPTION);
    }

    base /= BASE10_FLAG;
    for (i = 0U; i < numDigits; i++) {
        tmp[i] = '0' + ((amount / base) % BASE10_FLAG);
        base /= BASE10_FLAG;
    }

    tmp[i] = '\0';
    os_memmove(tmp2, tokenName, tokenNameLength);
    adjustDecimals((char *)tmp, i,
                   (char *)tmp2 + UINT64_STRING_BUFFER_OFFSET,
                   sizeof(tmp2) - tokenNameLength,
                   decimals);

    if (sizeof(tmp2) < outlen - 1U) {
        os_memmove(out, tmp2, sizeof(tmp2));
    } else {
        out[0] = '\0';
    }

    return strlen((char *)out);
}

//////////////////////////////////////////////////////////////////////

#if 0
unsigned short ark_print_amount(uint64_t amount, uint8_t *out, uint32_t outlen) {
    uint64_t partInt;
    uint64_t partDecimal;
    partInt = amount / 1000000;
    partDecimal = amount - (partInt * 1000000);
    // TODO : handle properly
    if ((partInt > 0xFFFFFFFF) || (partDecimal > 0xFFFFFFFF)) {
        THROW(EXCEPTION);
    }
    if (partDecimal == 0) {
        snprintf(out, outlen, "%d", (uint32_t)partInt);
    }
    else {
        snprintf(out, outlen, "%d.%d", (uint32_t)partInt, (uint32_t)partDecimal);
    }
    return strlen(out);
}
#endif
