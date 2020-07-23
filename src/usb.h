#pragma once

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_CONSUMER 2

typedef struct {
    uint8_t reportId;
    uint8_t mod;
    uint8_t key;
} ReportKeyboard;

typedef struct {
    uint8_t reportId;
    uint16_t media;
} ReportConsumer;


extern ReportKeyboard reportKeyboard;
extern ReportConsumer reportConsumer;

#ifdef __cplusplus
extern "C" {
#endif

void usbInitialize();
void usbSendHidReportBlocking(uint8_t reportId);
void usbSendHidKeyboard(uint8_t scancode);
void usbSendHidConsumer(uint16_t scancode);

#ifdef __cplusplus
}
#endif
