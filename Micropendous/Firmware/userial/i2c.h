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
 * I2C driver.
 *****************************************************************************/
#ifndef I2C_H
#define I2C_H

#include "version.h"
#include <stdbool.h>
#include <stdint.h>

#if FEATURE_I2C

/**
 * I2C read or write status flags
 *****************************************************************************/
typedef enum {
    i2c_wr,     //!< the following operation is a write operation.
    i2c_rd,     //!< the following operation is a read operation.
} i2c_rdwr_t;


/**
 * Initialise the I2C hardware.
 *****************************************************************************/
void i2c_init(void);


/**
 * Clear the I2C bus.
 *****************************************************************************/
void i2c_bus_clear(void);


/**
 * Configure the I2C hardware.
 *
 * \param freq the clock frequency in kHz of the I2C master.
 *****************************************************************************/
void i2c_config(uint16_t freq);


/**
 * Get the I2C master frequency.
 *
 * \return the I2C master frequency in kHz.
 *****************************************************************************/
uint16_t i2c_get_freq(void);


/**
 * Put a Start Condition on the bus.
 *
 * \param address the I2C slave address (left aligned).
 * \param i2c_rd_wr indicates if the following operation is a read or write operation.
 * \return true on success, false on error.
 *****************************************************************************/
bool i2c_master_start(uint8_t address, i2c_rdwr_t i2c_rd_wr);


/**
 * Put a Stop Condition on the bus.
 *****************************************************************************/
void i2c_master_stop(void);


/**
 * Write to a I2C slave.
 *
 * \param buf the buffer with the data to write.
 * \param buf_len the number of bytes to write.
 * \return true on success, false on error.
 *****************************************************************************/
bool i2c_master_tx(const uint8_t *buf, uint8_t buf_len);


/**
 * Read from a I2C slave.
 *
 * \param buf the buffer that store the read bytes.
 * \param buf_len the number of bytes in the buffer.
 * \param ack if true, send an ack (transmission contines) after the last byte
 *            otherwise send a nack (transmission ends).
 * \return true on success, false on error.
 *****************************************************************************/
bool i2c_master_rx(uint8_t *buf, uint8_t buf_len, bool ack);


#else // !FEATURE_I2C

#define i2c_init()

#endif // FEATURE_I2C
#endif // !defined I2C_H
