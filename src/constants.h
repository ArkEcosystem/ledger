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

////////////////////////////////////////////////////////////////////////////////

// General
#define MAX_RAW_OPERATION   300U
#define MAX_DISPLAY_BUFFER  255U

////////////////////////////////////////////////////////////////////////////////

// Hash
#define HASH_32_LENGTH  32U
#define HASH_64_LENGTH  64U

////////////////////////////////////////////////////////////////////////////////

// Identities
#define ADDRESS_LENGTH              34U
#define ADDRESS_HASH_LENGTH         21U
#define ADDRESS_MAX_BIP32_PATH      10U
#define PUBLICKEY_COMPRESSED_LENGTH 33U

////////////////////////////////////////////////////////////////////////////////

// Token
#define TOKEN_DECIMALS      8U
#define TOKEN_NAME          "ARK "
#define TOKEN_NAME_LENGTH   4U
#define TOKEN_NETWORK_BYTE  0x1e  // ARK Mainnet

////////////////////////////////////////////////////////////////////////////////

// Transactions
#define TRANSACTION_VERSION_LEGACY  0x00
#define TRANSACTION_VERSION_TYPE_1  0x01
#define TRANSACTION_VERSION_TYPE_2  0x02

#endif
