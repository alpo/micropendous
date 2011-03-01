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
#include "i2c.h"
#include "gpio.h"
#include "spi.h"
#include "adc.h"
#include "cbuf.h"
#include "command.h"
#include <avr/pgmspace.h>
#include <ctype.h>

#if FEATURE_I2C || FEATURE_GPIO || FEATURE_SPI || FEATURE_ADC
static int8_t xdigit_to_i(uint8_t c);
static char i_to_xdigit(uint8_t c);
#endif
#if FEATURE_I2C
static int c_to_i8(uint8_t c1, uint8_t c2);
#endif
static bool parse_error(cbf_t *tx_buf, char c, const char *str);
static void parse_discard_rx(cbf_t *rx_buf);
static inline bool parse_is_eol(uint8_t c) { return (c == '\r') || (c == '\n'); }
static inline bool parse_is_comment(uint8_t c) { return c == '#'; }
static inline bool parse_is_version(uint8_t c) { return c == 'V'; }
static inline bool parse_is_i2c(uint8_t c) { return c == 'I'; }
static inline bool parse_is_config(uint8_t c) { return c == 'C'; }
static inline bool parse_is_start(uint8_t c) { return c == 'S'; }
static inline bool parse_is_stop(uint8_t c) { return c == 'P'; }
static inline bool parse_is_read(uint8_t c) { return c == 'R'; }
static inline bool parse_is_write(uint8_t c) { return c == 'W'; }
static inline bool parse_is_clear(uint8_t c) { return c == 'X'; }
static inline bool parse_is_gpio(uint8_t c) { return c == 'G'; }
static inline bool parse_is_bitset(uint8_t c) { return c == 'B'; }
static inline bool parse_is_port(uint8_t c) { return c == 'A' || c == 'B'; }
static inline bool parse_is_portdir(uint8_t c) { return c == 'I' || c == 'O'; }
static inline bool parse_is_spi(uint8_t c) { return c == 'S'; }
static inline bool parse_is_adc(uint8_t c) { return c == 'A'; }
static inline bool parse_is_order(uint8_t c) { return c == 'L' || c == 'M'; }
static inline bool parse_is_highlow(uint8_t c) { return c == 'H' || c == 'L'; }
static inline bool parse_is_voltage(uint8_t c) { return c == 'V'; }
static bool parse_version(cbf_t *rx_buf, cbf_t *cmd_buf);
#if FEATURE_I2C
static bool parse_read_data(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_write_data(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_i2c(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_i2c_readwrite(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_i2c_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_i2c_clear(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
#endif
#if FEATURE_GPIO
static bool parse_gpio(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_gpio_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_gpio_read(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_gpio_bset(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
#endif
#if FEATURE_SPI
static bool parse_spi(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_spi_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_spi_read_write(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
#endif
#if FEATURE_ADC
static bool parse_adc(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_adc_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_adc_read(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
static bool parse_adc_voltage(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf);
#endif

/**
 * Convert a 4-bit integer value to its ASCII representation.
 *
 * \param c the value to convert.
 * \return the converted value or 'X' on error.
 *****************************************************************************/
char i_to_xdigit(uint8_t c)
{
    char ret;

    if (c <= 0x09) {
        ret = '0' + c;
    } else if (c <= 0x0f) {
        ret = 'A' + c - 10;
    } else {
        ret = 'X';
    }
    return ret;
}

/**
 * Convert a hexadecimal digit into an integer.
 *
 * \param c the character to convert.
 * \return the converted value or -1 on error.
 *****************************************************************************/
int8_t xdigit_to_i(uint8_t c)
{
    int ret;

    if (isdigit(c)) {
        ret = c - '0';
    } else if ('A' <= c && c <= 'F') {
        ret = c - 'A' + 10;
    } else {
        ret = -1;
    }
    return ret;
}

/**
 * Convert two hexadecimal digit into an integer.
 *
 * \param c1 the hi-nibble to convert.
 * \param c2 the lo-nibble to convert.
 * \return the converted value or -1 on error.
 *****************************************************************************/
int c_to_i8(uint8_t c1, uint8_t c2)
{
    int8_t tmp;

    c1 = tmp = xdigit_to_i(c1);
    if (tmp < 0) {
        return -1;
    }
    c2 = tmp = xdigit_to_i(c2);
    if (tmp < 0) {
        return -1;
    }
    return c1 << 4 | c2;
}

/**
 * Helper function to print an error message and to return false.
 *
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \param      c error character.
 * \param      str additional string describing the error.
 *****************************************************************************/
bool parse_error(cbf_t *tx_buf, char c, const char *str)
{
    cbf_put(tx_buf, c);
    if (str) {
        cbf_put(tx_buf, ':');
        cbf_put(tx_buf, ' ');
        while (pgm_read_byte(str)) {
            cbf_put(tx_buf, pgm_read_byte(str++));
        }
    }
    cbf_put(tx_buf, '\r');
    cbf_put(tx_buf, '\n');
    return false;
}

/**
 * Helper function to discard everything up to the end of line.
 *
 * \param[in] rx_buf the buffer containing the input command string.
 *****************************************************************************/
void parse_discard_rx(cbf_t *rx_buf)
{
    while (!cbf_isempty(rx_buf) && !parse_is_eol(cbf_peek(rx_buf))) {
        cbf_get(rx_buf);
    }
}

/**
 * Scans a receive buffer for a new message.
 *
 * \param[in]  rx_buf circular buffer that contains the input string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return     true if a complete message is in the buffer, false otherwise.
 *****************************************************************************/
bool cmd_scan(cbf_t *rx_buf, cbf_t *tx_buf)
{
    while (parse_is_eol(cbf_peek(rx_buf))) {
        cbf_get(rx_buf);
    }
    if ((cbf_find(rx_buf, '\r') > 0) || (cbf_find(rx_buf, '\n') > 0)) {
        return true;
    }
    if (cbf_isfull(rx_buf)) {
        cbf_init(rx_buf);
        parse_error(tx_buf, 'E', PSTR("garbage at beginning of line"));
    }
    return false;
}

/**
 * Parse the read input string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool cmd_parse(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    bool ret = false;

    cbf_init(cmd_buf);
    c = toupper(cbf_get(rx_buf));

    // Skip Carriage Return and New Line
    if (parse_is_eol(c)) {
        return true;
    }

    // Comment
    if (parse_is_comment(c)) {
        parse_discard_rx(rx_buf);
        ret = true;
    }
    // Version request
    else if (parse_is_version(c)) {
        ret = parse_version(rx_buf, cmd_buf);
    }
#if FEATURE_I2C
    // I2C commands
    else if (parse_is_i2c(c)) {
        ret = parse_i2c(rx_buf, cmd_buf, tx_buf);
    }
#endif
#if FEATURE_GPIO
    // GPIO commands
    else if (parse_is_gpio(c)) {
        ret = parse_gpio(rx_buf, cmd_buf, tx_buf);
    }
#endif
#if FEATURE_SPI
    // SPI commands
    else if (parse_is_spi(c)) {
        ret = parse_spi(rx_buf, cmd_buf, tx_buf);
    }
#endif
#if FEATURE_ADC
    // ADC commands
    else if (parse_is_adc(c)) {
        ret = parse_adc(rx_buf, cmd_buf, tx_buf);
    }
#endif
    else {
        ret = parse_error(tx_buf, 'E', PSTR("unknown command"));
    }

    if (!ret) {
        parse_discard_rx(rx_buf);
        return false;
    }

    c = cbf_get(rx_buf);
    if (!parse_is_eol(c)) {
        parse_discard_rx(rx_buf);
        return parse_error(tx_buf, 'E', PSTR("newline expected"));
    }
    return true;
}


/**
 * Parse a version request.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_version(cbf_t *rx_buf, cbf_t *cmd_buf)
{
    rx_buf = rx_buf;

    cbf_put(cmd_buf, 'V');
    return true;
}

/**
 * Parse a Read Command.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_read_data(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    int8_t data;

    data = xdigit_to_i(toupper(cbf_peek(rx_buf)));
    if (data < 0) {
        return parse_error(tx_buf, 'E', PSTR("invalid data"));
    } else {
        cbf_get(rx_buf);
        c = data << 4;
        data = xdigit_to_i(toupper(cbf_get(rx_buf)));
        if (data < 0) {
            return parse_error(tx_buf, 'E', PSTR("invalid data"));
        } else {
            c |= data;
        }
        cbf_put(cmd_buf, c);
    }
    return true;
}

/**
 * Parse Write Data
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_write_data(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    int8_t data;

    while (!cbf_isempty(rx_buf)) {
        c = toupper(cbf_peek(rx_buf));
        if (c == '\\') {
            cbf_get(rx_buf);
            cbf_put(cmd_buf, 1);
            cbf_put(cmd_buf, cbf_get(rx_buf));
        } else {
            data = xdigit_to_i(c);
            if (data >= 0) {
                cbf_get(rx_buf);
                c = data << 4;
                data = xdigit_to_i(toupper(cbf_get(rx_buf)));
                if (data < 0) {
                    return parse_error(tx_buf, 'E', PSTR("invalid data"));
                } else {
                    c |= data;
                }
                cbf_put(cmd_buf, 1);
                cbf_put(cmd_buf, c);
            } else {
                break;
            }
        }
    }
    return true;
}

#if FEATURE_I2C
/**
 * Parse an I2C command string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_i2c(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;

    cbf_put(cmd_buf, 'I');
    c = toupper(cbf_get(rx_buf));
    if (parse_is_config(c)) {
        return parse_i2c_config(rx_buf, cmd_buf, tx_buf);
    } else if (parse_is_start(c)) {
        return parse_i2c_readwrite(rx_buf, cmd_buf, tx_buf);
    } else if (parse_is_clear(c)) {
        return parse_i2c_clear(rx_buf, cmd_buf, tx_buf);
    } else {
        return parse_error(tx_buf, 'E', PSTR("invalid i2c command"));
    }
}

/**
 * Parse an I2C config string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_i2c_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    uint16_t freq;
    int8_t num = 0;

    freq = 0;
    while (!cbf_isempty(rx_buf) && !parse_is_eol(cbf_peek(rx_buf))) {
        c = toupper(cbf_get(rx_buf));
        num = xdigit_to_i(c);
        if (num < 0) {
            break;
        }
        freq = (freq << 4) | num;
    }
    if (num < 0) {
        return parse_error(tx_buf, 'E', PSTR("invalid frequency"));
    }

    cbf_put(cmd_buf, 'C');
    cbf_put(cmd_buf, freq >> 8);
    cbf_put(cmd_buf, freq & 0xff);

    return true;
}

/**
 * Parse an I2C bus clear command.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_i2c_clear(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    rx_buf = rx_buf;
    tx_buf = tx_buf;

    cbf_put(cmd_buf, 'X');

    return true;
}

/**
 * Parse the read input string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_i2c_readwrite(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    int address;

    do {
        // 'S' character read.
        cbf_put(cmd_buf, 'S');

        // Parse the address
        c = toupper(cbf_get(rx_buf));
        address = c_to_i8(c, toupper(cbf_get(rx_buf)));
        if (address < 0) {
            return parse_error(tx_buf, 'E', PSTR("address invalid"));
        }

        // Parse Read/Write byte
        c = toupper(cbf_get(rx_buf));
        if (parse_is_read(c)) {
            if ((address & 0x01) != 0x01) {
                return parse_error(tx_buf, 'E', PSTR("address invalid for R"));
            }
            cbf_put(cmd_buf, address);
            // Parse Read command
            if (!parse_read_data(rx_buf, cmd_buf, tx_buf)) {
                return false;
            }
        } else if (parse_is_write(c)) {
            if ((address & 0x01) != 0x00) {
                return parse_error(tx_buf, 'E', PSTR("address invalid for W"));
            }
            cbf_put(cmd_buf, address);
            // Parse Write command
            if (!parse_write_data(rx_buf, cmd_buf, tx_buf)) {
                return false;
            }
        } else {
            return parse_error(tx_buf, 'E', PSTR("[RW] expected"));
        }

        c = toupper(cbf_get(rx_buf));
    } while (parse_is_start(c));

    // Expect a 'P' character.
    if (!parse_is_stop(c)) {
        return parse_error(tx_buf, 'E', PSTR("P expected"));
    }
    cbf_put(cmd_buf, 'P');

    return true;
}
#endif

#if FEATURE_GPIO
/**
 * parse an gpio command string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_gpio(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;

    cbf_put(cmd_buf, 'G');
    c = toupper(cbf_get(rx_buf));
    if (parse_is_config(c)) {
        return parse_gpio_config(rx_buf, cmd_buf, tx_buf);
    } else if (parse_is_read(c)) {
        return parse_gpio_read(rx_buf, cmd_buf, tx_buf);
    } else if (parse_is_bitset(c)) {
        return parse_gpio_bset(rx_buf, cmd_buf, tx_buf);
    } else {
        return parse_error(tx_buf, 'E', PSTR("invalid gpio command"));
    }
}

/**
 * Parse an GPIO config command.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_gpio_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    uint8_t io_mask;

    cbf_put(cmd_buf, 'C');
    do {
        c = toupper(cbf_get(rx_buf));
        if (!parse_is_port(c)) {
            return parse_error(tx_buf, 'E', PSTR("invalid port"));
        }
        cbf_put(cmd_buf, c);
        io_mask = 0;
        for (uint8_t i = 0; i < 8; i++) {
            io_mask <<= 1;
            c = toupper(cbf_get(rx_buf));
            if (!parse_is_portdir(c)) {
                return parse_error(tx_buf, 'E', PSTR("no port direction"));
            }
            if (c == 'O') {
                io_mask |= 1;
            }
        }
        cbf_put(cmd_buf, io_mask);
        c = toupper(cbf_peek(rx_buf));
    } while (!cbf_isempty(rx_buf) && !parse_is_eol(c));
    return true;
}

/**
 * Parse an GPIO read command.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_gpio_read(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;

    cbf_put(cmd_buf, 'R');
    do {
        c = toupper(cbf_get(rx_buf));
        if (!parse_is_port(c)) {
            return parse_error(tx_buf, 'E', PSTR("invalid port"));
        }
        cbf_put(cmd_buf, c);
        c = toupper(cbf_peek(rx_buf));
    } while (!cbf_isempty(rx_buf) && !parse_is_eol(c));
    return true;
}

/**
 * Parse an GPIO set command.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_gpio_bset(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    uint8_t and_mask, or_mask, xor_mask;

    cbf_put(cmd_buf, 'B');
    do {
        c = toupper(cbf_get(rx_buf));
        if (!parse_is_port(c)) {
            return parse_error(tx_buf, 'E', PSTR("invalid port"));
        }
        cbf_put(cmd_buf, c);
        and_mask = or_mask = xor_mask = 0;
        for (uint8_t i = 0; i < 8; i++) {
            and_mask <<= 1;
            or_mask <<= 1;
            xor_mask <<= 1;
            c = toupper(cbf_get(rx_buf));
            if (c == '1') {
                or_mask |= 1;
            } else if (c == '0') {
                and_mask |= 1;
            } else if (c == 'F') {
                xor_mask |= 1;
            } else if (c == 'F') {
                xor_mask |= 1;
            } else if (c == 'X') {
            } else {
                return parse_error(tx_buf, 'E', PSTR("invalid bit value"));
            }
        }
        cbf_put(cmd_buf, ~and_mask);
        cbf_put(cmd_buf, or_mask);
        cbf_put(cmd_buf, xor_mask);
        c = toupper(cbf_peek(rx_buf));
    } while (!cbf_isempty(rx_buf) && !parse_is_eol(c));
    return true;
}
#endif

#if FEATURE_SPI
/**
 * parse an spi command string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_spi(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;

    cbf_put(cmd_buf, 'S');
    c = toupper(cbf_get(rx_buf));
    if (parse_is_config(c)) {
        return parse_spi_config(rx_buf, cmd_buf, tx_buf);
    } else if (parse_is_write(c)) {
        return parse_spi_read_write(rx_buf, cmd_buf, tx_buf);
    } else {
        return parse_error(tx_buf, 'E', PSTR("invalid spi command"));
    }
}

/**
 * parse an spi config string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_spi_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;
    int8_t mode;
    int8_t order;
    uint16_t freq;
    int8_t num = 0;
    bool query_only = false;

    if (parse_is_eol(cbf_peek(rx_buf))) {
        query_only = true;
    } else {
        mode = xdigit_to_i(toupper(cbf_get(rx_buf)));
        if (mode < 0 || mode > 3) {
            return parse_error(tx_buf, 'E', PSTR("invalid mode"));
        }
        order = toupper(cbf_get(rx_buf));
        if (!parse_is_order(order)) {
            return parse_error(tx_buf, 'E', PSTR("invalid order"));
        }
        order = order == 'M' ? spi_msb_first : spi_lsb_first;
        freq = 0;
        while (!cbf_isempty(rx_buf) && !parse_is_eol(cbf_peek(rx_buf))) {
            c = toupper(cbf_get(rx_buf));
            num = xdigit_to_i(c);
            if (num < 0) {
                break;
            }
            freq = (freq << 4) | num;
        }
        if (num < 0) {
            return parse_error(tx_buf, 'E', PSTR("invalid frequency"));
        }
    }

    cbf_put(cmd_buf, 'C');
    if (!query_only) {
        cbf_put(cmd_buf, mode);
        cbf_put(cmd_buf, order);
        cbf_put(cmd_buf, freq >> 8);
        cbf_put(cmd_buf, freq & 0xff);
    }

    return true;
}

/**
 * parse an spi read write string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_spi_read_write(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;

    cbf_put(cmd_buf, 'W');
    c = toupper(cbf_peek(rx_buf));
    if (parse_is_highlow(c)) {
        cbf_put(cmd_buf, c);
        cbf_get(rx_buf);
    }
    if (parse_is_eol(cbf_peek(rx_buf))) {
        return parse_error(tx_buf, 'E', PSTR("no data"));
    }
    if (!parse_write_data(rx_buf, cmd_buf, tx_buf)) {
        return false;
    }
    if (!parse_is_eol(cbf_peek(rx_buf))) {
        return parse_error(tx_buf, 'E', PSTR("garbage at the end of line"));
    }
    return true;
}
#endif

#if FEATURE_ADC
/**
 * parse an adc command string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_adc(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;

    cbf_put(cmd_buf, 'A');
    c = toupper(cbf_get(rx_buf));
    if (parse_is_config(c)) {
        return parse_adc_config(rx_buf, cmd_buf, tx_buf);
    } else if (parse_is_read(c)) {
        return parse_adc_read(rx_buf, cmd_buf, tx_buf);
    } else if (parse_is_voltage(c)) {
        return parse_adc_voltage(rx_buf, cmd_buf, tx_buf);
    } else {
        return parse_error(tx_buf, 'E', PSTR("invalid adc command"));
    }
}

/**
 * parse an adc config string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_adc_config(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    int16_t voltage;
    int8_t power;
    uint8_t c;

    cbf_put(cmd_buf, 'C');
    if (!cbf_isempty(rx_buf) && !parse_is_eol(cbf_peek(rx_buf))) {
        c = toupper(cbf_get(rx_buf));
        switch (c) {
            case 'I':
                cbf_put(cmd_buf, adc_int);
                break;
            case 'C':
                cbf_put(cmd_buf, adc_avcc);
                break;
            case 'F':
                cbf_put(cmd_buf, adc_aref);
                break;
            default:
                return parse_error(tx_buf, 'E', PSTR("invalid voltage reference"));
        }
        power = voltage = 0;
        if (!cbf_isempty(rx_buf) && !parse_is_eol(cbf_peek(rx_buf))) {
            while (!cbf_isempty(rx_buf) && !parse_is_eol(cbf_peek(rx_buf))) {
                c = toupper(cbf_get(rx_buf));
                if (c == '.') {
                    if (power != 0) {
                        return parse_error(tx_buf, 'E', PSTR("invalid voltage"));
                    }
                    power = 1;
                } else if ((c >= '0') && (c <= '9')) {
                    voltage = 10 * voltage + xdigit_to_i(c);
                    power = power ? power + 1 : 0;
                } else {
                    return parse_error(tx_buf, 'E', PSTR("invalid voltage"));
                }
                if (voltage > 999) {
                    return parse_error(tx_buf, 'E', PSTR("invalid voltage"));
                }
            }
            if (power == 0) {
                voltage *= 100;
            } else {
                while (power > 3) {
                    voltage /= 10;
                    power--;
                }
                while (power < 3) {
                    voltage *= 10;
                    power++;
                }
            }
        }
        if (voltage > 999) {
            return parse_error(tx_buf, 'E', PSTR("invalid voltage"));
        }
        cbf_put(cmd_buf, voltage >> 8);
        cbf_put(cmd_buf, voltage);
    }

    return true;
}

/**
 * parse an adc read string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_adc_read(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    int8_t channel;
    uint8_t c;

    cbf_put(cmd_buf, 'R');
    c = toupper(cbf_get(rx_buf));
    channel = xdigit_to_i(c);
    if (channel > ADC_NUMCHANNELS) {
        return parse_error(tx_buf, 'E', PSTR("invalid channel"));
    }
    cbf_put(cmd_buf, channel);

    return true;
}

/**
 * parse an adc voltage read string.
 *
 * \param[in]  rx_buf the buffer containing the input command string.
 * \param[out] cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool parse_adc_voltage(cbf_t *rx_buf, cbf_t *cmd_buf, cbf_t *tx_buf)
{
    int8_t channel;
    uint8_t c;

    cbf_put(cmd_buf, 'V');
    c = toupper(cbf_get(rx_buf));
    channel = xdigit_to_i(c);
    if (channel > ADC_NUMCHANNELS) {
        return parse_error(tx_buf, 'E', PSTR("invalid channel"));
    }
    cbf_put(cmd_buf, channel);

    return true;
}
#endif

/**
 * Execute a cmd_buf sequence.
 *
 * \param[in]  cmd_buf buffer conraining the output command string.
 * \param[out] tx_buf buffer contining the messages sent to the user.
 * \return true on success, false on error.
 *****************************************************************************/
bool cmd_exec(cbf_t *cmd_buf, cbf_t *tx_buf)
{
    uint8_t c;

    if (cbf_isempty(cmd_buf)) {
        return true;
    }
    c = cbf_get(cmd_buf);

    if (parse_is_version(c)) {
        cbf_put(tx_buf, c);
        for (uint8_t i = 0; i < sizeof(VERSION_STRING) - 1; i++) {
            cbf_put(tx_buf, VERSION_STRING[i]);
        }
    }
#if FEATURE_I2C
    else if (parse_is_i2c(c)) {
        uint8_t nbytes;
        c = cbf_get(cmd_buf);

        // I2C Config command
        if (parse_is_config(c)) {
            uint16_t freq;

            if (!cbf_isempty(cmd_buf)) {
                freq = (cbf_get(cmd_buf) << 8) | cbf_get(cmd_buf);
                if ((freq > 0) && (freq <= 800)) {
                    i2c_config(freq);
                }
            }
            freq = i2c_get_freq();

            cbf_put(tx_buf, 'I');
            cbf_put(tx_buf, 'C');
            cbf_put(tx_buf, i_to_xdigit((freq >> 12) & 0x0f));
            cbf_put(tx_buf, i_to_xdigit((freq >>  8) & 0x0f));
            cbf_put(tx_buf, i_to_xdigit((freq >>  4) & 0x0f));
            cbf_put(tx_buf, i_to_xdigit((freq >>  0) & 0x0f));
        } else if (parse_is_start(c)) {
            // I2C Read/Write command
            do {
                cbf_put(tx_buf, 'I');
                cbf_put(tx_buf, 'S');
                c = cbf_get(cmd_buf);
                if ((c & 0x01) == 0x01) {
                    // Read
                    if (!i2c_master_start(c, i2c_rd)) {
                        i2c_master_stop();
                        return parse_error(tx_buf, 'N', PSTR("Nack on rd address"));
                    }
                    cbf_put(tx_buf, 'A');
                    nbytes = cbf_get(cmd_buf);
                    while (nbytes > 0) {
                        if (!i2c_master_rx(&c, 1, nbytes > 1)) {
                            i2c_master_stop();
                            return parse_error(tx_buf, 'N', PSTR("Nack on rd byte"));
                        }
                        cbf_put(tx_buf, i_to_xdigit(c >> 4));
                        cbf_put(tx_buf, i_to_xdigit(c & 0x0f));
                        nbytes--;
                    }
                    c = cbf_get(cmd_buf);
                } else {
                    // Write
                    if (!i2c_master_start(c, i2c_wr)) {
                        i2c_master_stop();
                        return parse_error(tx_buf, 'N', PSTR("Nack on wr address"));
                    }
                    cbf_put(tx_buf, 'A');
                    c = cbf_get(cmd_buf);
                    while ((!parse_is_start(c)) && (!parse_is_stop(c))) {
                        nbytes = c;
                        while (nbytes > 0) {
                            c = cbf_get(cmd_buf);
                            if (i2c_master_tx(&c, 1)) {
                                cbf_put(tx_buf, 'A');
                            } else {
                                i2c_master_stop();
                                return parse_error(tx_buf, 'N', PSTR("Nack on wr byte"));
                            }
                            nbytes--;
                        }
                        c = cbf_get(cmd_buf);
                    }
                }
            } while (parse_is_start(c));

            if (!parse_is_stop(c)) {
                i2c_master_stop();
                return parse_error(tx_buf, 'F', PSTR("P expected"));
            }
            i2c_master_stop();
            cbf_put(tx_buf, 'P');
        } else if (parse_is_clear(c)) {
            i2c_bus_clear();
            cbf_put(tx_buf, 'I');
            cbf_put(tx_buf, 'X');
        } else {
            return parse_error(tx_buf, 'F', PSTR("C or S expected after I"));
        }
    }
#endif
#if FEATURE_GPIO
    else if (parse_is_gpio(c)) {
        cbf_put(tx_buf, 'G');
        c = cbf_get(cmd_buf);

        // GPIO Config command
        if (parse_is_config(c)) {
            uint8_t io_mask;
            uint8_t port;

            cbf_put(tx_buf, 'C');
            while (!cbf_isempty(cmd_buf)) {
                port = cbf_get(cmd_buf);
                io_mask = cbf_get(cmd_buf);
                if (!gpio_config(port, io_mask)) {
                    return parse_error(tx_buf, 'F', PSTR("no such port"));
                }
                cbf_put(tx_buf, port);
                io_mask = gpio_get_config(port);
                for (uint8_t i = 0; i < 8; i++) {
                    cbf_put(tx_buf, io_mask & 0x80 ? 'O' : 'I');
                    io_mask <<= 1;
                }
            }
        } else if (parse_is_read(c)) {
            // GPIO Read
            uint8_t port;

            cbf_put(tx_buf, 'R');
            while (!cbf_isempty(cmd_buf)) {
                port = cbf_get(cmd_buf);
                c = gpio_read(port);
                cbf_put(tx_buf, port);
                cbf_put(tx_buf, i_to_xdigit(c >> 4));
                cbf_put(tx_buf, i_to_xdigit(c & 0x0f));
            }
        } else if (parse_is_bitset(c)) {
            // GPIO Set
            uint8_t port;
            uint8_t and_mask, or_mask, xor_mask;

            cbf_put(tx_buf, 'B');
            while (!cbf_isempty(cmd_buf)) {
                port = cbf_get(cmd_buf);
                and_mask = cbf_get(cmd_buf);
                or_mask = cbf_get(cmd_buf);
                xor_mask = cbf_get(cmd_buf);
                if (!gpio_bitset(port, and_mask, or_mask, xor_mask)) {
                    return parse_error(tx_buf, 'F', PSTR("no such port"));
                }
                cbf_put(tx_buf, port);
                and_mask = gpio_read(port);
                for (uint8_t i = 0; i < 8; i++) {
                    cbf_put(tx_buf, and_mask & 0x80 ? '1' : '0');
                    and_mask <<= 1;
                }
            }
        } else {
            return parse_error(tx_buf, 'F', PSTR("C or R or S expected after G"));
        }
    }
#endif
#if FEATURE_SPI
    else if (parse_is_spi(c)) {
        uint8_t nbytes;
        // SPI commands
        cbf_put(tx_buf, 'S');
        c = cbf_get(cmd_buf);

        if (parse_is_config(c)) {
            // SPI Config command
            int8_t mode;
            int8_t order;
            uint16_t freq;

            if (!cbf_isempty(cmd_buf)) {
                mode = cbf_get(cmd_buf);
                order = cbf_get(cmd_buf);
                freq = (cbf_get(cmd_buf) << 8) | cbf_get(cmd_buf);
                spi_config(mode, order, freq);
            }
            mode = spi_get_mode();
            order = spi_get_data_order();
            freq = spi_get_freq();

            cbf_put(tx_buf, 'C');
            cbf_put(tx_buf, i_to_xdigit(mode));
            cbf_put(tx_buf, order == spi_msb_first ? 'M' : 'L');
            cbf_put(tx_buf, i_to_xdigit((freq >> 12) & 0x0f));
            cbf_put(tx_buf, i_to_xdigit((freq >>  8) & 0x0f));
            cbf_put(tx_buf, i_to_xdigit((freq >>  4) & 0x0f));
            cbf_put(tx_buf, i_to_xdigit((freq >>  0) & 0x0f));
        } else if (parse_is_write(c)) {
            // SPI Write command
            bool highlow = false;
            bool pol = true;

            cbf_put(tx_buf, 'W');
            c = cbf_get(cmd_buf);
            if (parse_is_highlow(c)) {
                highlow = true;
                pol = c == 'H';
                spi_set_ss(pol);
                c = cbf_get(cmd_buf);
            }
            while (!cbf_isempty(cmd_buf)) {
                nbytes = c;
                while (nbytes > 0) {
                    c = cbf_get(cmd_buf);
                    if (spi_master_write(&c, &c, 1)) {
                        cbf_put(tx_buf, i_to_xdigit((c >>  4) & 0x0f));
                        cbf_put(tx_buf, i_to_xdigit((c >>  0) & 0x0f));
                    } else {
                        return parse_error(tx_buf, 'F', PSTR("SPI write failed"));
                    }
                    nbytes--;
                }
                c = cbf_get(cmd_buf);
            }
            if (highlow) {
                spi_set_ss(!pol);
            }
        } else {
            return parse_error(tx_buf, 'F', PSTR("C or W expected after S"));
        }
    }
#endif
#if FEATURE_ADC
    else if (parse_is_adc(c)) {
    	// ADC command
    	cbf_put(tx_buf, 'A');
    	c = cbf_get(cmd_buf);

        // ADC read command
        if (parse_is_config(c)) {
            adc_cfg_t adc_cfg;

            cbf_put(tx_buf, 'C');

            if (!cbf_isempty(cmd_buf)) {
                adc_cfg.ref_type = cbf_get(cmd_buf);
                adc_cfg.ref_v = (cbf_get(cmd_buf) << 8) | cbf_get(cmd_buf);
                adc_config(&adc_cfg);
            }

            adc_get_config(&adc_cfg);
            switch (adc_cfg.ref_type) {
                default:
                case adc_int:
                    cbf_put(tx_buf, 'I');
                    break;
                case adc_avcc:
                    cbf_put(tx_buf, 'C');
                    break;
                case adc_aref:
                    cbf_put(tx_buf, 'F');
                    break;
            }
            cbf_put(tx_buf, i_to_xdigit(adc_cfg.ref_v / 100));
            cbf_put(tx_buf, '.');
            adc_cfg.ref_v = adc_cfg.ref_v % 100;
            cbf_put(tx_buf, i_to_xdigit(adc_cfg.ref_v / 10));
            adc_cfg.ref_v = adc_cfg.ref_v % 10;
            cbf_put(tx_buf, i_to_xdigit(adc_cfg.ref_v));
    	} else if (parse_is_read(c) || parse_is_voltage(c)) {
            uint8_t channel;
            uint8_t cmd;
            int16_t result;

            cmd = c;
            cbf_put(tx_buf, cmd);
            channel = cbf_get(cmd_buf);
            result = adc_read(channel);
            if (cmd == 'R') {
                cbf_put(tx_buf, i_to_xdigit((result >> 12) & 0x0f));
                cbf_put(tx_buf, i_to_xdigit((result >> 8) & 0x0f));
                cbf_put(tx_buf, i_to_xdigit((result >> 4) & 0x0f));
                cbf_put(tx_buf, i_to_xdigit(result & 0x0f));
            } else {
                adc_cfg_t adc_cfg;
                uint16_t divisor;
                adc_get_config(&adc_cfg);
                result = ((uint32_t)adc_cfg.ref_v * (uint32_t)result + 512) / 1024UL;
                divisor = 100;
                cbf_put(tx_buf, i_to_xdigit(result / divisor));
                cbf_put(tx_buf, '.');
                do {
                    result = result % divisor;
                    divisor /= 10;
                    cbf_put(tx_buf, i_to_xdigit(result / divisor));
                } while (divisor >= 10);
            }
    	}
    }
#endif
    else {
        return parse_error(tx_buf, 'F', PSTR("unknown command"));
    }

    cbf_put(tx_buf, '\r');
    cbf_put(tx_buf, '\n');

    return true;
}
