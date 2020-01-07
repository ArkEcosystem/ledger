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

#include <stdbool.h>
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
UX_STEP_NOCB(ux_idle_application_ready,
             pnn,
             { &C_icon, "Application", "is ready", });

UX_STEP_NOCB(ux_idle_application_version,
             bn,
             { "Version", APPVERSION, });

UX_STEP_VALID(ux_idle_application_quit,
              pb,
              os_sched_exit(-1),
              { &C_icon_dashboard_x, "Quit", });

// 3-Screen Main Menu UX Flow
UX_FLOW(ux_idle_flow,
        &ux_idle_application_ready,
        &ux_idle_application_version,
        &ux_idle_application_quit);

////////////////////////////////////////////////////////////////////////////////

// Operation UX Flow Template
UX_STEP_NOCB(ux_screen_operation,
             pnn,
             { &C_icon_eye, "Operation:",
               (const char *const)displayCtx.operation, });

UX_STEP_NOCB(ux_screen_item_1,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[0],
               .text = (const char *const)displayCtx.text[0] });

UX_STEP_NOCB(ux_screen_item_2,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[1],
               .text = (const char *const)displayCtx.text[1], });

UX_STEP_NOCB(ux_screen_item_3,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[2],
               .text = (const char *const)displayCtx.text[2], });

UX_STEP_NOCB(ux_screen_item_4,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[3],
               .text = (const char *const)displayCtx.text[3], });

UX_STEP_NOCB(ux_screen_item_5,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[4],
               .text = (const char *const)displayCtx.text[4], });

UX_STEP_NOCB(ux_screen_item_extended,
             bnnn_paging,
             { .title = (const char *const)displayCtx.title[DISPLAY_CTX_EXTENDED_TITLE_INDEX],
               .text = (const char *const)displayCtx.extended_text, });

UX_STEP_VALID(ux_screen_accept,
              pb,
              ioApprove(NULL),
              { &C_icon_validate_14, "Accept", });

UX_STEP_VALID(ux_screen_reject,
              pb,
              ioCancel(NULL),
              { &C_icon_crossmark, "Reject", });

////////////////////////////////////////////////////////////////////////////////

// 1-variable UX Flow
UX_FLOW(ux_flow_1_screen,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_accept, &ux_screen_reject);

// 2-variable UX Flow
UX_FLOW(ux_flow_2_screen,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_2,
        &ux_screen_accept, &ux_screen_reject);

// 3-variable UX Flow
UX_FLOW(ux_flow_3_screen,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_2,
        &ux_screen_item_3,
        &ux_screen_accept, &ux_screen_reject);

// 4-variable UX Flow
UX_FLOW(ux_flow_4_screen,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_2,
        &ux_screen_item_3,
        &ux_screen_item_4,
        &ux_screen_accept, &ux_screen_reject);

// 5-variable UX Flow
UX_FLOW(ux_flow_5_screen,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_2,
        &ux_screen_item_3,
        &ux_screen_item_4,
        &ux_screen_item_5,
        &ux_screen_accept, &ux_screen_reject);

// 1-variable UX Flow, Extended last step
UX_FLOW(ux_flow_1_screen_extended,
        &ux_screen_operation,
        &ux_screen_item_extended,
        &ux_screen_accept, &ux_screen_reject);

// 2-variable UX Flow, Extended last step
UX_FLOW(ux_flow_2_screen_extended,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_extended,
        &ux_screen_accept, &ux_screen_reject);

// 3-variable UX Flow, Extended last step
UX_FLOW(ux_flow_3_screen_extended,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_2,
        &ux_screen_item_extended,
        &ux_screen_accept, &ux_screen_reject);

// 4-variable UX Flow, Extended last step
UX_FLOW(ux_flow_4_screen_extended,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_2,
        &ux_screen_item_3,
        &ux_screen_item_extended,
        &ux_screen_accept, &ux_screen_reject);

// 5-variable UX Flow, Extended last step
UX_FLOW(ux_flow_5_screen_extended,
        &ux_screen_operation,
        &ux_screen_item_1,
        &ux_screen_item_2,
        &ux_screen_item_3,
        &ux_screen_item_4,
        &ux_screen_item_extended,
        &ux_screen_accept, &ux_screen_reject);

////////////////////////////////////////////////////////////////////////////////

void setDisplaySteps(uint8_t steps, bool isExtended) {
    switch(steps) {
        case 1: isExtended ? ux_flow_init(0U, ux_flow_1_screen_extended, NULL)
                           : ux_flow_init(0U, ux_flow_1_screen, NULL); break;

        case 2: isExtended ? ux_flow_init(0U, ux_flow_2_screen_extended, NULL)
                           : ux_flow_init(0U, ux_flow_2_screen, NULL); break;

        case 3: isExtended ? ux_flow_init(0U, ux_flow_3_screen_extended, NULL)
                           : ux_flow_init(0U, ux_flow_3_screen, NULL); break;

        case 4: isExtended ? ux_flow_init(0U, ux_flow_4_screen_extended, NULL)
                           : ux_flow_init(0U, ux_flow_4_screen, NULL); break;

        case 5: isExtended ? ux_flow_init(0U, ux_flow_5_screen_extended, NULL)
                           : ux_flow_init(0U, ux_flow_5_screen, NULL); break;

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
