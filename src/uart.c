#include <avr/pgmspace.h>
#include "uart.h"

#if defined(__AVR_ATmega328P__)
#define USE_DEBUG
#endif

#define BAUD 115200

void uartInit() {
#ifdef USE_DEBUG
    uint16_t b = (F_CPU / (8 * BAUD)) - 1;
    UCSR0A = 1 << U2X0;
    UBRR0H = (b >> 8) & 0xff;
    UBRR0L = b & 0xff;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
#endif
}

void uartWrite(uint8_t data) {
#ifdef USE_DEBUG
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
#endif
}

void _uartPuts(const char *s) {
    char c;
    while ((c = pgm_read_byte(s++))) {
        if (c == '\n') uartWrite('\r');
        uartWrite(c);
    }
}

PROGMEM static const char hexLUT[16] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
};

void uartHex16(uint16_t n) {
    uartPuts("0x");
    for (int i = 3; i>= 0; i--) {
        uartWrite(pgm_read_byte(&hexLUT[(n >> (i*4)) & 0xf]));
    }
}

void uartHex8(uint8_t n) {
    uartPuts("0x");
    for (int i = 1; i>= 0; i-- ) {
        uartWrite(pgm_read_byte(&hexLUT[(n >> (i*4)) & 0xf]));
    }
}