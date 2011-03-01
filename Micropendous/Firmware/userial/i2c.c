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
#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay_basic.h>

#if FEATURE_I2C
#define I2C_INIT_FREQ   100 // 100kHz

#define USE_HW_I2C  (BOARD == USERIAL_BOARD)



#if USE_HW_I2C
//***************************************************************************
//  TWI control register values
//***************************************************************************
#define TWICR_TWEN          (_BV(TWEN))                 //!< TWI enable
#define TWICR_TWIE          (_BV(TWIE))                 //!< interrupts enabled
#define TWICR_TWEA          (_BV(TWEA))                 //!< next I2C activity will be acknowledged
#define TWICR_TWSTA         (_BV(TWSTA))                //!< set the start bit
#define TWICR_TWSTO         (_BV(TWSTO))                //!< set the stop bit
#define TWICR_TWINT         (_BV(TWINT))                //!< clear onlythe TWINT bit
//#define TWICR_TWINT_TWEA    (_BV(TWINT)|_BV(TWEA))      //!< TWCR_TWINT and TWCR_TWEA

#else

#define SDA             4        // SDA Port D, Pin 4
#define SCL             5        // SCL Port D, Pin 5
#define SDA_DDR         DDRD
#define SCL_DDR         DDRD
#define SDA_OUT         PORTD
#define SCL_OUT         PORTD
#define SDA_IN          PIND
#define SCL_IN          PIND

#define sda_low()       (SDA_DDR |= _BV(SDA))
#define scl_low()       (SCL_DDR |= _BV(SCL))
#define sda_hi()        (SDA_DDR &= ~_BV(SDA))
#define scl_hi()        (SCL_DDR &= ~_BV(SCL))

#define T2_TICKS        (F_CPU/(I2C_INIT_FREQ * 1000UL * 3UL))
#define delay_T2()      (_delay_loop_1(T2_TICKS > 0 ? T2_TICKS : 1))

static bool hold_bus;

static void i2c_write_byte(uint8_t data);
static uint8_t i2c_read_bit(void);
#endif  // USE_HW_I2C



#if USE_HW_I2C
/**
 * Get the TWPS (prescaler) bits for the TWI
 *
 * \param freq I2C master frequency in kHz.
 *****************************************************************************/
static inline uint8_t i2c_get_twps(uint16_t freq) __attribute__((const));


/**
 * Calculates the TWBR (baudrate) bits for the TWI
 *
 * \param freq I2C master frequency in kHz.
 * \param twps the selected prescaler bits.
 *****************************************************************************/
static inline uint8_t i2c_get_twbr(uint16_t freq, uint8_t twps) __attribute__((const));


uint8_t i2c_get_twps(uint16_t freq)
{
    uint8_t ps;
    uint16_t val = F_CPU / (0xffUL * 2000UL);

    ps = 0;
    while (freq <= val) {
        ps++;
        val /= 4;
    }
    return ps;
}


uint8_t i2c_get_twbr(uint16_t freq, uint8_t twps)
{
    return (((F_CPU / 2000) / freq) - 8) / (1 << (twps *2));
}
#endif // USE_HW_I2C


void i2c_init(void)
{
    #if USE_HW_I2C
    TWCR = 0;
    i2c_config(I2C_INIT_FREQ);
    TWDR = 0xFF;
    TWCR = TWICR_TWEN;
    #else
    SDA_DDR &= ~_BV(SDA);       // release SDA
    SCL_DDR &= ~_BV(SCL);       // release SCL
    SDA_OUT &= ~_BV(SDA);
    SCL_OUT &= ~_BV(SCL);
    hold_bus = false;
    #endif
}


void i2c_bus_clear(void)
{
    i2c_master_start(0, i2c_rd);
    i2c_master_stop();
}


void i2c_config(uint16_t freq)
{
    #if USE_HW_I2C
    uint8_t twps;

    twps = i2c_get_twps(freq);
    TWBR = i2c_get_twbr(freq, twps);
    TWSR = twps & 0x03;
    #else
    (void)freq;     // frequency not used
    #endif
}


uint16_t i2c_get_freq(void)
{
    #if USE_HW_I2C
    uint8_t twps = 1 << (2 * (TWSR & 0x03));
    uint16_t freq = F_CPU / 1000UL;
    return freq / (16 + 2*TWBR * twps);
    #else
    return I2C_INIT_FREQ;   // default frequency
    #endif
}


bool i2c_master_start(uint8_t address, i2c_rdwr_t i2c_rd_wr)
{
    #if USE_HW_I2C
    bool error = false;
    uint8_t twsr;

    TWCR = TWICR_TWEN | TWICR_TWINT | TWICR_TWSTA;
    while ((TWCR & TWICR_TWINT) == 0);

    twsr = TWSR & TW_STATUS_MASK;
    if ((twsr != TW_START) && (twsr != TW_REP_START)) {
        error = true;
    } else {
        TWDR = (address & 0xfe) | i2c_rd_wr;
        TWCR = TWICR_TWEN | TWICR_TWINT;
        while ((TWCR & TWICR_TWINT) == 0);

        twsr = TWSR & TW_STATUS_MASK;
        if (i2c_rd_wr == i2c_rd) {
            error = twsr != TW_MR_SLA_ACK;
        } else {
            error = twsr != TW_MT_SLA_ACK;
        }
    }
    return !error;
    #else
    delay_T2();
    if (hold_bus) {
        sda_hi();
        delay_T2();
        scl_hi();
        delay_T2();
    }
    sda_low();
    delay_T2();
    scl_low();

    hold_bus = true;
    i2c_write_byte((address & 0xfe) | i2c_rd_wr);
    return i2c_read_bit() == 0;
    #endif
}


void i2c_master_stop(void)
{
    #if USE_HW_I2C
    TWCR = TWICR_TWEN | TWICR_TWINT | TWICR_TWSTO;
    #else
    delay_T2();
    scl_low();
    sda_low();
    delay_T2();
    scl_hi();
    delay_T2();
    sda_hi();
    hold_bus = false;
    #endif
}


#if !USE_HW_I2C
void i2c_write_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) {
        delay_T2();
        if (data & 0x80) {
            sda_hi();
        } else {
            sda_low();
        }
        scl_hi();
        delay_T2();
        scl_low();

        data <<= 1;
    }
}
#endif // !USE_HW_I2C


bool i2c_master_tx(const uint8_t *buf, uint8_t buf_len)
{
    bool error = false;

    #if USE_HW_I2C
    while ((buf_len > 0) && !error) {
        TWDR = *buf++;
        TWCR = TWICR_TWEN | TWICR_TWINT;
        while ((TWCR & TWICR_TWINT) == 0);
        error = (TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK;
        buf_len--;
    }
    #else
    while ((buf_len > 0) && !error) {
        i2c_write_byte(*buf++);
        error = i2c_read_bit() != 0;
        buf_len--;
    }
    #endif
    return !error;
}


#if !USE_HW_I2C
uint8_t i2c_read_bit(void)
{
    uint8_t bit;

    delay_T2();
    sda_hi();
    scl_hi();

    // wait for slave (clock stretching)
    do {
        delay_T2();
    } while ((SCL_IN & _BV(SCL)) == 0);
    bit = SDA_IN & _BV(SDA) ? 1 : 0;
    delay_T2();
    scl_low();
    return bit;
}
#endif // !USE_HW_I2C


bool i2c_master_rx(uint8_t *buf, uint8_t buf_len, bool ack)
{
    bool error = false;
    bool send_ack;
    #if USE_HW_I2C
    uint8_t twsr;

    while ((buf_len > 0) && !error) {
        send_ack = (buf_len > 1) || ack;
        if (send_ack) {
            TWCR = TWICR_TWEN | TWICR_TWINT | TWICR_TWEA;
        } else {
            TWCR = TWICR_TWEN | TWICR_TWINT;
        }
        while ((TWCR & TWICR_TWINT) == 0);
        *buf++ = TWDR;
        twsr = TWSR & TW_STATUS_MASK;
        if (send_ack) {
            error = twsr != TW_MR_DATA_ACK;
        } else {
            error = twsr != TW_MR_DATA_NACK;
        }
        buf_len--;
    }
    #else
    uint8_t data;

    while ((buf_len > 0) && !error) {
        send_ack = (buf_len > 1) || ack;
        data = 0;
        for (uint8_t i = 0; i < 8; i++) {
            data = (data << 1) | i2c_read_bit();
        }
        *buf++ = data;

        delay_T2();
        if (send_ack) {
            sda_low();
        } else {
            sda_hi();
        }
        delay_T2();
        scl_hi();
        delay_T2();
        scl_low();
        sda_hi();

        buf_len--;
    }
    #endif
    return !error;
}
#endif // FEATURE_I2C && BOARD == USERIAL_BOARD
