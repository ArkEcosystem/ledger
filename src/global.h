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

#pragma once

//////////////////////////////////////////////////////////////////////

// APDU
#define OFFSET_P1       2U
#define OFFSET_P2       3U
#define OFFSET_LC       4U
#define OFFSET_CDATA    5U
// Choosing P1_FIRST to be 0x00 has the problem that it is not possible to distinguish
// Case1 from Case2:
//
// Case1 (a single chunk): p1==P1_LAST
// Case2 (a few chunks): p1==P1_FIRST, p1==P1_MORE, p1==P1_MORE, ..., p1==P1_LAST
//
// When we receive a chunk with p1==P1_LAST we can't tell if this is the first chunk
// from Case1 or the last chunk from Case2. Because we append all chunks in a buffer,
// we need to be able to tell if this is the first chunk or not - so that we know
// whether to put the chunk in the beginning of the buffer or append at position
// rawMessageLength.
// To workaround this issue we reset rawMessageLength to 0 in
// ui_idle() and always append chunks at rawMessageLength.
#define P1_CONFIRM      0x01
#define P1_NON_CONFIRM  0x00
#define P2_NO_CHAINCODE 0x00
#define P2_CHAINCODE    0x01

#define P1_FIRST        0x00
#define P1_MORE         0x01
#define P1_LAST         0x80
#define P2_SECP256K1    0x40
#define P2_ED25519      0x80

//////////////////////////////////////////////////////////////////////

// Hash
#define HASH_32_LENGTH  32U
#define HASH_64_LENGTH  64U

//////////////////////////////////////////////////////////////////////

// Identities
#define VOLATILE_ADDRESS_LENGTH     68U
#define ADDRESS_HASH_LENGTH         21U
#define ADDRESS_MAX_BIP32_PATH      10U
#define PUBLICKEY_COMPRESSED_LENGTH 33U

//////////////////////////////////////////////////////////////////////

// Token
#define TOKEN_DECIMALS      8U
#define TOKEN_NAME          "ARK "
#define TOKEN_NAME_LENGTH   5U
#define TOKEN_NETWORK_BYTE  0x1e // ARK Mainnet

//////////////////////////////////////////////////////////////////////

// Transactions
#define OPERATION_TYPE_TRANSFER 0x00
#define OPERATION_TYPE_VOTE     0x03
#define MAX_RAW_TX              300U

