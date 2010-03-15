/* Tone.cpp

  A Tone Generator Library

  Written by Brett Hagman

  Adapted for LUFA + Arduino by Opendous Inc. 2010-03
  For more information visit:  www.Micropendous.org/LUFAduino
  Note: Only Timer0(8-bit) and Timer3(16-bit) exist on all compatible
  USB AVRs as Timer1 is used by FreeRTOS

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Version Modified By    Date     Comments
------- -----------    -------- --------
0001    B Hagman       09/08/02 Initial coding
0002    B Hagman       09/08/18 Multiple pins
0003    B Hagman       09/08/18 Moved initialization from constructor to begin()
0004    B Hagman       09/09/26 Fixed problems with ATmega8
0005    B Hagman       09/11/23 Scanned prescalars for best fit on 8 bit timers
                       09/11/25 Changed pin toggle method to XOR
                       09/11/25 Fixed timer0 from being excluded
0006    D Mellis       09/12/29 Replaced objects with functions
0007    Opendous Inc.  10/03/15 Adapted for USB AVRs (AT90USB, ATmegaXUY)

*************************************************/

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <wiring.h>
#include <pins_micropendous.h>

// timerx_toggle_count:
//  > 0 - duration specified
//  = 0 - stopped
//  < 0 - infinitely (until stop() method called, or new play() called)

volatile long timer0_toggle_count;
volatile uint8_t *timer0_pin_port;
volatile uint8_t timer0_pin_mask;

volatile long timer3_toggle_count;
volatile uint8_t *timer3_pin_port;
volatile uint8_t timer3_pin_mask;

#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
		defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
		defined(__AVR_ATmega32U6__))
volatile long timer2_toggle_count;
volatile uint8_t *timer2_pin_port;
volatile uint8_t timer2_pin_mask;
#endif

#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
volatile long timer4_toggle_count;
volatile uint8_t *timer4_pin_port;
volatile uint8_t timer4_pin_mask;
#endif



#define AVAILABLE_TONE_PINS 1

const uint8_t PROGMEM tone_pin_to_timer_PGM[1] = { 2 };
static uint8_t tone_pins[AVAILABLE_TONE_PINS] = { 255 };




static int8_t toneBegin(uint8_t _pin)
{
  int8_t _timer = -1;

  // if we're already using the pin, the timer should be configured.  
  for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
    if (tone_pins[i] == _pin) {
      return pgm_read_byte(tone_pin_to_timer_PGM + i);
    }
  }
  
  // search for an unused timer.
  for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
    if (tone_pins[i] == 255) {
      tone_pins[i] = _pin;
      _timer = pgm_read_byte(tone_pin_to_timer_PGM + i);
      break;
    }
  }
  
  if (_timer != -1)
  {
    // Set timer specific stuff
    // All timers in CTC mode
    // 8 bit timers will require changing prescalar values,
    // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar
    switch (_timer)
    {
      case 0:
        // 8 bit timer
        TCCR0A = 0;
        TCCR0B = 0;
        bitWrite(TCCR0A, WGM01, 1);
        bitWrite(TCCR0B, CS00, 1);
        timer0_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer0_pin_mask = digitalPinToBitMask(_pin);
        break;
      case 3:
        // 16 bit timer
        TCCR3A = 0;
        TCCR3B = 0;
        bitWrite(TCCR3B, WGM32, 1);
        bitWrite(TCCR3B, CS30, 1);
        timer3_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer3_pin_mask = digitalPinToBitMask(_pin);
        break;
#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
      case 2:
        // 8 bit timer
        TCCR2A = 0;
        TCCR2B = 0;
        bitWrite(TCCR2A, WGM21, 1);
        bitWrite(TCCR2B, CS20, 1);
        timer2_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer2_pin_mask = digitalPinToBitMask(_pin);
        break;
#endif
#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
      case 4:
        // 16 bit timer
        TCCR4A = 0;
        TCCR4B = 0;
        //bitWrite(TCCR4B, WGM42, 1);
#warning Timer4 handling is not yet properly implemented in Tone.cpp
        bitWrite(TCCR4B, CS40, 1);
        timer4_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer4_pin_mask = digitalPinToBitMask(_pin);
        break;
#endif
    }
  }

  return _timer;
}



// frequency (in hertz) and duration (in milliseconds).

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
  uint8_t prescalarbits = 0b001;
  long toggle_count = 0;
  uint32_t ocr = 0;
  int8_t _timer;

  _timer = toneBegin(_pin);

  if (_timer >= 0)
  {
    // Set the pinMode as OUTPUT
    pinMode(_pin, OUTPUT);
    
    // if we are using an 8 bit timer, scan through prescalars to find the best fit
    if (_timer == 0 || _timer == 2)
    {
      ocr = F_CPU / frequency / 2 - 1;
      prescalarbits = 0b001;  // ck/1: same for both timers
      if (ocr > 255)
      {
        ocr = F_CPU / frequency / 2 / 8 - 1;
        prescalarbits = 0b010;  // ck/8: same for both timers

        if (_timer == 2 && ocr > 255)
        {
          ocr = F_CPU / frequency / 2 / 32 - 1;
          prescalarbits = 0b011;
        }

        if (ocr > 255)
        {
          ocr = F_CPU / frequency / 2 / 64 - 1;
          prescalarbits = _timer == 0 ? 0b011 : 0b100;

          if (_timer == 2 && ocr > 255)
          {
            ocr = F_CPU / frequency / 2 / 128 - 1;
            prescalarbits = 0b101;
          }

          if (ocr > 255)
          {
            ocr = F_CPU / frequency / 2 / 256 - 1;
            prescalarbits = _timer == 0 ? 0b100 : 0b110;
            if (ocr > 255)
            {
              // can't do any better than /1024
              ocr = F_CPU / frequency / 2 / 1024 - 1;
              prescalarbits = _timer == 0 ? 0b101 : 0b111;
            }
          }
        }
      }

#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
		defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
		defined(__AVR_ATmega32U6__))
      if (_timer == 2)
        TCCR2B = prescalarbits;
      else
#endif

		TCCR0B = prescalarbits;
    }
    else
    {
      // two choices for the 16 bit timers: ck/1 or ck/64
      ocr = F_CPU / frequency / 2 - 1;

      prescalarbits = 0b001;
      if (ocr > 0xffff)
      {
        ocr = F_CPU / frequency / 2 / 64 - 1;
        prescalarbits = 0b011;
      }

      if (_timer == 3)
        TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;
#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
      else if (_timer == 4)
        TCCR4B = (TCCR4B & 0b11111000) | prescalarbits;
#endif

    }
    

    // Calculate the toggle count
    if (duration > 0)
    {
      toggle_count = 2 * frequency * duration / 1000;
    }
    else
    {
      toggle_count = -1;
    }

    // Set the OCR for the given timer,
    // set the toggle count,
    // then turn on the interrupts
    switch (_timer)
    {

      case 0:
        OCR0A = ocr;
        timer0_toggle_count = toggle_count;
        bitWrite(TIMSK0, OCIE0A, 1);
        break;
#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
		defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
		defined(__AVR_ATmega32U6__))
      case 2:
        OCR2A = ocr;
        timer2_toggle_count = toggle_count;
        bitWrite(TIMSK2, OCIE2A, 1);
        break;
#endif

      case 3:
        OCR3A = ocr;
        timer3_toggle_count = toggle_count;
        bitWrite(TIMSK3, OCIE3A, 1);
        break;
#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
	  case 4:
        OCR4A = ocr;
        timer4_toggle_count = toggle_count;
        bitWrite(TIMSK4, OCIE4A, 1);
        break;
#endif
    }
  }
}


void noTone(uint8_t _pin)
{
  int8_t _timer = -1;
  
  for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
    if (tone_pins[i] == _pin) {
      _timer = pgm_read_byte(tone_pin_to_timer_PGM + i);
      tone_pins[i] = 255;
    }
  }
  
  switch (_timer)
  {
	case 0:
      TIMSK0 = 0;
      break;
    case 3:
      TIMSK3 = 0;
      break;

#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
		defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
		defined(__AVR_ATmega32U6__))
    case 2:
      TIMSK2 = 0;
      break;
#endif
#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
    case 4:
      TIMSK4 = 0;
      break;
#endif
  }

  digitalWrite(_pin, 0);
}


#ifndef TIMER0COMPAvectDefinition
#define TIMER0COMPAvectDefinition
ISR(TIMER0_COMPA_vect)
{
  if (timer0_toggle_count != 0)
  {
    // toggle the pin
    *timer0_pin_port ^= timer0_pin_mask;

    if (timer0_toggle_count > 0)
      timer0_toggle_count--;
  }
  else
  {
    TIMSK0 = 0;   // disable the interrupt
    *timer0_pin_port &= ~(timer0_pin_mask);  // keep pin low after stop
  }
}
#endif


#ifndef TIMER3COMPAvectDefinition
#define TIMER3COMPAvectDefinition
/*
ISR(TIMER3_COMPA_vect)
{
  if (timer3_toggle_count != 0)
  {
    // toggle the pin
    *timer3_pin_port ^= timer3_pin_mask;

    if (timer3_toggle_count > 0)
      timer3_toggle_count--;
  }
  else
  {
    TIMSK3 = 0;   // disable the interrupt
    *timer3_pin_port &= ~(timer3_pin_mask);  // keep pin low after stop
  }
}
*/
#endif


#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
#ifndef TIMER2COMPAvectDefinition
#define TIMER2COMPAvectDefinition
ISR(TIMER2_COMPA_vect)
{

  if (timer2_toggle_count != 0)
  {
    // toggle the pin
    *timer2_pin_port ^= timer2_pin_mask;

    if (timer2_toggle_count > 0)
      timer2_toggle_count--;
  }
  else
  {
    TIMSK2 = 0;   // disable the interrupt
    *timer2_pin_port &= ~(timer2_pin_mask);  // keep pin low after stop
  }
}
#endif
#endif


#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
#ifndef TIMER2COMPAvectDefinition
#define TIMER2COMPAvectDefinition
ISR(TIMER4_COMPA_vect)
{
  if (timer4_toggle_count != 0)
  {
    // toggle the pin
    *timer4_pin_port ^= timer4_pin_mask;

    if (timer4_toggle_count > 0)
      timer4_toggle_count--;
  }
  else
  {
    TIMSK4 = 0;   // disable the interrupt
    *timer4_pin_port &= ~(timer4_pin_mask);  // keep pin low after stop
  }
}
#endif
#endif
