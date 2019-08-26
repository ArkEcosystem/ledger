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

#include "menu.h"

#include "glyphs.h"

#include "os_io_seproxyhal.h"

#include "global.h"

//////////////////////////////////////////////////////////////////////

extern volatile char fullAddress[VOLATILE_ADDRESS_LENGTH];
extern volatile char fullAmount[50];

//////////////////////////////////////////////////////////////////////

extern const ux_menu_entry_t menu_about[3];

const ux_menu_entry_t menu_main[] = {
    { NULL, NULL, 0, &C_icon_ark, "Use wallet to", "view accounts", PUBLICKEY_COMPRESSED_LENGTH, 12 },
    { menu_about, NULL, 0, NULL, "About", NULL, 0, 0 },
    { NULL, os_sched_exit, 0, &C_icon_dashboard, "Quit app", NULL, 50, 29 },
      UX_MENU_END };

const ux_menu_entry_t menu_about[] = {
    { NULL, NULL, 0, NULL, "Version", APPVERSION, 0, 0 },
    { menu_main, NULL, 2, &C_icon_back, "Back", NULL, 61, 40 },
      UX_MENU_END};

//////////////////////////////////////////////////////////////////////

const char * const ui_approval_transfer[][2] = {
    { "Operation", (const char *const)fullAmount },
    { "To", (const char *const)fullAddress },
    { "Amount", (const char *const)fullAmount },
    { "Fees", (const char *const)fullAmount },
};

const char * const ui_approval_vote1[][2] = {
    { "Operation", (const char *const)fullAmount },
    { "Vote", (const char *const)fullAddress },
    { "Fees", (const char *const)fullAmount },
};

const char * const ui_approval_vote2[][2] = {
    { "Operation", (const char *const)fullAmount },
    { "Vote 1", (const char *const)fullAddress },
    { "Vote 2", (const char *const)fullAddress },
    { "Fees", (const char *const)fullAmount },
};

//////////////////////////////////////////////////////////////////////

void ui_idle(void) {
  UX_MENU_DISPLAY(0, menu_main, NULL);
}

#endif  // TARGET_NANOS
