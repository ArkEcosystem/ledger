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

#include "utils/hex.h"

#include <stddef.h>
#include <stdint.h>

#include <os.h>

////////////////////////////////////////////////////////////////////////////////

const uint8_t hexDigits[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f'
};

////////////////////////////////////////////////////////////////////////////////

// Convert Bytes to a Hex string.
// NULL terminator is added at (hexStringLen + 1)
void bytesToHex(char *dest, const uint8_t *src, size_t length) {
    while (length--) {
        *dest++ = hexDigits[(*src >> 0x04) & 0xF];
        *dest++ = hexDigits[*src & 0xF];
        ++src;
    }
    *dest = '\0';
}

////////////////////////////////////////////////////////////////////////////////
