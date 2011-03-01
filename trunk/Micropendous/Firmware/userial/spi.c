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
#include "version.h"
#include "spi.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>


#if FEATURE_SPI

//***************************************************************************
//  TWI control register values
//***************************************************************************
#define SPI_PORT        PORTB                   //!< SPI PORT register
#define SPI_PIN         PINB                    //!< SPI PIN register
#define SPI_DDR         DDRB                    //!< SPI DDR register
#define SPI_MOSI        2                       //!< SPI MOSI pin
#define SPI_MISO        3                       //!< SPI MISO pin
#define SPI_SCK         1                       //!< SPI SCK pin
#define SPI_SS          0                       //!< SPI SS pin


void spi_init(void)
{
    unsigned char tmp;

    // Switch SCK, MOSI and SS pins to output mode.
    SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS);
    SPI_PORT |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS);

    // Enable MISO pull-up.
    SPI_PORT |= _BV(SPI_MISO);

    // Activate the SPI hardware.
    SPCR = _BV(SPE) | _BV(MSTR);
    // Clear status flags.
    tmp = SPSR;
    tmp = SPDR;
}

void spi_config(uint8_t mode, spi_data_order_t order, uint16_t freq)
{
    uint8_t divisor;

    // SPI Mode   CPOL   CPHA   Shift SCK-edge   Capture SCK-edge
    // 0          0      0      Falling          Rising
    // 1          0      1      Rising           Falling
    // 2          1      0      Rising           Falling
    // 3          1      1      Falling          Rising
    if (freq >= F_CPU /  2000) {
        divisor = 0x04;
    } else if (freq >= F_CPU /  4000) {
        divisor = 0x00;
    } else if (freq >= F_CPU /  8000) {
        divisor = 0x05;
    } else if (freq >= F_CPU / 16000) {
        divisor = 0x01;
    } else if (freq >= F_CPU / 32000) {
        divisor = 0x06;
    } else if (freq >= F_CPU / 64000) {
        divisor = 0x02;
    } else {
        divisor = 0x03;
    }
    SPCR = _BV(SPE) | _BV(MSTR) |
        (order ? _BV(DORD) : 0) |
        (mode & 0x02 ? _BV(CPOL) : 0) |
        (mode & 0x01 ? _BV(CPHA) : 0) |
        (divisor & 0x02 ? _BV(SPR1) : 0)|
        (divisor & 0x01 ? _BV(SPR0) : 0);
    if (divisor & 0x04) {
        SPSR |= _BV(SPI2X);
    } else {
        SPSR &= ~_BV(SPI2X);
    }
}

uint8_t spi_get_mode(void)
{
    uint8_t spcr = SPCR;

    return (spcr & _BV(CPOL) ? 0x02 : 0x00) | (spcr & _BV(CPHA) ? 0x01 : 0x00);
}

spi_data_order_t spi_get_data_order(void)
{
    return SPCR & _BV(DORD) ? spi_lsb_first : spi_msb_first;
}

uint16_t spi_get_freq(void)
{
    uint16_t freq;
    uint8_t divisor;
    uint8_t spcr = SPCR;

    divisor = (SPSR & _BV(SPI2X) ? 0x04 : 0x00) | (spcr & _BV(SPR1) ? 0x02 : 0x00) | (spcr & _BV(SPR0) ? 0x01 : 0x00);
    switch (divisor) {
        case 0x03:
            freq = F_CPU / 1000 / 128;
            break;
        case 0x02:
            freq = F_CPU / 1000 / 64;
            break;
        case 0x06:
            freq = F_CPU / 1000 / 32;
            break;
        case 0x01:
            freq = F_CPU / 1000 / 16;
            break;
        case 0x05:
            freq = F_CPU / 1000 /  8;
            break;
        case 0x00:
            freq = F_CPU / 1000 /  4;
            break;
        case 0x04:
        default:
            freq = F_CPU / 1000 /  2;
            break;
    }
    return freq;
}

bool spi_master_write(const uint8_t *txbuf, uint8_t *rxbuf, uint8_t buf_len)
{
    while (buf_len--) {
        SPDR = *txbuf++;
        loop_until_bit_is_set(SPSR, SPIF);
        *rxbuf++ = SPDR;
    }
    return true;
}

void spi_set_ss(bool high)
{
    if (high) {
        SPI_PORT |= _BV(SPI_SS);
    } else {
        SPI_PORT &= ~_BV(SPI_SS);
    }
}
#endif // FEATURE_SPI
