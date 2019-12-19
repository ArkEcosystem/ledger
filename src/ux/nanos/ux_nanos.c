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

#include <os.h>

#if defined(TARGET_NANOS)

#include "ux/nanos/ux_nanos.h"

#include <ux.h>
#include <os_io_seproxyhal.h>

#include "glyphs.h"

#include "approval.h"

#include "transactions/transaction.h"
#include "ux/display_context.h"

#include "utils/utils.h"

////////////////////////////////////////////////////////////////////////////////

ux_state_t ux;
bagl_element_t tmp_element;

////////////////////////////////////////////////////////////////////////////////

extern unsigned int ux_step;
extern unsigned int ux_step_count;

////////////////////////////////////////////////////////////////////////////////

const ux_menu_entry_t menu_main[4] = {
    { NULL, NULL, 0, &C_icon_ark,
      "Use wallet to", "view accounts", 33U, 12U },
    { NULL, NULL, 0U, NULL,
      "Version", APPVERSION, 0U, 0U },
    { NULL, (ux_menu_callback_t)os_sched_exit, 0U, &C_icon_dashboard,
      "Quit app", NULL, 50U, 29U },
      UX_MENU_END };

////////////////////////////////////////////////////////////////////////////////

const char *const menu_flow[6][2] = {
    { "type",                                   (const char *const)displayCtx.operation },
    { (const char *const)displayCtx.title[0],   (const char *const)displayCtx.var[0] },
    { (const char *const)displayCtx.title[1],   (const char *const)displayCtx.var[1] },
    { (const char *const)displayCtx.title[2],   (const char *const)displayCtx.var[2] },
    { (const char *const)displayCtx.title[3],   (const char *const)displayCtx.var[3] },
    { (const char *const)displayCtx.title[4],   (const char *const)displayCtx.var[4] },
};

////////////////////////////////////////////////////////////////////////////////

const bagl_element_t operation_menu[7] = {
    { { BAGL_RECTANGLE, // type
            0x00,       // userid
            0,          // x
            0,          // y
            128,        // width
            32,         //height
            0,          // stroke
            0,          // radius
            BAGL_FILL,  // fill
            0x000000,   // foreground color
            0xFFFFFF,   // background color
            0,          // font Id
            0 },        // iconId
        NULL },         // txt,

    { { BAGL_ICON, 0x00, 3, 12, 7, 7, 0, 0, 0, 0xFFFFFF, 0x000000, 0,
        BAGL_GLYPH_ICON_CROSS },
        NULL },

    { { BAGL_ICON, 0x00, 117, 13, 8, 6, 0, 0, 0, 0xFFFFFF, 0x000000, 0,
        BAGL_GLYPH_ICON_CHECK },
        NULL },

    { { BAGL_LABELINE, 0x01, 0, 12, 128, 32, 0, 0, 0, 0xFFFFFF, 0,
        BAGL_FONT_OPEN_SANS_EXTRABOLD_11px | BAGL_FONT_ALIGNMENT_CENTER, 0 },
        "Review" },

    { { BAGL_LABELINE, 0x01, 0, 26, 128, 32, 0, 0, 0, 0xFFFFFF, 0x000000,
        BAGL_FONT_OPEN_SANS_EXTRABOLD_11px | BAGL_FONT_ALIGNMENT_CENTER, 0 },
        "operation:" },

    { { BAGL_LABELINE, 0x02, 0x00, 12, 128, 32, 0, 0, 0, 0xFFFFFF, 0x000000,
        BAGL_FONT_OPEN_SANS_REGULAR_11px | BAGL_FONT_ALIGNMENT_CENTER, 0 },
        NULL },

    { { BAGL_LABELINE, 0x12, 23, 26, 84, 12, 0x80 | 10, 0, 0, 0xFFFFFF, 0x000000,
        BAGL_FONT_OPEN_SANS_EXTRABOLD_11px | BAGL_FONT_ALIGNMENT_CENTER, 26 },
        NULL },
};

////////////////////////////////////////////////////////////////////////////////

unsigned int operation_menu_button(unsigned int button_mask,
                                   unsigned int button_mask_counter) {
    switch (button_mask) {
        case BUTTON_EVT_RELEASED | BUTTON_LEFT:  // CANCEL
            ioCancel(NULL);
            break;
        case BUTTON_EVT_RELEASED | BUTTON_RIGHT:  // CONFIRM
            ioApprove(NULL);
            break;
    }

    // clear the display variables.
    explicit_bzero(&displayCtx, sizeof(displayCtx));

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

// Handles the Operation Type
const bagl_element_t *operation_prepro(const bagl_element_t *element) {
    unsigned int display = 1;
    if (element->component.userid <= 0) {
        return element;
    }

    display = (ux_step == element->component.userid - 1U) ||
            (element->component.userid >= 0x02 && ux_step >= 1U);

    if (display) {

        switch (element->component.userid) {
            case 0x01: UX_CALLBACK_SET_INTERVAL(2000); break;

            case 0x02:

            case 0x12:
                bytecpy(&tmp_element, element, sizeof(bagl_element_t));

                display = ux_step - 1U;
                tmp_element.text =
                menu_flow[display][(element->component.userid) >> 4];

                UX_CALLBACK_SET_INTERVAL(MAX(
                        3000UL,
                        1000UL +
                        bagl_label_roundtrip_duration_ms(&tmp_element, 7U)));

            return &tmp_element;
        }
    }

    return display ? element : NULL;
}

////////////////////////////////////////////////////////////////////////////////

// Set the number of Items (steps) to be displayed.
void setDisplaySteps(uint8_t steps) {
    ux_step = 0;
    ux_step_count = steps + 2U;
    UX_DISPLAY(operation_menu, (bagl_element_callback_t)operation_prepro);
}

////////////////////////////////////////////////////////////////////////////////

void ui_idle(void) {
    UX_MENU_DISPLAY(0, menu_main, NULL);
}

////////////////////////////////////////////////////////////////////////////////

#endif  // TARGET_NANOS
