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
#include "hardware.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/power.h>


#define HW_LED      PORTD   // the LED register
#define HW_LED_DDR  DDRD    // the LED direction register


#if FEATURE_LEDS
static inline void led_init(void);
#else
#define led_init()
#endif //FEATURE_LEDS


void hw_init(void)
{
    // Disable watchdog if enabled by bootloader/fuses
    MCUSR &= ~(1U << WDRF);
    wdt_disable();
    clock_prescale_set(clock_div_1);    // Disable clock division
    SMCR = _BV(SM1) | _BV(SM0);     // Power save mode

    led_init();
}

void hw_enable_int(void)
{
    sei();
}

void hw_delay_ms(uint16_t ms)
{
    const uint16_t loops_per_ms = F_CPU / 4000U; // 4 ticks per inner loop
    uint16_t tmp;
    __asm__ __volatile__(
        "\n1:"                  "\n\t"
        "movw %[tmp], %[lpm]"   "\n\t"
        "\n2:"                  "\n\t"
        "sbiw %[tmp], 1"        "\n\t"
        "brne 2b"               "\n\t"
        "sbiw %[ms], 1"         "\n\t"
        "brne 1b"               "\n\t"
        "clr r1"                "\n\t"
        : [tmp] "=e" (tmp),
          [ms] "+e" (ms)
        : [lpm] "r" (loops_per_ms)
        : "r0"
    );
}

void hw_sleep(void)
{
    __asm__ __volatile__("sleep");
}

#if FEATURE_LEDS
void led_init(void)
{
    HW_LED &= ~HW_LED_ALL;
    HW_LED_DDR |= HW_LED_ALL;
}

void led_set(led_t led)
{
    HW_LED &= led | ~HW_LED_ALL;
    HW_LED |= led & HW_LED_ALL;
}
#endif //FEATURE_LEDS
