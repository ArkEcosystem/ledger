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

#ifndef ARK_DISPLAY_CONTEXT_H
#define ARK_DISPLAY_CONTEXT_H

#include <stddef.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////

static const size_t DISPLAY_CTX_STEP_COUNT          = 5;
static const size_t DISPLAY_CTX_OP_SIZE             = 18;
static const size_t DISPLAY_CTX_TITLE_SIZE          = DISPLAY_CTX_OP_SIZE - 3;
static const size_t DISPLAY_CTX_TEXT_SIZE           = 68;
static const size_t DISPLAY_CTX_EXTENDED_TEXT_SIZE  = 256;

////////////////////////////////////////////////////////////////////////////////

// 704 Bytes (64-byte-aligned)
typedef struct display_context_t {
    uint8_t     operation   [DISPLAY_CTX_OP_SIZE];
    uint8_t     title       [DISPLAY_CTX_STEP_COUNT] [DISPLAY_CTX_TITLE_SIZE];
    uint8_t     text        [DISPLAY_CTX_STEP_COUNT] [DISPLAY_CTX_TEXT_SIZE];

    // For potentially large display texts.
    // Should always be set as last step in the UX display flow.
    //
    // 255 strLen + the null-terminator.
    // e.g. Message, VendorField, IPFS DAG
    uint8_t extended_title[DISPLAY_CTX_TITLE_SIZE];
    uint8_t extended_text[DISPLAY_CTX_EXTENDED_TEXT_SIZE];
} DisplayContext;

extern DisplayContext displayCtx;

////////////////////////////////////////////////////////////////////////////////

#endif
