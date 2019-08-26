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

#pragma once

#include "ux.h"

#include "os.h"
#include "cx.h"

#include "io.h"

//////////////////////////////////////////////////////////////////////

ux_state_t G_ux;
bolos_ux_params_t G_ux_params;

uint8_t title1[15], title2[15], title3[15];
uint8_t operation[15], var1[68], var2[68], var3[68];

void ui_idle(void);

//////////////////////////////////////////////////////////////////////

extern volatile char fullAddress[VOLATILE_ADDRESS_LENGTH];

//////////////////////////////////////////////////////////////////////

UX_STEP_NOCB(ux_idle_flow_1_step,
             pnn,
             { &C_icon, "Application", "is ready", }
);

UX_STEP_NOCB(ux_idle_flow_3_step,
             bn,
             { "Version", APPVERSION, }
);

UX_STEP_VALID(ux_idle_flow_4_step,
              pb,
              os_sched_exit(-1),
              { &C_icon_dashboard_x, "Quit", }
);

UX_FLOW(ux_idle_flow,
        &ux_idle_flow_1_step,
        &ux_idle_flow_3_step,
        &ux_idle_flow_4_step
);

//////////////////////////////////////////////////////////////////////

UX_STEP_NOCB(ux_display_public_flow_5_step,
             bnnn_paging,
             { .title = "Address", .text = (char *)fullAddress, }
);

UX_STEP_VALID(ux_display_public_flow_6_step,
              pb,
              io_seproxyhal_touch_address_ok(NULL),
              { &C_icon_validate_14, "Approve", }
);

UX_STEP_VALID(ux_display_public_flow_7_step,
              pb,
              io_seproxyhal_touch_address_cancel(NULL),
              { &C_icon_crossmark, "Reject", }
);

UX_FLOW(ux_display_public_flow,
        &ux_display_public_flow_5_step,
        &ux_display_public_flow_6_step,
        &ux_display_public_flow_7_step
);

//////////////////////////////////////////////////////////////////////

UX_STEP_NOCB(ux_confirm_full_flow_1_step,
             pnn,
             { &C_icon_eye, "Review operation:", operation, }
);
UX_STEP_NOCB(ux_confirm_full_flow_2_step,
             bnnn_paging,
             { .title = title1, .text = var1 }
);

UX_STEP_NOCB(ux_confirm_full_flow_3_step,
             bnnn_paging,
             { .title = title2, .text = var2, }
);

UX_STEP_NOCB(ux_confirm_full_flow_4_step,
             bnnn_paging,
             { .title = title3, .text = var3, }
);

UX_STEP_VALID(ux_confirm_full_flow_5_step,
              pb,
              io_seproxyhal_touch_tx_ok(NULL),
              { &C_icon_validate_14, "Accept", }
);

UX_STEP_VALID(ux_confirm_full_flow_6_step,
              pb,
              io_seproxyhal_touch_tx_cancel(NULL),
              { &C_icon_crossmark, "Reject", }
);

UX_FLOW(ux_confirm_full_flow_2var,
        &ux_confirm_full_flow_1_step,
        &ux_confirm_full_flow_2_step,
        &ux_confirm_full_flow_3_step,
        &ux_confirm_full_flow_5_step,
        &ux_confirm_full_flow_6_step
);

UX_FLOW(ux_confirm_full_flow_3var,
        &ux_confirm_full_flow_1_step,
        &ux_confirm_full_flow_2_step,
        &ux_confirm_full_flow_3_step,
        &ux_confirm_full_flow_4_step,
        &ux_confirm_full_flow_5_step,
        &ux_confirm_full_flow_6_step
);
