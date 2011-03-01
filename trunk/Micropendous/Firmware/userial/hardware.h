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
 * Hardware abstraction layer.
 *****************************************************************************/
#ifndef HARDWARE_H
#define HARDWARE_H

#include "version.h"
#include <stdint.h>


/**
 * LED enumeration.
 *****************************************************************************/
typedef enum {
    HW_LED1 = 0x80U,    //!< LED number 1
    HW_LED2 = 0x40U,    //!< LED number 2
    HW_LED_ALL = HW_LED1 | HW_LED2, //!< all LEDs

    HW_LEDMASK_USB_NOTREADY = 0x00,
    HW_LEDMASK_USB_ENUMERATING = (HW_LED2),
    HW_LEDMASK_USB_READY = (HW_LED1),
    HW_LEDMASK_USB_ERROR = (HW_LED1 | HW_LED2),
} led_t;



/**
 * Initialise the hardware.
 *****************************************************************************/
void hw_init(void);

/**
 * Enable Interrupts
 *****************************************************************************/
void hw_enable_int(void);


#if FEATURE_LEDS
/**
 * Set the LEDs.
 *
 * \param led   bitmask of the LEDs
 *****************************************************************************/
void led_set(led_t led);

#else

#define led_set(led)        //!< dummy function

#endif //FEATURE_LEDS



/**
 * Wait for ms milliseconds.
 *
 * \param ms    number of milliseconds to wait
 *****************************************************************************/
void hw_delay_ms(uint16_t ms);

/**
 * Put the hw into sleep mode.
 *****************************************************************************/
void hw_sleep(void);


#endif // !defined HARDWARE_H
