/*******************************************************************************
*   Ark Wallet
*   (c) 2017 Ledger
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

#ifndef ARK_CONSTANTS_H
#define ARK_CONSTANTS_H

#include <stddef.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////

// General
static const size_t MAX_RAW_OPERATION       = 704;      // 64-byte-aligned
static const size_t MAX_TEXT_LEN            = 255;

////////////////////////////////////////////////////////////////////////////////

// Hash
static const size_t HASH_20_LEN     = 20;
static const size_t HASH_32_LEN     = 32;
static const size_t HASH_64_LEN     = 64;

////////////////////////////////////////////////////////////////////////////////

// Identities
static const size_t ADDRESS_LEN                     = 34;
static const size_t ADDRESS_HASH_LEN                = 21;
static const size_t ADDRESS_MAX_BIP32_PATH          = 10;
static const size_t PUBLICKEY_COMPRESSED_LEN        = 33;

////////////////////////////////////////////////////////////////////////////////

// Labels
static const char *const LABEL_AMOUNT       = "Amount";
static const size_t LABEL_AMOUNT_SIZE       = 7;

static const char *const LABEL_EXPIRATION       = "Expiration";
static const size_t LABEL_EXPIRATION_SIZE       = 11;

static const char *const LABEL_FEE      = "Fee";
static const size_t LABEL_FEE_SIZE      = 4;

static const char *const LABEL_TO       = "To";
static const size_t LABEL_TO_SIZE       = 3;

////////////////////////////////////////////////////////////////////////////////

// Token
static const size_t TOKEN_DECIMALS          = 8;
static const char *const TOKEN_NAME         = "ARK ";
static const size_t TOKEN_NAME_SIZE         = 4;        // sizeof("ARK ") - 1
static const uint8_t TOKEN_NETWORK_BYTE     = 0x1e;     // ARK Mainnet

////////////////////////////////////////////////////////////////////////////////

// Transactions
static const uint8_t TRANSACTION_HEADER             = 0xFF;

static const uint8_t TRANSACTION_VERSION_LEGACY     = 0;
static const uint8_t TRANSACTION_VERSION_TYPE_1     = 1;
static const uint8_t TRANSACTION_VERSION_TYPE_2     = 2;

#endif
