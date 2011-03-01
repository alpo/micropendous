/*
 *  userial -- a USB to I2C converter
 *
 *  Copyright (c) 2008-2009, Thomas Pircher <tehpeh@gmx.net>
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *****************************************************************************/
/**
 * \file
 *
 * Utility function for ADC handling.
 *****************************************************************************/
#include "version.h"
#include "adc.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>


#if (F_CPU / 2) < 200000
    #define ADC_PRESCALER 0x00
#elif (F_CPU / 2) < 20000
    #define ADC_PRESCALER 0x01
#elif (F_CPU / 4) < 20000
    #define ADC_PRESCALER 0x02
#elif (F_CPU / 8) < 20000
    #define ADC_PRESCALER 0x03
#elif (F_CPU / 16) < 20000
    #define ADC_PRESCALER 0x04
#elif (F_CPU / 32) < 20000
    #define ADC_PRESCALER 0x05
#elif (F_CPU / 64) < 20000
    #define ADC_PRESCALER 0x06
#else
    #define ADC_PRESCALER 0x07
#endif


#if FEATURE_ADC
static adc_cfg_t adc_cfg;


void adc_init(void)
{
    ADCSRA = _BV(ADEN) | (ADC_PRESCALER << ADPS0);
    adc_cfg.ref_type = adc_int;
    adc_cfg.ref_v = 256;
}


void adc_config(adc_cfg_t *cfg)
{
    uint8_t ref;
    switch (cfg->ref_type) {
        default:
        case adc_int:
            ref = 0;
            break;
        case adc_avcc:
            ref = 1;
            break;
        case adc_aref:
            ref = 3;
            break;
    }
    ADCSRA = _BV(ADEN) | (ref << REFS0) | (ADC_PRESCALER << ADPS0);
    adc_cfg = *cfg;
}


void adc_get_config(adc_cfg_t *cfg)
{
    *cfg = adc_cfg;
}


uint16_t adc_read(uint8_t channel)
{
#if (defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || \
        defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) || \
        defined(__AVR_ATmega32U6__))
    DDRF  &= ~_BV(channel);
    DIDR0 |=  _BV(channel);
#elif (defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__))
    if (channel < 8)
    {
        DDRF  &= ~_BV(channel);
        DIDR0 |=  _BV(channel);
    }
    else if (channel == 8)
    {
        DDRD  &= ~_BV(4);
        DIDR2 |=  _BV(0);
    }
    else if (channel < 11)
    {
        DDRD  &= ~_BV(channel - 3);
        DIDR2 |=  _BV(channel - 8);
    }
    else
    {
        DDRB  &= ~_BV(channel - 7);
        DIDR2 |=  _BV(channel - 8);
    }
#endif

    // start reading
    ADMUX = (0x03 << REFS0) | channel;
    ADCSRA |= _BV(ADSC);

    // wait for conversion
    while((ADCSRA & _BV(ADSC)));

    // return result
    return ADC;
}
#endif // FEATURE_ADC
