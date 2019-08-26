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

#include "nanos/screen.h"

#include "glyphs.h"

#include "os_io_seproxyhal.h"

#include "global.h"

//////////////////////////////////////////////////////////////////////

extern volatile char fullAddress[VOLATILE_ADDRESS_LENGTH];

//////////////////////////////////////////////////////////////////////

const bagl_element_t ui_address_nanos[] = {
    _B_CLEAN_SCREEN,

    _B_ICON_CROSS(_B_FIELD_USERID(0x00)),
    _B_ICON_CHECK(_B_FIELD_USERID(0x00)),

    _B_LABELINE_UPPER_BOLD(_B_FIELD_USERID(0x01), _B_FIELD_TEXT("Confirm")),
    _B_LABELINE_LOWER_BOLD(_B_FIELD_USERID(0x01), _B_FIELD_TEXT("address")),

    _B_LABELINE_UPPER_REGULAR(_B_FIELD_USERID(0x02), _B_FIELD_TEXT("Address")),
    _B_LABELINE(
        _B_FIELD_USERID(0x02),
        _B_FIELD_X(23),
        _B_FIELD_Y(26),
        _B_FIELD_WIDTH(82),
        _B_FIELD_HEIGHT(12),
        _B_FIELD_STROKE(BAGL_STROKE_FLAG_ONESHOT | _B_SCROLL_WAIT_MS(1000)),
        _B_FIELD_FONTID(BAGL_FONT_OPEN_SANS_EXTRABOLD_11px | BAGL_FONT_ALIGNMENT_CENTER),
        _B_FIELD_ICONID(BAGL_GLYPH_ICON_EYE_BADGE),
        _B_FIELD_TEXT((char *)fullAddress)
    ),
};

//////////////////////////////////////////////////////////////////////

const bagl_element_t ui_approval_nanos[] = {
    _B_CLEAN_SCREEN,

    _B_ICON_CROSS(_B_FIELD_USERID(0x00)),

    _B_ICON_CHECK(_B_FIELD_USERID(0x00)),

    _B_LABELINE_UPPER_BOLD(_B_FIELD_USERID(0x01), _B_FIELD_TEXT("Confirm")),
    _B_LABELINE_LOWER_BOLD(_B_FIELD_USERID(0x01), _B_FIELD_TEXT("transaction")),

    _B_LABELINE_UPPER_REGULAR(_B_FIELD_USERID(0x02), _B_FIELD_TEXT(NULL)),
    _B_LABELINE(
      _B_FIELD_USERID(0x12),
      _B_FIELD_X(23),
      _B_FIELD_Y(26),
      _B_FIELD_WIDTH(84),
      _B_FIELD_HEIGHT(12),
      _B_FIELD_STROKE(BAGL_STROKE_FLAG_ONESHOT | _B_SCROLL_WAIT_MS(1000)),
      _B_FIELD_FONTID(BAGL_FONT_OPEN_SANS_REGULAR_11px | BAGL_FONT_ALIGNMENT_CENTER),
      _B_FIELD_ICONID(BAGL_GLYPH_ICON_EYE_BADGE),
      _B_FIELD_TEXT(NULL)
    ),
};

#endif // TARGET_NANOS
