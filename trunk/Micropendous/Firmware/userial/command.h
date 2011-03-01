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
 * Command parser.
 *****************************************************************************/
#ifndef COMMAND_H
#define COMMAND_H
#include <stdint.h>
#include <stdbool.h>
#include "cbuf.h"


/**
 * Scans a receive buffer for a new message.
 *
 * \param[in]  rx_buf circular buffer that contains the input string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return     true if a complete message is in the buffer, false otherwise.
 *****************************************************************************/
bool cmd_scan(cbf_t *rx_buf, cbf_t *tx_buf);

/**
 * Parse a command line.
 *
 * \param rx_buf[in] circular buffer that contains the input string.
 * \param cmd_buf[out] circular buffer that will be filled with the commands.
 * \param tx_buf[out] circular buffer that will contain the error message.
 * \return true on success, false on failure.
 *****************************************************************************/
bool cmd_parse(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);

/**
 * Executed a parsed command.
 *
 * \param cmd_buf[in] circular buffer that will be filled with the commands.
 * \param tx_buf[out] circular buffer that will contain the error message.
 * \return true on success, false on failure.
 *****************************************************************************/
bool cmd_exec(cbf_t *cmd_buf, cbf_t *tx_buf);

#endif  // COMMAND_H
