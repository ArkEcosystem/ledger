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

#ifndef ARK_TRANSACTIONS_DEFAULTS_OFFSETS_HPP
#define ARK_TRANSACTIONS_DEFAULTS_OFFSETS_HPP

#include <stddef.h>

////////////////////////////////////////////////////////////////////////////////
// v2 Transaction Offsets
static const size_t HEADER_OFFSET               = 0;
static const size_t VERSION_OFFSET              = 1;
static const size_t NETWORK_OFFSET              = 2;
static const size_t TYPE_OFFSET                 = 7;
static const size_t SENDER_PUBLICKEY_OFFSET     = 17;
static const size_t FEE_OFFSET                  = 50;
static const size_t VF_LEN_OFFSET               = 58;
static const size_t VF_OFFSET                   = 59;

////////////////////////////////////////////////////////////////////////////////
// v1 Transaction Offsets
static const size_t TYPE_OFFSET_V1                  = 3;
static const size_t SENDER_PUBLICKEY_OFFSET_V1      = 8;
static const size_t FEE_OFFSET_V1                   = 41;
static const size_t VF_LEN_OFFSET_V1                = 49;
static const size_t VF_OFFSET_V1                    = 50;

////////////////////////////////////////////////////////////////////////////////
// Legacy Ledger Transaction Asset Offset
static const size_t TYPE_OFFSET_LEGACY          = 0;
static const size_t RECIPIENT_OFFSET_LEGACY     = 38;
static const size_t AMOUNT_OFFSET_LEGACY        = 123;
static const size_t FEE_OFFSET_LEGACY           = 131;
static const size_t ASSET_OFFSET_LEGACY         = 139;

#endif
