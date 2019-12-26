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

#include "display/display.h"

#if defined(TARGET_NANOS) || defined(TARGET_NANOX)

#include <stdint.h>

#include <cx.h>
#include <ux.h>
#include <os_io_seproxyhal.h>

#include "glyphs.h"

#include "approval.h"

#include "display/context.h"

////////////////////////////////////////////////////////////////////////////////

ux_state_t G_ux;

////////////////////////////////////////////////////////////////////////////////

// Main App Menu
UX_STEP_NOCB(ux_idle_flow_1_step,
             pnn,
             { &C_icon, "Application", "is ready", });

UX_STEP_NOCB(ux_idle_flow_3_step,
             bn,
             { "Version", APPVERSION, });

UX_STEP_VALID(ux_idle_flow_4_step,
              pb,
              os_sched_exit(-1),
              { &C_icon_dashboard_x, "Quit", });

// 3-Screen Main Menu UX Flow
UX_FLOW(ux_idle_flow,
        &ux_idle_flow_1_step,
        &ux_idle_flow_3_step,
        &ux_idle_flow_4_step);

////////////////////////////////////////////////////////////////////////////////

// Operation UX Flow Template
UX_STEP_NOCB(ux_confirm_full_flow_1_step,
             pnn,
             { &C_icon_eye, "Operation:",
               (const char *const)displayCtx.operation, });

UX_STEP_NOCB(ux_confirm_full_flow_2_step,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[0],
               .text = (const char *const)displayCtx.var[0] });

UX_STEP_NOCB(ux_confirm_full_flow_3_step,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[1],
               .text = (const char *const)displayCtx.var[1], });

UX_STEP_NOCB(ux_confirm_full_flow_4_step,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[2],
               .text = (const char *const)displayCtx.var[2], });

UX_STEP_NOCB(ux_confirm_full_flow_5_step,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[3],
               .text = (const char *const)displayCtx.var[3], });

UX_STEP_NOCB(ux_confirm_full_flow_6_step,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[4],
               .text = (const char *const)displayCtx.var[4], });

UX_STEP_VALID(ux_confirm_full_flow_7_step,
              pb,
              ioApprove(NULL),
              { &C_icon_validate_14, "Accept", });

UX_STEP_VALID(ux_confirm_full_flow_8_step,
              pb,
              ioCancel(NULL),
              { &C_icon_crossmark, "Reject", });

////////////////////////////////////////////////////////////////////////////////

// 1-variable UX Flow
UX_FLOW(ux_confirm_full_flow_1var,
        &ux_confirm_full_flow_1_step,
        &ux_confirm_full_flow_2_step,
        &ux_confirm_full_flow_7_step,
        &ux_confirm_full_flow_8_step);

// 2-variable UX Flow
UX_FLOW(ux_confirm_full_flow_2var,
        &ux_confirm_full_flow_1_step,
        &ux_confirm_full_flow_2_step,
        &ux_confirm_full_flow_3_step,
        &ux_confirm_full_flow_7_step,
        &ux_confirm_full_flow_8_step);

// 3-variable UX Flow
UX_FLOW(ux_confirm_full_flow_3var,
        &ux_confirm_full_flow_1_step,
        &ux_confirm_full_flow_2_step,
        &ux_confirm_full_flow_3_step,
        &ux_confirm_full_flow_4_step,
        &ux_confirm_full_flow_7_step,
        &ux_confirm_full_flow_8_step);

// 4-variable UX Flow
UX_FLOW(ux_confirm_full_flow_4var,
        &ux_confirm_full_flow_1_step,
        &ux_confirm_full_flow_2_step,
        &ux_confirm_full_flow_3_step,
        &ux_confirm_full_flow_4_step,
        &ux_confirm_full_flow_5_step,
        &ux_confirm_full_flow_7_step,
        &ux_confirm_full_flow_8_step);

// 5-variable UX Flow
UX_FLOW(ux_confirm_full_flow_5var,
        &ux_confirm_full_flow_1_step,
        &ux_confirm_full_flow_2_step,
        &ux_confirm_full_flow_3_step,
        &ux_confirm_full_flow_4_step,
        &ux_confirm_full_flow_5_step,
        &ux_confirm_full_flow_6_step,
        &ux_confirm_full_flow_7_step,
        &ux_confirm_full_flow_8_step);

////////////////////////////////////////////////////////////////////////////////

// Set the number of Items (steps) to be displayed.
void setDisplaySteps(uint8_t steps) {
    switch(steps) {
        case 1: ux_flow_init(0U, ux_confirm_full_flow_1var, NULL); break;
        case 2: ux_flow_init(0U, ux_confirm_full_flow_2var, NULL); break;
        case 3: ux_flow_init(0U, ux_confirm_full_flow_3var, NULL); break;
        case 4: ux_flow_init(0U, ux_confirm_full_flow_4var, NULL); break;
        case 5: ux_flow_init(0U, ux_confirm_full_flow_5var, NULL); break;
        default: break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ui_idle(void) {
    // reserve a display stack slot if none yet
    if (G_ux.stack_count == 0) {
        ux_stack_push();
    }
    ux_flow_init(0, ux_idle_flow, NULL);
}

////////////////////////////////////////////////////////////////////////////////

#endif  // defined(TARGET_NANOS) || defined(TARGET_NANOX)
