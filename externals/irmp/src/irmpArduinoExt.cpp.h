/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmpArduinoExt.cpp.h - Arduino extensions to the original irmp.c
 *
 * Copyright (c) 2019-2020 Armin Joachimsmeyer
 *
 * This file is part of IRMP https://github.com/ukw100/IRMP.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

//  // Must be included after declaration of irmp_start_bit_detected etc.
#if defined(ARDUINO)
#undef _IRSND_H_              // We are in IRMP now! Remove old symbol maybe set from former including irsnd.c.h.
#include "IRTimer.cpp.h"    // include code for timer
static bool irmp_led_feedback;

#if defined(IRMP_ENABLE_PIN_CHANGE_INTERRUPT)
#include "irmpPinChangeInterrupt.cpp.h"
#endif // defined(IRMP_ENABLE_PIN_CHANGE_INTERRUPT)

#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
uint_fast8_t irmp_input_pin; // global variable to hold input pin number. Is referenced by defining IRMP_INPUT_PIN as irmp_input_pin.

void irmp_init(uint_fast8_t aIrmpInputPin)
{
    irmp_input_pin = aIrmpInputPin;
#  if defined IRMP_ENABLE_PIN_CHANGE_INTERRUPT
    initPCIInterrupt();
#  else
    initIRTimerForReceive();
#  endif
#  ifdef IRMP_MEASURE_TIMING
    pinModeFast(IRMP_TIMING_TEST_PIN, OUTPUT);
#  endif
}
#endif // if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)

void irmp_init(void)
{
#  ifdef IRMP_INPUT_PIN
    pinModeFast(IRMP_INPUT_PIN, INPUT);                                 // set pin to input
#  else
    IRMP_PORT &= ~_BV(IRMP_BIT);                                        // deactivate pullup
    IRMP_DDR &= ~_BV(IRMP_BIT);                                         // set pin to input
#  endif
#  if defined IRMP_ENABLE_PIN_CHANGE_INTERRUPT
    initPCIInterrupt();
#  else
    initIRTimerForReceive();
#  endif
#  ifdef IRMP_MEASURE_TIMING
    pinModeFast(IRMP_TIMING_TEST_PIN, OUTPUT);
#  endif
}
#endif // defined(ARDUINO)
