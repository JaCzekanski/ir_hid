#include <avr/interrupt.h>
#include <avr/pgmspace.h>


#if defined(__AVR_ATmega328P__)
#define IRMP_INPUT_PIN 3
#elif defined(__AVR_ATtiny85__)
#define IRMP_INPUT_PIN 0
#else
#error Unsupported MCU in ir.cpp
#endif

#define ARDUINO
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 1
#define LED_BUILTIN 5
#define F(x) x

void pinMode(int pin, int mode) {
    if (pin == IRMP_INPUT_PIN) {
#if defined(__AVR_ATmega328P__)
        if (mode == OUTPUT) DDRD |= (1 << 3);
        else            DDRD &= ~(1 << 3);
#elif defined(__AVR_ATtiny85__)
        if (mode == OUTPUT) DDRB |= (1 << 0);
        else            DDRB &= ~(1 << 0);
#else
#error Unsupported MCU in ir.cpp
#endif
    }
}

void digitalWrite(int pin, int value) {
    // Not used
}

int digitalRead(int pin) {
    if (pin == IRMP_INPUT_PIN) {
#if defined(__AVR_ATmega328P__)
        return (PIND & (1 << 3)) != 0;
#elif defined(__AVR_ATtiny85__)
        return (PINB & (1 << 0)) != 0;
#else
#error Unsupported MCU in ir.cpp
#endif
    }
    return 0;
}

struct Print {
    void print(char *c) {}
    void print(int i, int bits) {}
    void print(char c) {}
    void print(const char *c) {}
    void println() {}
};

//Print Serial;
#define __FlashStringHelper char
//#define IRMP_PROTOCOL_NAMES 1



//#include <irmpSelectMain15Protocols.h>

#define IRMP_SUPPORT_SIRCS_PROTOCOL    1
#define IRMP_SUPPORT_NEC_PROTOCOL      1
#define IRMP_SUPPORT_SAMSUNG_PROTOCOL  1
#define IRMP_SUPPORT_KASEIKYO_PROTOCOL 1
#include <irmp.c.h>