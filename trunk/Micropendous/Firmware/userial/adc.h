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
#ifndef ADC_H
#define ADC_H
#include "version.h"
#include <stdbool.h>
#include <stdint.h>


#if FEATURE_ADC

/**
 * \def ADC_NUMCHANNELS
 * The number of channels supported.
 *****************************************************************************/
#define ADC_NUMCHANNELS 3


/**
 * The ADC config structure
 *****************************************************************************/
 typedef struct {
     enum {
         adc_aref,              //!< AREF, Internal Vref turned off
         adc_avcc,              //!< AVcc with external capacitor on AREF pin
         adc_int                //!< Internal 2.56V Voltage Reference with external capacitor on AREF pin
     } ref_type;                //!< type of reference voltage
     uint16_t ref_v;            //!< referebce voltage level (x100)
 } adc_cfg_t;


/**
 * Set the ADC configuration
 *
 * \param cfg pointer to an ADC config structure
 *****************************************************************************/
void adc_config(adc_cfg_t *cfg);


/**
 * Get the ADC configuration
 *
 * \param cfg pointer to an ADC config structure
 *****************************************************************************/
void adc_get_config(adc_cfg_t *cfg);


/**
 * Setup the ADC hardware
 *****************************************************************************/
void adc_init(void);

/**
 * Read a value from a ADC channel.
 *
 * \param channel the ADC channel.
 * \return the the ADC value.
 *****************************************************************************/
uint16_t adc_read(uint8_t channel);


#else // !FEATURE_ADC

#define adc_init()

#endif // FEATURE_ADC

#endif // !defined ADC_H
