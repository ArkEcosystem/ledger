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

#include "apdu.h"
#include "ctx.h"

//////////////////////////////////////////////////////////////////////

extern txContent_t txContent;
extern void ui_idle(void);

//////////////////////////////////////////////////////////////////////

void sample_main(void) {
    volatile unsigned int rx = 0U;
    volatile unsigned int tx = 0U;
    volatile unsigned int flags = 0U;

    // DESIGN NOTE: the bootloader ignores the way APDU are fetched. The only
    // goal is to retrieve APDU.
    // When APDU are to be fetched from multiple IOs, like NFC+USB+BLE, make
    // sure the io_event is called with a
    // switch event, before the apdu is replied to the bootloader. This avoid
    // APDU injection faults.
    for (;;) {
        volatile unsigned short sw = 0U;

        BEGIN_TRY {
            TRY {
                rx = tx;
                // Ensure no race in catch_other if io_exchange throws an error
                tx = 0U;
                rx = io_exchange(CHANNEL_APDU | flags, rx);
                flags = 0U;

                // No apdu received.
                // Let's reset the session and the bootloader config.
                if (rx == 0U) {
                    THROW(0x6982);
                }

                PRINTF("New APDU received:\n%.*H\n", rx, G_io_apdu_buffer);

                handle_apdu(&flags, &tx, &txContent);
            }
            CATCH(EXCEPTION_IO_RESET) {
                THROW(EXCEPTION_IO_RESET);
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
                G_io_apdu_buffer[tx + 1U] = sw;

                tx += 2U;
            }
            FINALLY {}
        }
        END_TRY;
    }

    // return_to_dashboard:
    return;
}

//////////////////////////////////////////////////////////////////////

void app_exit(void) {
    BEGIN_TRY_L(exit) {
        TRY_L(exit) {
            os_sched_exit(-1);
        }
        FINALLY_L(exit) {}
    }
    END_TRY_L(exit);
}

//////////////////////////////////////////////////////////////////////

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

                USB_power(0U);
                USB_power(1U);

                ui_idle();

                #ifdef HAVE_BLE
                    BLE_power(0U, NULL);
                    BLE_power(1U, "Nano X");
                #endif  // HAVE_BLE
 
                sample_main();
            }
            CATCH(EXCEPTION_IO_RESET) {
                // reset IO and UX
                CLOSE_TRY;
                continue;
            }
            CATCH_ALL {
                CLOSE_TRY;
                break;
            }
            FINALLY {}
        }
        END_TRY;
    }
    app_exit();

    return 0U;
}
