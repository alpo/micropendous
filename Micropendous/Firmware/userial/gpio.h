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
 * Utility function for GPIO handling.
 *****************************************************************************/
#ifndef GPIO_H
#define GPIO_H
#include <stdbool.h>
#include <stdint.h>

/**
 * Configure the GPIO hardware.
 *
 * \param port the name of the port ('A', 'B', etc.)
 * \param io_mask marks the input/output direction: 0: input, 1: output
 * \return true on success, false on error
 *****************************************************************************/
bool gpio_config(uint8_t port, uint8_t io_mask);

/**
 * Read the GPIO hardware configuration..
 *
 * \param port the name of the port ('A', 'B', etc.)
 * \return the input/output mask: 0: input, 1: output
 *****************************************************************************/
uint8_t gpio_get_config(uint8_t port);

/**
 * Read a value from a GPIO port.
 *
 * \param port the name of the port ('A', 'B', etc.)
 * \return the the port's value.
 *****************************************************************************/
uint8_t gpio_read(uint8_t port);

/**
 * Set reset bits on a GPIO port.
 *
 * \param port the name of the port ('A', 'B', etc.)
 * \param and_mask a bitmask that will be and-ed to the port value.
 * \param or_mask a bitmask that will be or-ed to the port value.
 * \param xor_mask a bitmask that will be xor-ed to the port value.
 * \return true on success, false on error
 *****************************************************************************/
bool gpio_bitset(uint8_t port, uint8_t and_mask, uint8_t or_mask, uint8_t xor_mask);

#endif // !defined GPIO_H
