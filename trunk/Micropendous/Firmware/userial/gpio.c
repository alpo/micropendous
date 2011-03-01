/*
 *  userial -- a USB to I2C converter
 *
 *  Copyright (c) 2008-2010, Thomas Pircher <tehpeh@gmx.net>
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
#include "version.h"
#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>


#if FEATURE_GPIO
bool gpio_config(uint8_t port, uint8_t io_mask)
{
    switch (port) {
        #if BOARD == USERIAL_BOARD
        case 'A':
            DDRA = io_mask;
            break;
        #endif
        case 'B':
            DDRB = io_mask;
            break;
        #if BOARD == BUMBLEB_BOARD
        case 'C':
            DDRC = io_mask;
            break;
        case 'D':
            DDRD = io_mask;
            break;
        #endif
        default:
            return false;
    }
    return true;
}

uint8_t gpio_get_config(uint8_t port)
{
    switch (port) {
        #if BOARD == USERIAL_BOARD
        case 'A':
            return DDRA;
        #endif
        case 'B':
            return DDRB;
        #if BOARD == BUMBLEB_BOARD
        case 'C':
            return DDRC;
        case 'D':
            return DDRD;
        #endif
        default:
            return 0;
    }
}

uint8_t gpio_read(uint8_t port)
{
    switch (port) {
        #if BOARD == USERIAL_BOARD
        case 'A':
            return PINA;
        #endif
        case 'B':
            return PINB;
        #if BOARD == BUMBLEB_BOARD
        case 'C':
            return PINC;
        case 'D':
            return PIND;
        #endif
        default:
            return 0;
    }
}

bool gpio_bitset(uint8_t port, uint8_t and_mask, uint8_t or_mask, uint8_t xor_mask)
{
    switch (port) {
        #if BOARD == USERIAL_BOARD
        case 'A':
            PORTA = ((PORTA ^ xor_mask) | or_mask) & and_mask;
            break;
        #endif
        case 'B':
            PORTB = ((PORTB ^ xor_mask) | or_mask) & and_mask;
            break;
        #if BOARD == BUMBLEB_BOARD
        case 'C':
            PORTC = ((PORTC ^ xor_mask) | or_mask) & and_mask;
            break;
        case 'D':
            PORTD = ((PORTD ^ xor_mask) | or_mask) & and_mask;
            break;
        #endif
        default:
            return false;
    }
    return true;
}
#endif // FEATURE_GPIO
