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

#include "transactions/assets/type_1.h"

#include <stdint.h>

#include <os.h>

#include "constants.h"

////////////////////////////////////////////////////////////////////////////////

// Second Signature Registration (Type 1) - 33 Bytes
//
// @param SecondSignatureRegistration *registration: The Second Signature Registration (Type 1) Asset.
// @param uint8_t *buffer: The serialized buffer beginning at the Assets offset.
// @param uint32_t length: The Asset Length.
//
// ---
// Internals:
//
// Second PublicKey - 33 Bytes:
// - os_memmove(registration->publicKey, buffer, PUBLICKEY_COMPRESSED_LENGTH);
//
// ---
StreamStatus deserializeSecondSignature(SecondSignatureRegistration *registration,
                                        const uint8_t *buffer,
                                        const uint32_t length) {
    if (length != 33U) {
        return USTREAM_FAULT;
    }

    os_memmove(registration->publicKey, buffer, PUBLICKEY_COMPRESSED_LENGTH);

    return USTREAM_FINISHED;
}

////////////////////////////////////////////////////////////////////////////////
