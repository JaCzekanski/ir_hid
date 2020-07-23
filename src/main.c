#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "irmp.h"
#include "usbdrv.h"
#include "uart.h"
#include "usb.h"

#if defined(__AVR_ATmega328P__)
#define LED_INIT() DDRB |= (1 << 5);
#define LED_ON() PORTB |= (1 << 5)
#define LED_OFF() PORTB &= ~(1 << 5)
#elif defined(__AVR_ATtiny85__)
#define LED_INIT() DDRB |= (1 << 1);
#define LED_ON() PORTB |= (1 << 1)
#define LED_OFF() PORTB &= ~(1 << 1)
#else
#error Unsupported MCU in main.c
#endif

IRMP_DATA irmpData;

struct ButtonEntry {
    uint8_t protocol;
    uint16_t address;
    uint16_t command;

    uint8_t reportId;
    uint16_t key;
};

const int EEPROM_SIZE = 512;
const int ENTRY_SIZE = 8;
const int ENTRY_COUNT = EEPROM_SIZE / ENTRY_SIZE;

void readButtonEntryFromEeprom(struct ButtonEntry* dst, int i) {
    eeprom_read_block(dst, (void *) (i * ENTRY_SIZE), ENTRY_SIZE);
}

void getReceivedData(uint8_t buffer[8]) {
    struct ButtonEntry* e = (struct ButtonEntry*)buffer;
    e->protocol = irmpData.protocol;
    e->address = irmpData.address;
    e->command = irmpData.command;

    irmpData.protocol = 0;
}


int getEntryCount() {
    return ENTRY_COUNT;
}

void readEntry(uint8_t buffer[8], int i) {
    memset(buffer, 0, 8);
    struct ButtonEntry* e = (struct ButtonEntry*)buffer;
    if (i >= ENTRY_COUNT) {
        return;
    }

    readButtonEntryFromEeprom(e, i);
}

void usbRemoveEntry(uint8_t entry) {
    // Copy all entries after it
}

enum UsbState {
    UsbNop,
    UsbWriteButton,
};

enum UsbState usbState = UsbNop;
struct ButtonEntry entryToWrite;

void usbWriteEntry(uint8_t buffer[8]) {
    memcpy(&entryToWrite, buffer, sizeof(struct ButtonEntry));
    usbState = UsbWriteButton;
}

void writeEntry() {
    usbState = UsbNop;

    int i;
    for (i = 0; i < ENTRY_COUNT; i++) {
        struct ButtonEntry entry;
        readButtonEntryFromEeprom(&entry, i);

        if (entry.protocol == entryToWrite.protocol &&
            entry.address == entryToWrite.address &&
            entry.command == entryToWrite.command) {
            // Just update
            break;
        }

        if (entry.reportId == 0 || entry.reportId == 0xff) {
            // Free
            break;
        }
    }
    if (i == ENTRY_COUNT - 1) {
        // No free space
        return;
    }

    eeprom_update_block(&entryToWrite, (void *) (i * ENTRY_SIZE), ENTRY_SIZE);
}

int __attribute__((noreturn)) main(void) {
    LED_INIT();
    LED_ON();

    wdt_disable();

//    uartInit();
//    uartPuts("ir_hid v0.1\n");

    irmp_init();

    usbInitialize();
    LED_OFF();

    for (;;) {                /* main event loop */
        if (usbState == UsbWriteButton) {
            writeEntry();
        }
        if (irmp_get_data(&irmpData)) {
            // One entry - 8 bytes
            // protocol - 1 byte
            // address  - 2 bytes
            // command  - 2 bytes
            // reportId - 1 byte (0 - not assigned)
            // key      - 2 bytes
//            if (!(irmpData.flags & IRMP_FLAG_REPETITION)) {
                for (int i = 0; i < ENTRY_COUNT; i++) {
                    struct ButtonEntry e;
                    readButtonEntryFromEeprom(&e, i);

                    if (e.reportId == 0 || e.reportId == 0xff) break;
                    if (e.protocol != irmpData.protocol) continue;
                    if (e.address != irmpData.address) continue;
                    if (e.command != irmpData.command) continue;

                    // Simulate key
                    if (e.reportId == REPORT_ID_KEYBOARD) {
                        usbSendHidKeyboard(e.key);
                    } else if (e.reportId == REPORT_ID_CONSUMER) {
                        usbSendHidConsumer(e.key);
                    }
//                }
            }
        }

        usbPoll();
    }
}
