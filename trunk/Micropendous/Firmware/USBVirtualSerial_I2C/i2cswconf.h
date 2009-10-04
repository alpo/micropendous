/*! \file lcdconf.h \brief Character LCD driver configuration. */
//*****************************************************************************
//
// File Name	: 'lcdconf.h'
// Title		: Character LCD driver for HD44780/SED1278 displays
//					(usable in mem-mapped, or I/O mode)
// Author		: Pascal Stang - Copyright (C) 2000-2002
// Created		: 11/22/2000
// Revised		: 4/30/2002
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef I2CSWCONF_H
#define I2CSWCONF_H

#define SDAPORT	PORTD
#define SDADDR	DDRD
#define SDA	1
#define SDAPIN	PIND

#define SCLPORT	PORTD
#define SCLDDR	DDRD
#define SCL	0
#define SCLPIN	PIND

#endif  // I2CSWCONF_H
