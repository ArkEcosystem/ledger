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

#include "transactions/ux/vendorfield_ux.h"

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

#include "operations/transactions/transaction.h"

#include "utils/utils.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////

void setVendorField(const Transaction *transaction) {
    const char *const LABEL_VENDORFIELD = "VendorField";
    const size_t LABEL_VENDORFIELD_SIZE = 12;

    bytecpy((char *)displayCtx.extended_title,
            LABEL_VENDORFIELD,
            LABEL_VENDORFIELD_SIZE);

    bytecpy((char *)displayCtx.extended_text,
            (uint8_t *)transaction->vendorField,
            transaction->vendorFieldLength);
    displayCtx.extended_text[transaction->vendorFieldLength] = '\0';
}

////////////////////////////////////////////////////////////////////////////////
