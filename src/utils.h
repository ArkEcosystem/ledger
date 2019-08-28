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

#pragma once

#include <os.h>

//////////////////////////////////////////////////////////////////////

#define BASE10_FLAG                     10U
#define UINT64_DEC_MAX_STRING_LENGTH    20U
#define UINT64_STRING_BUFFER_OFFSET     4U

//////////////////////////////////////////////////////////////////////

unsigned short print_amount(uint64_t amount,
                            uint8_t *out,
                            uint32_t outlen,
                            const char *tokenName,
                            uint8_t tokenNameLength,
                            uint8_t decimals);
