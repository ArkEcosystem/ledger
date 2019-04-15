/*******************************************************************************
*   Ark Wallet
*   (c) 2017 Ledger
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
#include "cx.h"
#include <stdbool.h>
#include "arkParse.h"
#include "arkHelpers.h"

#include "os_io_seproxyhal.h"
#include "string.h"

#include "glyphs.h"

bagl_element_t tmp_element;
unsigned char G_io_seproxyhal_spi_buffer[IO_SEPROXYHAL_BUFFER_SIZE_B];

uint32_t set_result_get_publicKey(void);

#define MAX_BIP32_PATH 10

#define CLA 0xE0
#define INS_GET_PUBLIC_KEY 0x02
#define INS_SIGN 0x04
#define INS_SIGN_MESSAGE 0x08
#define INS_GET_APP_CONFIGURATION 0x06
#define P1_CONFIRM 0x01
#define P1_NON_CONFIRM 0x00
#define P2_NO_CHAINCODE 0x00
#define P2_CHAINCODE 0x01
/* Choosing P1_FIRST to be 0x00 has the problem that it is not possible to distinguish
Case1 from Case2:
Case1 (a single chunk): p1==P1_LAST
Case2 (a few chunks): p1==P1_FIRST, p1==P1_MORE, p1==P1_MORE, ..., p1==P1_LAST
When we receive a chunk with p1==P1_LAST we can't tell if this is the first chunk
from Case1 or the last chunk from Case2. Because we append all chunks in a buffer,
we need to be able to tell if this is the first chunk or not - so that we know
whether to put the chunk in the beginning of the buffer or append at position
rawMessageLength. To workaround this issue we reset rawMessageLength to 0 in
ui_idle() and always append chunks at rawMessageLength.
*/
#define P1_FIRST 0x00
#define P1_MORE 0x01
#define P1_LAST 0x80
#define P2_SECP256K1 0x40
#define P2_ED25519 0x80

#define OFFSET_CLA 0
#define OFFSET_INS 1
#define OFFSET_P1 2
#define OFFSET_P2 3
#define OFFSET_LC 4
#define OFFSET_CDATA 5

#define MAX_RAW_TX 300
#define MAX_RAW_MESSAGE 132

typedef struct publicKeyContext_t {
    cx_ecfp_public_key_t publicKey;
    uint8_t address[41];
    uint8_t chainCode[32];
    bool getChaincode;
} publicKeyContext_t;

typedef struct transactionContext_t {
    cx_curve_t curve;
    uint8_t pathLength;
    uint32_t bip32Path[MAX_BIP32_PATH];
    uint8_t rawTx[MAX_RAW_TX];
    uint32_t rawTxLength;
} transactionContext_t;

typedef struct messageContext {
    cx_curve_t curve;
    uint8_t pathLength;
    uint32_t bip32Path[MAX_BIP32_PATH];
    uint8_t rawMessage[MAX_RAW_MESSAGE];
    uint32_t rawMessageLength;
} messageContext_t;

union {
    publicKeyContext_t publicKeyContext;
    transactionContext_t transactionContext;
    messageContext_t messageContext;
} tmpCtx;

txContent_t txContent;

volatile uint8_t fidoTransport;
volatile char addressSummary[32];
volatile char fullAddress[68];
volatile char fullAmount[50];
volatile char fullMessage[MAX_RAW_MESSAGE + 1];
volatile char maxFee[50];
volatile bool dataPresent;

bagl_element_t tmp_element;

unsigned int io_seproxyhal_touch_exit(const bagl_element_t *e);
unsigned int io_seproxyhal_touch_tx_ok(const bagl_element_t *e);
unsigned int io_seproxyhal_touch_tx_cancel(const bagl_element_t *e);
unsigned int io_seproxyhal_touch_address_ok(const bagl_element_t *e);
unsigned int io_seproxyhal_touch_address_cancel(const bagl_element_t *e);
unsigned int io_seproxyhal_touch_sign_message_ok(const bagl_element_t *e);
unsigned int io_seproxyhal_touch_sign_message_cancel(const bagl_element_t *e);
void ui_idle(void);
ux_state_t ux;
// display stepped screens
unsigned int ux_step;
unsigned int ux_step_count;

const unsigned char hex_digits[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void array_hexstr(char *strbuf, const void *bin, unsigned int len) {
    while (len--) {
        *strbuf++ = hex_digits[((*((char *)bin)) >> 4) & 0xF];
        *strbuf++ = hex_digits[(*((char *)bin)) & 0xF];
        bin = (const void *)((unsigned int)bin + 1);
    }
    *strbuf = 0; // EOS
}

const bagl_element_t *ui_menu_item_out_over(const bagl_element_t *e) {
    // the selection rectangle is after the none|touchable
    e = (const bagl_element_t *)(((unsigned int)e) + sizeof(bagl_element_t));
    return e;
}

#define BAGL_FONT_OPEN_SANS_LIGHT_16_22PX_AVG_WIDTH 10
#define BAGL_FONT_OPEN_SANS_REGULAR_10_13PX_AVG_WIDTH 8
#define MAX_CHAR_PER_LINE 25

#define COLOR_BG_1 0xF9F9F9
#define COLOR_APP 0x27a2db
#define COLOR_APP_LIGHT 0x93d1ed


#if defined(TARGET_NANOS)

const ux_menu_entry_t menu_main[];

const ux_menu_entry_t menu_about[] = {
    {NULL, NULL, 0, NULL, "Version", APPVERSION, 0, 0},
    {menu_main, NULL, 2, &C_icon_back, "Back", NULL, 61, 40},
    UX_MENU_END};

const ux_menu_entry_t menu_main[] = {
    {NULL, NULL, 0, &C_icon_ark, "Use wallet to", "view accounts", 33, 12},
    {menu_about, NULL, 0, NULL, "About", NULL, 0, 0},
    {NULL, os_sched_exit, 0, &C_icon_dashboard, "Quit app", NULL, 50, 29},
    UX_MENU_END};

#endif // #if TARGET_NANOS


#if defined(TARGET_NANOS)

/* Dummy macros for readability. */
#define _B_FIELD_TYPE(a) (a)
#define _B_FIELD_USERID(a) (a)
#define _B_FIELD_X(a) (a)
#define _B_FIELD_Y(a) (a)
#define _B_FIELD_WIDTH(a) (a)
#define _B_FIELD_HEIGHT(a) (a)
#define _B_FIELD_STROKE(a) (a)
#define _B_FIELD_RADIUS(a) (a)
#define _B_FIELD_FILL(a) (a)
#define _B_FIELD_FGCOLOR(a) (a)
#define _B_FIELD_BGCOLOR(a) (a)
#define _B_FIELD_FONTID(a) (a)
#define _B_FIELD_ICONID(a) (a)
#define _B_FIELD_TEXT(a) (a)
#define _B_FIELD_TOUCH_AREA_BRIM(a) (a)
#define _B_FIELD_OVERFGCOLOR(a) (a)
#define _B_FIELD_OVERBGCOLOR(a) (a)
#define _B_FIELD_TAP(a) (a)
#define _B_FIELD_OUT(a) (a)
#define _B_FIELD_OVER(a) (a)

/* When scrolling a long text, how much time to wait at the beginning and end of
the scroll. This macro takes the number of milliseconds and produces a number in
unit "100ms", as used by the SDK. For example, the SDK interprets 10 as "10 times
100ms" (== 1000ms == 1sec). */
#define _B_SCROLL_WAIT_MS(ms) ((ms) / 100)

#define _B_CLEAN_SCREEN { \
  { \
    _B_FIELD_TYPE(BAGL_RECTANGLE), \
    _B_FIELD_USERID(0x00), \
    _B_FIELD_X(0), \
    _B_FIELD_Y(0), \
    _B_FIELD_WIDTH(128), \
    _B_FIELD_HEIGHT(32), \
    _B_FIELD_STROKE(0), \
    _B_FIELD_RADIUS(0), \
    _B_FIELD_FILL(BAGL_FILL), \
    _B_FIELD_FGCOLOR(0x000000), \
    _B_FIELD_BGCOLOR(0xFFFFFF), \
    _B_FIELD_FONTID(0), \
    _B_FIELD_ICONID(0) \
  }, \
  _B_FIELD_TEXT(NULL), \
  _B_FIELD_TOUCH_AREA_BRIM(0), \
  _B_FIELD_OVERFGCOLOR(0), \
  _B_FIELD_OVERBGCOLOR(0), \
  _B_FIELD_TAP(NULL), \
  _B_FIELD_OUT(NULL), \
  _B_FIELD_OVER(NULL) \
}

#define _B_ICON(iconid, userid, x, y, w, h) { \
  { \
    _B_FIELD_TYPE(BAGL_ICON), \
    _B_FIELD_USERID(userid), \
    _B_FIELD_X(x), \
    _B_FIELD_Y(y), \
    _B_FIELD_WIDTH(w), \
    _B_FIELD_HEIGHT(h), \
    _B_FIELD_STROKE(0), \
    _B_FIELD_RADIUS(0), \
    _B_FIELD_FILL(0), \
    _B_FIELD_FGCOLOR(0xFFFFFF), \
    _B_FIELD_BGCOLOR(0x000000), \
    _B_FIELD_FONTID(0), \
    _B_FIELD_ICONID(iconid) \
  }, \
  _B_FIELD_TEXT(NULL), \
  _B_FIELD_TOUCH_AREA_BRIM(0), \
  _B_FIELD_OVERFGCOLOR(0), \
  _B_FIELD_OVERBGCOLOR(0), \
  _B_FIELD_TAP(NULL), \
  _B_FIELD_OUT(NULL), \
  _B_FIELD_OVER(NULL) \
}

#define _B_ICON_LEFT(iconid, userid) \
  _B_ICON( \
      _B_FIELD_ICONID(iconid), \
      _B_FIELD_USERID(userid), \
      _B_FIELD_X(3), \
      _B_FIELD_Y(12), \
      _B_FIELD_WIDTH(7), \
      _B_FIELD_HEIGHT(7) \
  )

#define _B_ICON_RIGHT(iconid, userid) \
  _B_ICON( \
      _B_FIELD_ICONID(iconid), \
      _B_FIELD_USERID(userid), \
      _B_FIELD_X(117), \
      _B_FIELD_Y(13), \
      _B_FIELD_WIDTH(8), \
      _B_FIELD_HEIGHT(6) \
  )

#define _B_ICON_CROSS(userid) \
  _B_ICON_LEFT( \
      _B_FIELD_ICONID(BAGL_GLYPH_ICON_CROSS), \
      _B_FIELD_USERID(userid) \
  )

#define _B_ICON_CHECK(userid) \
  _B_ICON_RIGHT( \
      _B_FIELD_ICONID(BAGL_GLYPH_ICON_CHECK), \
      _B_FIELD_USERID(userid) \
  )

#define _B_LABELINE(userid, x, y, w, h, stroke, fontid, iconid, text) { \
  { \
    _B_FIELD_TYPE(BAGL_LABELINE), \
    _B_FIELD_USERID(userid), \
    _B_FIELD_X(x), \
    _B_FIELD_Y(y), \
    _B_FIELD_WIDTH(w), \
    _B_FIELD_HEIGHT(h), \
    _B_FIELD_STROKE(stroke), \
    _B_FIELD_RADIUS(0), \
    _B_FIELD_FILL(0), \
    _B_FIELD_FGCOLOR(0xFFFFFF), \
    _B_FIELD_BGCOLOR(0x000000), \
    _B_FIELD_FONTID(fontid), \
    _B_FIELD_ICONID(iconid) \
  }, \
  _B_FIELD_TEXT(text), \
  _B_FIELD_TOUCH_AREA_BRIM(0), \
  _B_FIELD_OVERFGCOLOR(0), \
  _B_FIELD_OVERBGCOLOR(0), \
  _B_FIELD_TAP(NULL), \
  _B_FIELD_OUT(NULL), \
  _B_FIELD_OVER(NULL) \
}

#define _B_LABELINE_REGULAR(userid, y, text) \
  _B_LABELINE( \
    _B_FIELD_USERID(userid), \
    _B_FIELD_X(0), \
    _B_FIELD_Y(y), \
    _B_FIELD_WIDTH(128), \
    _B_FIELD_HEIGHT(32), \
    _B_FIELD_STROKE(0), \
    _B_FIELD_FONTID(BAGL_FONT_OPEN_SANS_REGULAR_11px | BAGL_FONT_ALIGNMENT_CENTER), \
    _B_FIELD_ICONID(0), \
    _B_FIELD_TEXT(text) \
  )

#define _B_LABELINE_UPPER_REGULAR(userid, text) \
  _B_LABELINE_REGULAR( \
    _B_FIELD_USERID(userid), \
    _B_FIELD_Y(12), \
    _B_FIELD_TEXT(text) \
  )

#define _B_LABELINE_BOLD(userid, y, text) \
  _B_LABELINE( \
    _B_FIELD_USERID(userid), \
    _B_FIELD_X(0), \
    _B_FIELD_Y(y), \
    _B_FIELD_WIDTH(128), \
    _B_FIELD_HEIGHT(32), \
    _B_FIELD_STROKE(0), \
    _B_FIELD_FONTID(BAGL_FONT_OPEN_SANS_EXTRABOLD_11px | BAGL_FONT_ALIGNMENT_CENTER), \
    _B_FIELD_ICONID(0), \
    _B_FIELD_TEXT(text) \
  )

#define _B_LABELINE_UPPER_BOLD(userid, text) \
  _B_LABELINE_BOLD( \
    _B_FIELD_USERID(userid), \
    _B_FIELD_Y(12), \
    _B_FIELD_TEXT(text) \
  )

#define _B_LABELINE_LOWER_BOLD(userid, text) \
  _B_LABELINE_BOLD( \
    _B_FIELD_USERID(userid), \
    _B_FIELD_Y(26), \
    _B_FIELD_TEXT(text) \
  )

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

const bagl_element_t *ui_address_prepro(const bagl_element_t *element) {
    if (element->component.userid <= 0) {
        return element;
    }

    if (ux_step != element->component.userid - 1) {
        return NULL;
    }

    switch (element->component.userid) {
    case 1:
        UX_CALLBACK_SET_INTERVAL(2000);
        break;
    case 2:
        UX_CALLBACK_SET_INTERVAL(MAX(
            3000, 1000 + bagl_label_roundtrip_duration_ms(element, 7)));
        break;
    }

    return element;
}

unsigned int ui_address_nanos_button(unsigned int button_mask,
                                     unsigned int button_mask_counter);
#endif // #if defined(TARGET_NANOS)


#if defined(TARGET_NANOS)

volatile char *ui_approval_transfer[][2] = {
    {"Operation", fullAmount},
    {"To", fullAddress},
    {"Amount", fullAmount},
    {"Fees", fullAmount},
};

volatile char *ui_approval_vote1[][2] = {
    {"Operation", fullAmount},
    {"Vote", fullAddress},
    {"Fees", fullAmount},
};

volatile char *ui_approval_vote2[][2] = {
    {"Operation", fullAmount},
    {"Vote 1", fullAddress},
    {"Vote 2", fullAddress},
    {"Fees", fullAmount},
};


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

const bagl_element_t *ui_approval_prepro(const bagl_element_t *element) {
    unsigned int display;
    uint32_t addressLength;

    if (element->component.userid <= 0) {
        return element;
    }

    // display the meta element when at least bigger
    display = (ux_step == element->component.userid - 1) ||
      (element->component.userid >= 0x02 && ux_step >= 1);

    if (!display) {
        return NULL;
    }

    switch (element->component.userid) {
    case 0x01:
        UX_CALLBACK_SET_INTERVAL(2000);
        return element;
    case 0x02:
    case 0x12:
        os_memmove(&tmp_element, element, sizeof(bagl_element_t));
        display = ux_step - 1;
        if (txContent.type == OPERATION_TYPE_TRANSFER) {
            switch (display) {
                case 0: // Operation
                    strcpy((char *)fullAmount, "Transfer");
                    goto display_transfer;
                case 1: // Destination
                    addressLength = ark_public_key_to_encoded_base58(txContent.recipientId, 21, (unsigned char *)fullAddress, sizeof(fullAddress), txContent.recipientId[0], 1);
                    fullAddress[addressLength] = '\0';
                    goto display_transfer;
                case 2: // Amount
                    ark_print_amount(txContent.amount, (char *)fullAmount, sizeof(fullAmount));
                    goto display_transfer;
                case 3: // fees
                    ark_print_amount(txContent.fee, (char *)fullAmount, sizeof(fullAmount));
                    display_transfer:
                    tmp_element.text = (char *)ui_approval_transfer[display][(element->component.userid)>>4];
                    break;
            }
        }
        else
        if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 1)) {
            switch(display) {
                case 0: // Operation
                    strcpy((char *)fullAmount, "1 vote");
                    goto display_vote1;
                case 1: // Vote
                    os_memmove((void *)fullAddress, tmpCtx.transactionContext.rawTx + txContent.assetOffset, 67);
                    fullAddress[67] = '\0';
                    goto display_vote1;
                case 2: // fees
                    ark_print_amount(txContent.fee, (char *)fullAmount, sizeof(fullAmount));
                    display_vote1:
                    tmp_element.text = (char *)ui_approval_vote1[display][(element->component.userid)>>4];
                    break;
            }
        }
        else
        if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 2)) {
            switch(display) {
                case 0: // Operation
                    strcpy((char *)fullAmount, "2 votes");
                    goto display_vote2;
                case 1: // Vote 1
                    os_memmove((void *)fullAddress, tmpCtx.transactionContext.rawTx + txContent.assetOffset, 67);
                    fullAddress[67] = '\0';
                    goto display_vote2;
                case 2: // Vote 2
                    os_memmove((void *)fullAddress, tmpCtx.transactionContext.rawTx + txContent.assetOffset + 67, 67);
                    fullAddress[67] = '\0';
                    goto display_vote2;
                case 3: // fees
                    ark_print_amount(txContent.fee, (char *)fullAmount, sizeof(fullAmount));
                    display_vote2:
                    tmp_element.text = (char *)ui_approval_vote2[display][(element->component.userid)>>4];
                    break;
            }
        }

        UX_CALLBACK_SET_INTERVAL(MAX(
            3000, 1000 + bagl_label_roundtrip_duration_ms(&tmp_element, 7)));
        return &tmp_element;
    }

    return element;
}



unsigned int ui_approval_nanos_button(unsigned int button_mask,
                                      unsigned int button_mask_counter);

#endif // #if defined(TARGET_NANOS)


#if defined(TARGET_NANOS)


const bagl_element_t ui_sign_message_nanos[] = {
    _B_CLEAN_SCREEN,
    _B_ICON_CROSS(_B_FIELD_USERID(0x00)),
    _B_ICON_CHECK(_B_FIELD_USERID(0x00)),

    _B_LABELINE_UPPER_BOLD(_B_FIELD_USERID(0x01), _B_FIELD_TEXT("Sign")),
    _B_LABELINE_LOWER_BOLD(_B_FIELD_USERID(0x01), _B_FIELD_TEXT("message")),

    _B_LABELINE_UPPER_REGULAR(_B_FIELD_USERID(0x02), _B_FIELD_TEXT("Message")),
    _B_LABELINE(
      _B_FIELD_USERID(0x02),
      _B_FIELD_X(23),
      _B_FIELD_Y(26),
      _B_FIELD_WIDTH(82),
      _B_FIELD_HEIGHT(12),
      _B_FIELD_STROKE(BAGL_STROKE_FLAG_ONESHOT | _B_SCROLL_WAIT_MS(1000)),
      _B_FIELD_FONTID(BAGL_FONT_OPEN_SANS_EXTRABOLD_11px | BAGL_FONT_ALIGNMENT_CENTER),
      _B_FIELD_ICONID(BAGL_GLYPH_ICON_EYE_BADGE),
      _B_FIELD_TEXT((char *)fullMessage)
    ),

    _B_LABELINE_UPPER_REGULAR(_B_FIELD_USERID(0x03), _B_FIELD_TEXT("Part 1 of 2")),
    _B_LABELINE_LOWER_BOLD(_B_FIELD_USERID(0x03), _B_FIELD_TEXT("foo")),

    _B_LABELINE_UPPER_REGULAR(_B_FIELD_USERID(0x04), _B_FIELD_TEXT("Part 2 of 2")),
    _B_LABELINE_LOWER_BOLD(_B_FIELD_USERID(0x04), _B_FIELD_TEXT("bar")),
};

const bagl_element_t *ui_sign_message_prepro(const bagl_element_t *element) {
    if (element->component.userid > 0) {
        if (ux_step != element->component.userid - 1) {
            return NULL;
        }
        switch (element->component.userid) {
            case 1:
                UX_CALLBACK_SET_INTERVAL(2000);
                break;
            case 2:
                UX_CALLBACK_SET_INTERVAL(MAX(
                    3000, 1000 + bagl_label_roundtrip_duration_ms(element, 7)));
                break;
        }
        return element;
    }
    return element;
}

unsigned int ui_sign_message_nanos_button(unsigned int button_mask,
                                     unsigned int button_mask_counter);



#endif

void ui_idle(void) {
#if defined(TARGET_NANOS)
    UX_MENU_DISPLAY(0, menu_main, NULL);
#endif // #if TARGET_ID
}


unsigned int io_seproxyhal_touch_exit(const bagl_element_t *e) {
    // Go back to the dashboard
    os_sched_exit(0);
    return 0; // do not redraw the widget
}

unsigned int io_seproxyhal_touch_address_ok(const bagl_element_t *e) {
    uint32_t tx = set_result_get_publicKey();
    G_io_apdu_buffer[tx++] = 0x90;
    G_io_apdu_buffer[tx++] = 0x00;
    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
    // Display back the original UX
    ui_idle();
    return 0; // do not redraw the widget
}

unsigned int io_seproxyhal_touch_address_cancel(const bagl_element_t *e) {
    G_io_apdu_buffer[0] = 0x69;
    G_io_apdu_buffer[1] = 0x85;
    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
    // Display back the original UX
    ui_idle();
    return 0; // do not redraw the widget
}

#if defined(TARGET_NANOS)
unsigned int ui_address_nanos_button(unsigned int button_mask,
                                     unsigned int button_mask_counter) {
    switch (button_mask) {
    case BUTTON_EVT_RELEASED | BUTTON_LEFT: // CANCEL
        io_seproxyhal_touch_address_cancel(NULL);
        break;

    case BUTTON_EVT_RELEASED | BUTTON_RIGHT: { // OK
        io_seproxyhal_touch_address_ok(NULL);
        break;
    }
    }
    return 0;
}
#endif // #if defined(TARGET_NANOS)

unsigned int io_seproxyhal_touch_tx_ok(const bagl_element_t *e) {
    uint8_t privateKeyData[64];
    unsigned char finalhash[32];
    cx_sha256_t localHash;
    cx_ecfp_private_key_t privateKey;
    uint32_t tx = 0;

    os_perso_derive_node_bip32(tmpCtx.transactionContext.curve, tmpCtx.transactionContext.bip32Path, tmpCtx.transactionContext.pathLength, privateKeyData, NULL);
    cx_ecfp_init_private_key(tmpCtx.transactionContext.curve, privateKeyData, 32, &privateKey);
    os_memset(privateKeyData, 0, sizeof(privateKeyData));
    if (tmpCtx.transactionContext.curve == CX_CURVE_256K1) {
        cx_sha256_init(&localHash);
        cx_hash(&localHash.header, CX_LAST, tmpCtx.transactionContext.rawTx, tmpCtx.transactionContext.rawTxLength, finalhash);
#if CX_APILEVEL >= 8
        tx = cx_ecdsa_sign(&privateKey, CX_RND_RFC6979 | CX_LAST, CX_SHA256, finalhash, sizeof(finalhash), G_io_apdu_buffer, NULL);
#else        
        tx = cx_ecdsa_sign(&privateKey, CX_RND_RFC6979 | CX_LAST, CX_SHA256, finalhash, sizeof(finalhash), G_io_apdu_buffer);
        G_io_apdu_buffer[0] = 0x30;
#endif        
    } else {
#if CX_APILEVEL >= 8
        tx = cx_eddsa_sign(&privateKey, CX_LAST, CX_SHA512, tmpCtx.transactionContext.rawTx, tmpCtx.transactionContext.rawTxLength, NULL, 0, G_io_apdu_buffer, NULL);
#else        
        tx = cx_eddsa_sign(&privateKey, NULL, CX_LAST, CX_SHA512, tmpCtx.transactionContext.rawTx, tmpCtx.transactionContext.rawTxLength, G_io_apdu_buffer);
#endif        
    }

    os_memset(&privateKey, 0, sizeof(privateKey));
    // os_memmove(G_io_apdu_buffer + tx, tmpCtx.transactionContext.rawTx, sizeof(tmpCtx.transactionContext.rawTx));
    // tx += sizeof(tmpCtx.transactionContext.rawTx);
    G_io_apdu_buffer[tx++] = 0x90;
    G_io_apdu_buffer[tx++] = 0x00;

    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
    // Display back the original UX
    ui_idle();

    return 0; // do not redraw the widget
}

unsigned int io_seproxyhal_touch_tx_cancel(const bagl_element_t *e) {
    G_io_apdu_buffer[0] = 0x69;
    G_io_apdu_buffer[1] = 0x85;
    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
    // Display back the original UX
    ui_idle();
    return 0; // do not redraw the widget
}

#if defined(TARGET_NANOS)

unsigned int ui_approval_nanos_button(unsigned int button_mask,
                                      unsigned int button_mask_counter) {
    switch (button_mask) {
    case BUTTON_EVT_RELEASED | BUTTON_LEFT:
        io_seproxyhal_touch_tx_cancel(NULL);
        break;

    case BUTTON_EVT_RELEASED | BUTTON_RIGHT: {
        io_seproxyhal_touch_tx_ok(NULL);
        break;
    }
    }
    return 0;
}

#endif // #if defined(TARGET_NANOS)

unsigned int io_seproxyhal_touch_sign_message_ok(const bagl_element_t *e) {
    uint8_t privateKeyData[64];
    unsigned char finalhash[32];
    cx_sha256_t localHash;
    cx_ecfp_private_key_t privateKey;
    uint32_t tx = 0;

    os_perso_derive_node_bip32(tmpCtx.messageContext.curve, tmpCtx.messageContext.bip32Path, tmpCtx.messageContext.pathLength, privateKeyData, NULL);
    cx_ecfp_init_private_key(tmpCtx.messageContext.curve, privateKeyData, 32, &privateKey);
    os_memset(privateKeyData, 0, sizeof(privateKeyData));
    if (tmpCtx.messageContext.curve == CX_CURVE_256K1) {
        cx_sha256_init(&localHash);
        cx_hash(&localHash.header, CX_LAST, tmpCtx.messageContext.rawMessage, tmpCtx.messageContext.rawMessageLength, finalhash);
#if CX_APILEVEL >= 8
        tx = cx_ecdsa_sign(&privateKey, CX_RND_RFC6979 | CX_LAST, CX_SHA256, finalhash, sizeof(finalhash), G_io_apdu_buffer, NULL);
#else
        tx = cx_ecdsa_sign(&privateKey, CX_RND_RFC6979 | CX_LAST, CX_SHA256, finalhash, sizeof(finalhash), G_io_apdu_buffer);
        G_io_apdu_buffer[0] = 0x30;
#endif
    } else {
        THROW(0x9000);
#if CX_APILEVEL >= 8
        tx = cx_eddsa_sign(&privateKey, CX_LAST, CX_SHA512, tmpCtx.messageContext.rawMessage, tmpCtx.messageContext.rawMessageLength, NULL, 0, G_io_apdu_buffer, NULL);
#else
        tx = cx_eddsa_sign(&privateKey, NULL, CX_LAST, CX_SHA512, tmpCtx.messageContext.rawMessage, tmpCtx.messageContext.rawMessageLength, G_io_apdu_buffer);
#endif
    }

    os_memset(&privateKey, 0, sizeof(privateKey));
    G_io_apdu_buffer[tx++] = 0x90;
    G_io_apdu_buffer[tx++] = 0x00;

    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);

    // Display back the original UX
    ui_idle();

    return 0; // do not redraw the widget
}

unsigned int io_seproxyhal_touch_sign_message_cancel(const bagl_element_t *e) {
    G_io_apdu_buffer[0] = 0x69;
    G_io_apdu_buffer[1] = 0x85;
    // Send back the response, do not restart the event loop
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
    // Display back the original UX
    ui_idle();
    return 0; // do not redraw the widget
}

#if defined(TARGET_NANOS)

unsigned int ui_sign_message_nanos_button(unsigned int button_mask,
                                      unsigned int button_mask_counter) {
    switch (button_mask) {
    case BUTTON_EVT_RELEASED | BUTTON_LEFT:
        io_seproxyhal_touch_sign_message_cancel(NULL);
        break;

    case BUTTON_EVT_RELEASED | BUTTON_RIGHT:
        io_seproxyhal_touch_sign_message_ok(NULL);
        break;
    }
    return 0;
}

#endif // #if defined(TARGET_NANOS)


unsigned short io_exchange_al(unsigned char channel, unsigned short tx_len) {
    switch (channel & ~(IO_FLAGS)) {
    case CHANNEL_KEYBOARD:
        break;

    // multiplexed io exchange over a SPI channel and TLV encapsulated protocol
    case CHANNEL_SPI:
        if (tx_len) {
            io_seproxyhal_spi_send(G_io_apdu_buffer, tx_len);

            if (channel & IO_RESET_AFTER_REPLIED) {
                reset();
            }
            return 0; // nothing received from the master so far (it's a tx
                      // transaction)
        } else {
            return io_seproxyhal_spi_recv(G_io_apdu_buffer,
                                          sizeof(G_io_apdu_buffer), 0);
        }

    default:
        THROW(INVALID_PARAMETER);
    }
    return 0;
}

uint32_t set_result_get_publicKey() {
    uint32_t tx = 0;
    uint32_t addressLength = strlen((char *)tmpCtx.publicKeyContext.address);
    G_io_apdu_buffer[tx++] = 33;
    ark_compress_public_key(&tmpCtx.publicKeyContext.publicKey,
                            G_io_apdu_buffer + tx, 33);
    tx += 33;
    G_io_apdu_buffer[tx++] = addressLength;
    os_memmove(G_io_apdu_buffer + tx, tmpCtx.publicKeyContext.address,
               addressLength);
    tx += addressLength;
    if (tmpCtx.publicKeyContext.getChaincode) {
        os_memmove(G_io_apdu_buffer + tx, tmpCtx.publicKeyContext.chainCode,
                   32);
        tx += 32;
    }
    return tx;
}

void handleGetPublicKey(uint8_t p1, uint8_t p2, uint8_t *dataBuffer,
                        uint16_t dataLength, volatile unsigned int *flags,
                        volatile unsigned int *tx) {
    UNUSED(dataLength);
    uint8_t privateKeyData[33];
    uint32_t bip32Path[MAX_BIP32_PATH];
    uint32_t i;
    uint8_t bip32PathLength = *(dataBuffer++);
    cx_ecfp_private_key_t privateKey;
    uint8_t p2Chain = p2 & 0x3F;
    cx_curve_t curve;
    uint8_t addressLength;

    if ((bip32PathLength < 0x01) || (bip32PathLength > MAX_BIP32_PATH)) {
        PRINTF("Invalid path\n");
        THROW(0x6a80);
    }
    if ((p1 != P1_CONFIRM) && (p1 != P1_NON_CONFIRM)) {
        THROW(0x6B00);
    }
    if ((p2Chain != P2_CHAINCODE) && (p2Chain != P2_NO_CHAINCODE)) {
        THROW(0x6B01);
    }
    if (((p2 & P2_SECP256K1) == 0) && ((p2 & P2_ED25519) == 0)) {
        THROW(0x6B02);
    }
    if (((p2 & P2_SECP256K1) != 0) && ((p2 & P2_ED25519) != 0)) {
        THROW(0x6B03);
    }
    curve = (((p2 & P2_ED25519) != 0) ? CX_CURVE_Ed25519 : CX_CURVE_256K1);

    for (i = 0; i < bip32PathLength; i++) {
        bip32Path[i] = (dataBuffer[0] << 24) | (dataBuffer[1] << 16) |
                       (dataBuffer[2] << 8) | (dataBuffer[3]);
        dataBuffer += 4;
    }
    tmpCtx.publicKeyContext.getChaincode = (p2Chain == P2_CHAINCODE);
    os_perso_derive_node_bip32(curve, bip32Path, bip32PathLength,
                               privateKeyData,
                               (tmpCtx.publicKeyContext.getChaincode
                                    ? tmpCtx.publicKeyContext.chainCode
                                    : NULL));
    cx_ecfp_init_private_key(curve, privateKeyData, 32, &privateKey);
    cx_ecfp_generate_pair(curve, &tmpCtx.publicKeyContext.publicKey,
                          &privateKey, 1);

    os_memset(&privateKey, 0, sizeof(privateKey));
    os_memset(privateKeyData, 0, sizeof(privateKeyData));
    ark_compress_public_key(&tmpCtx.publicKeyContext.publicKey, privateKeyData,
                            33);
    addressLength = ark_public_key_to_encoded_base58(
        privateKeyData, 33, tmpCtx.publicKeyContext.address,
        sizeof(tmpCtx.publicKeyContext.address), 23, 0);
    tmpCtx.publicKeyContext.address[addressLength] = '\0';
    if (p1 == P1_NON_CONFIRM) {
        *tx = set_result_get_publicKey();
        THROW(0x9000);
    } else {
        os_memset((void *)fullAddress, 0, sizeof(fullAddress));
        os_memmove((void *)fullAddress, tmpCtx.publicKeyContext.address, 5);
        os_memmove((void *)(fullAddress + 5), "...", 3);
        os_memmove((void *)(fullAddress + 8),
                   tmpCtx.publicKeyContext.address + addressLength - 4, 4);

// prepare for a UI based reply
#if defined(TARGET_NANOS)
#if 0
        snprintf(fullAddress, sizeof(fullAddress), " 0x%.*s ", 40,
                 tmpCtx.publicKeyContext.address);
#endif
        ux_step = 0;
        ux_step_count = 2;
        UX_DISPLAY(ui_address_nanos, ui_address_prepro);
#endif // #if TARGET

        *flags |= IO_ASYNCH_REPLY;
    }
}

void handleSign(uint8_t p1, uint8_t p2, uint8_t *workBuffer,
                uint16_t dataLength, volatile unsigned int *flags,
                volatile unsigned int *tx) {
    UNUSED(tx);
    //uint8_t addressLength;
    uint32_t i;
    //unsigned char address[41];
    bool last = (p1 & P1_LAST);
    p1 &= 0x7F;

    if (p1 == P1_FIRST) {
        tmpCtx.transactionContext.pathLength = workBuffer[0];
        if ((tmpCtx.transactionContext.pathLength < 0x01) ||
            (tmpCtx.transactionContext.pathLength > MAX_BIP32_PATH)) {
            PRINTF("Invalid path\n");
            THROW(0x6a80);
        }
        workBuffer++;
        dataLength--;
        for (i = 0; i < tmpCtx.transactionContext.pathLength; i++) {
            tmpCtx.transactionContext.bip32Path[i] =
                (workBuffer[0] << 24) | (workBuffer[1] << 16) |
                (workBuffer[2] << 8) | (workBuffer[3]);
            workBuffer += 4;
            dataLength -= 4;
        }
        if (((p2 & P2_SECP256K1) == 0) && ((p2 & P2_ED25519) == 0)) {
            THROW(0x6B00);
        }
        if (((p2 & P2_SECP256K1) != 0) && ((p2 & P2_ED25519) != 0)) {
            THROW(0x6B00);
        }
        tmpCtx.transactionContext.curve =
            (((p2 & P2_ED25519) != 0) ? CX_CURVE_Ed25519 : CX_CURVE_256K1);
    } else 
    if (p1 != P1_MORE) {
        THROW(0x6B00);
    }

    if (p1 == P1_FIRST) {
        tmpCtx.transactionContext.rawTxLength = dataLength;
        os_memmove(tmpCtx.transactionContext.rawTx, workBuffer, dataLength);
    }
    else
    if (p1 == P1_MORE) {
        if ((tmpCtx.transactionContext.rawTxLength + dataLength) > MAX_RAW_TX) {
            THROW(0x6A80);
        }
        os_memmove(tmpCtx.transactionContext.rawTx + tmpCtx.transactionContext.rawTxLength, workBuffer, dataLength);
        tmpCtx.transactionContext.rawTxLength += dataLength;
    }

    if (!last) {
        THROW(0x9000);
    }

    if (parseTx(tmpCtx.transactionContext.rawTx, tmpCtx.transactionContext.rawTxLength, &txContent) != USTREAM_FINISHED) {        
        THROW(0x6A80);
    }
    //ark_print_amount(txContent.amount + txContent.fee, fullAmount, sizeof(fullAmount));

#if defined(TARGET_NANOS)
    //os_memset(addressSummary, 0, addressLength);
    //os_memmove((void *)addressSummary, tmpCtx.publicKeyContext.address, addressLength);
    //SPRINTF(addressSummary, "%d / %x -> %s", addressLength, txContent.recipientId[0], tmpCtx.publicKeyContext.address);
    // os_memmove((void *)(addressSummary + 5), "...", 3);
    // os_memmove((void *)(addressSummary + 8),
    //            tmpCtx.publicKeyContext.address + addressLength - 5, 5);
    // cx_sha256_init(&localHash);
    // cx_hash(&localHash.header, CX_LAST, tmpCtx.transactionContext.rawTx, tmpCtx.transactionContext.rawTxLength, finalhash);
    // SPRINTF(addressSummary, "tx: %d %x%x ... %x%x", dataLength, tmpCtx.transactionContext.rawTx[0],tmpCtx.transactionContext.rawTx[1],tmpCtx.transactionContext.rawTx[dataLength-7],tmpCtx.transactionContext.rawTx[dataLength-6]);

    //SPRINTF(addressSummary, "to %d %x%x ... %x%x", txContent.timestamp, tmpCtx.transactionContext.rawTx[0],tmpCtx.transactionContext.rawTx[1],tmpCtx.transactionContext.rawTx[dataLength-7],tmpCtx.transactionContext.rawTx[dataLength-6]);
    // os_memmove((void *)addressSummary, "size tx: ", 9);
    // os_memmove((void *)(addressSummary + 9), dataLength, 4);
#endif
    // os_memmove(tmpCtx.transactionContext.rawTx, SIGN_PREFIX,
    //            sizeof(SIGN_PREFIX));

#if defined(TARGET_NANOS)
    ux_step = 0;
    // "confirm", amount, address, [sourcetag], [destinationtag], fees
    if (txContent.type == OPERATION_TYPE_TRANSFER) {
        ux_step_count = 5;
    }
    else
    if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 1)) {    
        ux_step_count = 4;
    }
    else
    if ((txContent.type == OPERATION_TYPE_VOTE) && (txContent.voteSize == 2)) {    
        ux_step_count = 5;
    }        
    // if (txContent.sourceTagPresent) {
    //     ux_step_count++;
    // }
    // if (txContent.destinationTagPresent) {
    //     ux_step_count++;
    // }
    UX_DISPLAY(ui_approval_nanos, ui_approval_prepro);
#endif // #if TARGET

    *flags |= IO_ASYNCH_REPLY;
}

void handleSignMessage(uint8_t p1, uint8_t p2, uint8_t *workBuffer,
                uint16_t dataLength, volatile unsigned int *flags,
                volatile unsigned int *tx) {
    UNUSED(tx);

    if (p1 != P1_LAST) {
        THROW(0x6a81);
    }

    tmpCtx.messageContext.pathLength = workBuffer[0];
    if ((tmpCtx.messageContext.pathLength < 0x01) ||
        (tmpCtx.messageContext.pathLength > MAX_BIP32_PATH)) {
        PRINTF("Invalid path\n");
        THROW(0x6a85);
    }
    workBuffer++;
    dataLength--;

    for (uint32_t i = 0; i < tmpCtx.messageContext.pathLength; i++) {
        tmpCtx.messageContext.bip32Path[i] =
            (workBuffer[0] << 24) | (workBuffer[1] << 16) |
            (workBuffer[2] << 8) | (workBuffer[3]);
        workBuffer += 4;
        dataLength -= 4;
    }

    if ((p2 & P2_SECP256K1) == 0 && (p2 & P2_ED25519) == 0) {
        THROW(0x6b01);
    }
    if ((p2 & P2_SECP256K1) != 0 && (p2 & P2_ED25519) != 0) {
        THROW(0x6b02);
    }
    tmpCtx.messageContext.curve = (p2 & P2_ED25519) != 0 ? CX_CURVE_Ed25519 : CX_CURVE_256K1;

    if (dataLength > MAX_RAW_MESSAGE) {
        THROW(0x6a81);
    }

    os_memmove(tmpCtx.messageContext.rawMessage, workBuffer, dataLength);
    tmpCtx.messageContext.rawMessageLength = dataLength;

    os_memset((void *)fullMessage, 0, sizeof(fullMessage));
    os_memmove((void *)fullMessage, tmpCtx.messageContext.rawMessage, tmpCtx.messageContext.rawMessageLength);
    fullMessage[tmpCtx.messageContext.rawMessageLength] = '\0';

#if defined(TARGET_NANOS)
    ux_step = 0;
    ux_step_count = 2;

    UX_DISPLAY(ui_sign_message_nanos, ui_sign_message_prepro);
#endif // #if TARGET

    *flags |= IO_ASYNCH_REPLY;
}

void handleGetAppConfiguration(uint8_t p1, uint8_t p2, uint8_t *workBuffer,
                               uint16_t dataLength,
                               volatile unsigned int *flags,
                               volatile unsigned int *tx) {
    UNUSED(p1);
    UNUSED(p2);
    UNUSED(workBuffer);
    UNUSED(dataLength);
    UNUSED(flags);
    G_io_apdu_buffer[0] = 0x00;
    G_io_apdu_buffer[1] = LEDGER_MAJOR_VERSION;
    G_io_apdu_buffer[2] = LEDGER_MINOR_VERSION;
    G_io_apdu_buffer[3] = LEDGER_PATCH_VERSION;
    *tx = 4;
    THROW(0x9000);
}

void handleApdu(volatile unsigned int *flags, volatile unsigned int *tx) {
    unsigned short sw = 0;

    BEGIN_TRY {
        TRY {
            if (G_io_apdu_buffer[OFFSET_CLA] != CLA) {
                THROW(0x6E00);
            }

            switch (G_io_apdu_buffer[OFFSET_INS]) {
            case INS_GET_PUBLIC_KEY:
                handleGetPublicKey(G_io_apdu_buffer[OFFSET_P1],
                                   G_io_apdu_buffer[OFFSET_P2],
                                   G_io_apdu_buffer + OFFSET_CDATA,
                                   G_io_apdu_buffer[OFFSET_LC], flags, tx);
                break;

            case INS_SIGN:
                handleSign(G_io_apdu_buffer[OFFSET_P1],
                           G_io_apdu_buffer[OFFSET_P2],
                           G_io_apdu_buffer + OFFSET_CDATA,
                           G_io_apdu_buffer[OFFSET_LC], flags, tx);
                break;

            case INS_SIGN_MESSAGE:
                handleSignMessage(G_io_apdu_buffer[OFFSET_P1],
                                  G_io_apdu_buffer[OFFSET_P2],
                                  G_io_apdu_buffer + OFFSET_CDATA,
                                  G_io_apdu_buffer[OFFSET_LC], flags, tx);
                break;

            case INS_GET_APP_CONFIGURATION:
                handleGetAppConfiguration(
                    G_io_apdu_buffer[OFFSET_P1], G_io_apdu_buffer[OFFSET_P2],
                    G_io_apdu_buffer + OFFSET_CDATA,
                    G_io_apdu_buffer[OFFSET_LC], flags, tx);
                break;

            default:
                THROW(0x6D00);
                break;
            }
        }
        CATCH_OTHER(e) {
            switch (e & 0xF000) {
            case 0x6000:
                // Wipe the transaction context and report the exception
                sw = e;
                os_memset(&txContent, 0, sizeof(txContent));
                break;
            case 0x9000:
                // All is well
                sw = e;
                break;
            default:
                // Internal error
                sw = 0x6800 | (e & 0x7FF);
                break;
            }
            // Unexpected exception => report
            G_io_apdu_buffer[*tx] = sw >> 8;
            G_io_apdu_buffer[*tx + 1] = sw;
            *tx += 2;
        }
        FINALLY {
        }
    }
    END_TRY;
}

void sample_main(void) {
    volatile unsigned int rx = 0;
    volatile unsigned int tx = 0;
    volatile unsigned int flags = 0;

    // DESIGN NOTE: the bootloader ignores the way APDU are fetched. The only
    // goal is to retrieve APDU.
    // When APDU are to be fetched from multiple IOs, like NFC+USB+BLE, make
    // sure the io_event is called with a
    // switch event, before the apdu is replied to the bootloader. This avoid
    // APDU injection faults.
    for (;;) {
        volatile unsigned short sw = 0;

        BEGIN_TRY {
            TRY {
                rx = tx;
                tx = 0; // ensure no race in catch_other if io_exchange throws
                        // an error
                rx = io_exchange(CHANNEL_APDU | flags, rx);
                flags = 0;

                // no apdu received, well, reset the session, and reset the
                // bootloader configuration
                if (rx == 0) {
                    THROW(0x6982);
                }

                handleApdu(&flags, &tx);
            }
            CATCH_OTHER(e) {
                switch (e & 0xF000) {
                case 0x6000:
                    // Wipe the transaction context and report the exception
                    sw = e;
                    os_memset(&txContent, 0, sizeof(txContent));
                    break;
                case 0x9000:
                    // All is well
                    sw = e;
                    break;
                default:
                    // Internal error
                    sw = 0x6800 | (e & 0x7FF);
                    break;
                }
                // Unexpected exception => report
                G_io_apdu_buffer[tx] = sw >> 8;
                G_io_apdu_buffer[tx + 1] = sw;
                tx += 2;
            }
            FINALLY {
            }
        }
        END_TRY;
    }

    // return_to_dashboard:
    return;
}

// override point, but nothing more to do
void io_seproxyhal_display(const bagl_element_t *element) {
    io_seproxyhal_display_default((bagl_element_t *)element);
}

unsigned char io_event(unsigned char channel) {
    // nothing done with the event, throw an error on the transport layer if
    // needed

    // can't have more than one tag in the reply, not supported yet.
    switch (G_io_seproxyhal_spi_buffer[0]) {
    case SEPROXYHAL_TAG_FINGER_EVENT:
        UX_FINGER_EVENT(G_io_seproxyhal_spi_buffer);
        break;

    case SEPROXYHAL_TAG_BUTTON_PUSH_EVENT:
        UX_BUTTON_PUSH_EVENT(G_io_seproxyhal_spi_buffer);
        break;

    case SEPROXYHAL_TAG_STATUS_EVENT:
        if (G_io_apdu_media == IO_APDU_MEDIA_USB_HID &&
            !(U4BE(G_io_seproxyhal_spi_buffer, 3) &
              SEPROXYHAL_TAG_STATUS_EVENT_FLAG_USB_POWERED)) {
            THROW(EXCEPTION_IO_RESET);
        }
    // no break is intentional
    default:
        UX_DEFAULT_EVENT();
        break;

    case SEPROXYHAL_TAG_DISPLAY_PROCESSED_EVENT:
        UX_DISPLAYED_EVENT({});
        break;

    case SEPROXYHAL_TAG_TICKER_EVENT:
        UX_TICKER_EVENT(G_io_seproxyhal_spi_buffer, {
            if (UX_ALLOWED) {
#if 0
                if (skipWarning && (ux_step == 0)) {
                    ux_step++;
                }
#endif
                if (ux_step_count) {
                    // prepare next screen
                    ux_step = (ux_step + 1) % ux_step_count;
                    // redisplay screen
                    UX_REDISPLAY();
                }
            }
        });
        break;
    }

    // close the event if not done previously (by a display or whatever)
    if (!io_seproxyhal_spi_is_status_sent()) {
        io_seproxyhal_general_status();
    }

    // command has been processed, DO NOT reset the current APDU transport
    return 1;
}

void app_exit(void) {
    BEGIN_TRY_L(exit) {
        TRY_L(exit) {
            os_sched_exit(-1);
        }
        FINALLY_L(exit) {
        }
    }
    END_TRY_L(exit);
}

__attribute__((section(".boot"))) int main(void) {
    // exit critical section
    __asm volatile("cpsie i");

    // ensure exception will work as planned
    os_boot();

    for (;;) {
        os_memset(&txContent, 0, sizeof(txContent));

        UX_INIT();
        BEGIN_TRY {
            TRY {
                io_seproxyhal_init();

                USB_power(1);

                ui_idle();

                sample_main();
            }
            CATCH(EXCEPTION_IO_RESET) {
                // reset IO and UX
                continue;
            }
            CATCH_ALL {
                break;
            }
            FINALLY {
            }
        }
        END_TRY;
    }
    app_exit();

    return 0;
}
