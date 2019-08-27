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

#include <os.h>
#include <cx.h>
#include <ux.h>
#include <os_io_seproxyhal.h>

//////////////////////////////////////////////////////////////////////

ux_state_t G_ux;
bolos_ux_params_t G_ux_params;

//////////////////////////////////////////////////////////////////////

uint8_t title1[15], title2[15], title3[15];
uint8_t operation[15], var1[68], var2[68], var3[68];

//////////////////////////////////////////////////////////////////////

void ui_idle(void);
