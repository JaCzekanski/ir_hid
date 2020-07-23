#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void uartInit();
void uartWrite(uint8_t data);
void _uartPuts(const char *s);
#define uartPuts(x) _uartPuts(PSTR(x))
void uartHex16(uint16_t i);
void uartHex8(uint8_t i);

#ifdef __cplusplus
}
#endif
