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

#include "os.h"

#if defined(TARGET_NANOS)

#include "nanos/buttons.h"

#include "io.h"

unsigned int ui_address_nanos_button(unsigned int button_mask,
                                     unsigned int button_mask_counter) {
    switch (button_mask) {
        case BUTTON_EVT_RELEASED | BUTTON_LEFT:  // CANCEL
            io_seproxyhal_touch_address_cancel(NULL);
            break;
        case BUTTON_EVT_RELEASED | BUTTON_RIGHT:  // OK
            io_seproxyhal_touch_address_ok(NULL);
            break;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////

unsigned int ui_approval_nanos_button(unsigned int button_mask,
                                      unsigned int button_mask_counter) {
    switch (button_mask) {
        case BUTTON_EVT_RELEASED | BUTTON_LEFT:  // CANCEL
            io_seproxyhal_touch_tx_cancel(NULL);
            break;
        case BUTTON_EVT_RELEASED | BUTTON_RIGHT:  // OK
            io_seproxyhal_touch_tx_ok(NULL);
            break;
    }
    return 0;
}

#endif // TARGET_NANOS
