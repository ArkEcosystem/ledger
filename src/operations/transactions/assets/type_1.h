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

#ifndef ARK_OPERATIONS_TRANSACTION_TYPE_1_H
#define ARK_OPERATIONS_TRANSACTION_TYPE_1_H

#include <stdint.h>

#include "constants.h"

#include "operations/status.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct ss_registration_asset_t {
    uint8_t publicKey[PUBLICKEY_COMPRESSED_LENGTH];
} SecondSignatureRegistration;

////////////////////////////////////////////////////////////////////////////////

StreamStatus deserializeSecondSignature(SecondSignatureRegistration *registration,
                                        const uint8_t *buffer,
                                        const uint32_t length);

////////////////////////////////////////////////////////////////////////////////

#endif
