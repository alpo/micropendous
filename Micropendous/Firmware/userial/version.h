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
/**
 * \file
 *
 * Version number and build parameters.
 *****************************************************************************/
#ifndef VERSION_H
#define VERSION_H
#include "version.h"


/**
 * \def PROJECT_NAME
 *
 * Project name
 *****************************************************************************/
#define PROJECT_NAME    "userial"


/**
 * \def VERSION_STRING
 *
 * Firmware version number
 *****************************************************************************/
#define VERSION_STRING  "1.9"


#define USERIAL_BOARD 1
#define BUMBLEB_BOARD 2


#define FEATURE_LEDS    1       //!< LEDs support
#define FEATURE_I2C     1       //!< I2C support
#define FEATURE_SPI     1       //!< SPI support
#define FEATURE_GPIO    1       //!< GPIO support
#if BOARD == USERIAL_BOARD
#define FEATURE_ADC     1       //!< ADC support
#else
#define FEATURE_ADC     0       //!< No ADC support on bumble-b (AT90USB162)
#endif


#endif  // VERSION_H
