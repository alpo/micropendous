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
 * SPI driver.
 *****************************************************************************/
#ifndef SPI_H
#define SPI_H

#include "version.h"
#include <stdbool.h>
#include <stdint.h>


#if FEATURE_SPI

typedef enum {
    spi_msb_first,
    spi_lsb_first
} spi_data_order_t;


/**
 * Initialise the SPI hardware.
 *****************************************************************************/
void spi_init(void);

/**
 * Configure the SPI hardware.
 *
 * \param i2c_freq the clock frequency in kHz of the SPI master.
 * SPI Mode   CPOL   CPHA   Shift SCK-edge   Capture SCK-edge
 * 0          0      0      Falling          Rising
 * 1          0      1      Rising           Falling
 * 2          1      0      Rising           Falling
 * 3          1      1      Falling          Rising
 *****************************************************************************/
void spi_config(uint8_t mode, spi_data_order_t order, uint16_t freq);

/**
 * Get the mode of operation.
 *
 * \return the mode of operation.
 *****************************************************************************/
uint8_t spi_get_mode(void);

/**
 * Get the data order.
 *
 * \return the data order.
 *****************************************************************************/
spi_data_order_t spi_get_data_order(void);

/**
 * Get the frequency.
 *
 * \return the frequency.
 *****************************************************************************/
uint16_t spi_get_freq(void);

/**
 * Write to a SPI slave.
 *
 * \param txbuf the buffer with the data to write.
 * \param rxbuf the buffer where the read data will be read to.
 * \param buf_len the number of bytes to write.
 * \return true on success, false on error.
 *****************************************************************************/
bool spi_master_write(const uint8_t *txbuf, uint8_t *rxbuf, uint8_t buf_len);

/**
 * Set the Slave Select signal
 *
 * \param high if true set SS to high, otherwise set SS to low.
 *****************************************************************************/
void spi_set_ss(bool high);

#else // !FEATURE_SPI

#define spi_init()

#endif // FEATURE_SPI

#endif // !defined SPI_H
