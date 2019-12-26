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

#ifndef ARK_CRYPTO_HASHING_H
#define ARK_CRYPTO_HASHING_H

#include <stddef.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////

void hash160(uint8_t *in, size_t inSize, uint8_t *out);

void hash256(uint8_t *in, size_t inSize, uint8_t *out);

void doubleHash256(uint8_t *in, size_t inSize, uint8_t *out);

////////////////////////////////////////////////////////////////////////////////

#endif
