#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "uart.h"
#include "usb.h"

#define REQUEST_READ_LAST 1
#define REQUEST_ENTRY_COUNT 2
#define REQUEST_READ_ENTRY 3
#define REQUEST_WRITE_ENTRY 4
#define REQUEST_REMOVE_ENTRY 5

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
        // Consumer
        0x85, REPORT_ID_CONSUMER,      // REPORT_ID (2)
        0x05, 0x0C,                       // usage page (consumer device)
        0x09, 0x01,                   // usage -- consumer control
        0xA1, 0x01,                   // collection (application)
        /* 1 Media Keys */
        0x15, 0x00,                   // logical minimum
        0x26, 0xFF, 0x03,               // logical maximum (3ff)
        0x19, 0x00,                   // usage minimum (0)
        0x2A, 0xFF, 0x03,               // usage maximum (3ff)
        0x95, 0x01,                   // report count (4)
        0x75, 0x10,                   // report size (16)
        0x81, 0x00,                   // input
        0xC0, /* end collection */

        // Keyboard
        0x85, REPORT_ID_KEYBOARD,      // REPORT_ID (1)
        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x06,                    // USAGE (Keyboard)
        0xa1, 0x01,                    // COLLECTION (Application)
        0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

        // Modifiers
        0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
        0x75, 0x01,                    //   REPORT_SIZE (1)
        0x95, 0x08,                    //   REPORT_COUNT (8)
        0x81, 0x02,                    //   INPUT (Data,Var,Abs)

        // One key
        0x05, 0x07,                    //  USAGE_PAGE (Keyboard)
        0x95, 0x01,                    //  REPORT_COUNT (1)
        0x75, 0x08,                    //  REPORT_SIZE (8)
        0x15, 0x00,                    //  LOGICAL_MINIMUM (0)
        0x26, 0xE7, 0x00,              //  LOGICAL_MAXIMUM (231)
        0x19, 0x00,                    //  USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0xE7,                    //  USAGE_MAXIMUM (Keyboard Right GUI)
        0x81, 0x00,                    //  INPUT (Data,Ary,Abs)

        0xc0,                          // END_COLLECTION
};


ReportKeyboard reportKeyboard;
ReportConsumer reportConsumer;
static uchar idleRate;

extern void getReceivedData(uint8_t buffer[8]);
extern int getEntryCount();
extern int readEntry(uint8_t buffer[8], int i);
extern void usbWriteEntry(uint8_t buffer[8]);
extern void usbRemoveEntry(uint8_t entry);
static uint8_t buffer[8];

uchar usbFunctionWrite(uchar *data, uchar len) {
    // TODO: Check which request is processed
    usbWriteEntry(data);
    return 1;
}

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
    usbRequest_t *rq = (void *) data;

    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
        if (rq->bRequest == USBRQ_HID_GET_REPORT) {  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            // Not used by macOS
            if (rq->wValue.bytes[0] == REPORT_ID_KEYBOARD) {
                usbMsgPtr = (void *) &reportKeyboard;
                return sizeof(reportKeyboard);
            } else if (rq->wValue.bytes[0] == REPORT_ID_CONSUMER) {
                usbMsgPtr = (void *) &reportConsumer;
                return sizeof(reportConsumer);
            }
        } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
            usbMsgPtr = &idleRate;
            return 1;
        } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
            idleRate = rq->wValue.bytes[1];
        }
    } else if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_VENDOR) {
//        uartPuts("Vendor rq ");
//        uartHex16(rq->bRequest);
//        uartPuts(" index ");
//        uartHex16(rq->wIndex.word);
//        uartPuts(" length ");
//        uartHex16(rq->wLength.word);
//        uartPuts(" data: ");
//
//        for (int i = 0; i < rq->wLength.word; i++) {
//            uartHex8(rq->wValue.bytes[i]);
//        }
//        uartPuts("\n");
        if (rq->bRequest == REQUEST_READ_LAST) {
            getReceivedData(buffer);
            usbMsgPtr = buffer;
            return sizeof(buffer);
        } else if (rq->bRequest == REQUEST_ENTRY_COUNT) {
            buffer[0] = getEntryCount();
            usbMsgPtr = buffer;
            return 1;
        } else if (rq->bRequest == REQUEST_READ_ENTRY) {
            readEntry(buffer, rq->wIndex.word);
            usbMsgPtr = buffer;
            return sizeof(buffer);
        } else if (rq->bRequest == REQUEST_WRITE_ENTRY) {
            return USB_NO_MSG;
        } else if (rq->bRequest == REQUEST_REMOVE_ENTRY) {
            usbRemoveEntry(rq->wIndex.word);
            return USB_NO_MSG;
        }
        return 0;
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

void usbInitialize() {
    reportKeyboard.reportId = REPORT_ID_KEYBOARD;
    reportConsumer.reportId = REPORT_ID_CONSUMER;

    cli();

    clock_prescale_set(clock_div_1);

    USBOUT &= ~(_BV(USB_CFG_DMINUS_BIT) | _BV(USB_CFG_DPLUS_BIT));
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();

    usbInit();
    sei();
}

void usbSendHidReportBlocking(uint8_t reportId) {
    for (;;) {
        usbPoll();
        if (usbInterruptIsReady()) {
            if (reportId == REPORT_ID_KEYBOARD) {
                usbSetInterrupt((uint8_t *) &reportKeyboard, sizeof(reportKeyboard));
            } else if (reportId == REPORT_ID_CONSUMER) {
                usbSetInterrupt((uint8_t *) &reportConsumer, sizeof(reportConsumer));
            }
            return;
        }
    }
}

void usbSendHidKeyboard(uint8_t scancode) {
    reportKeyboard.key = scancode;
    usbSendHidReportBlocking(REPORT_ID_KEYBOARD);
    reportKeyboard.key = 0;
    usbSendHidReportBlocking(REPORT_ID_KEYBOARD);
}

void usbSendHidConsumer(uint16_t scancode) {
    reportConsumer.media = scancode;
    usbSendHidReportBlocking(REPORT_ID_CONSUMER);
    reportConsumer.media = 0;
    usbSendHidReportBlocking(REPORT_ID_CONSUMER);
}

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */
// section copied from EasyLogger
/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
void calibrateOscillator(void)
{
    uchar       step = 128;
    uchar       trialValue = 0, optimumValue;
    int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}
/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/
#endif