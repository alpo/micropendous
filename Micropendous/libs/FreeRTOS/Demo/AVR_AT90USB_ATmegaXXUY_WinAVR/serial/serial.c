/*
    FreeRTOS V6.0.2 - Copyright (C) 2010 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS eBook                                  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*
Changes from V1.2.3

	+ The function xPortInitMinimal() has been renamed to 
	  xSerialPortInitMinimal() and the function xPortInit() has been renamed
	  to xSerialPortInit().

Changes from V2.0.0

	+ Delay periods are now specified using variables and constants of
	  portTickType rather than unsigned long.
	+ xQueueReceiveFromISR() used in place of xQueueReceive() within the ISR.

Changes from V2.6.0

	+ Replaced the inb() and outb() functions with direct memory
	  access.  This allows the port to be built with the 20050414 build of
	  WinAVR.

Changes from V6.0.2

	+ Adapted for USB AVRs and other modern AVRs by Opendous Inc.
*/

/* BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER. */


#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "serial.h"
#include <LUFA/Drivers/Peripheral/Serial.h>

/* register redefinitions */
#include "avr_registers.h"

#define serBAUD_DIV_CONSTANT			( ( unsigned long ) 16 )

/* Constants for writing to UCSRB. */
#define serRX_INT_ENABLE				(1 << RXCIE1)
#define serRX_ENABLE					(1 << RXEN1)
#define serTX_ENABLE					(1 << TXEN1)
#define serTX_INT_ENABLE				(1 << UDRIE1)

/* Constants for writing to UCSRC. */
#define serEIGHT_DATA_BITS				((1 << UCSZ11) | (1 << UCSZ10))

static xQueueHandle xRxedChars; 
static xQueueHandle xCharsForTx; 

#define vInterruptOn()	{ UCSR1B |= serTX_INT_ENABLE; }
#define vInterruptOff()	{ UCSR1B &= ~serTX_INT_ENABLE; }

/*-----------------------------------------------------------*/

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
	//unsigned long ulBaudRateCounter;
	//unsigned char ucByte;


	// Create the queues used by the com test task.
	xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );

	Serial_Init(9600, false);


/*
	portENTER_CRITICAL();
	{
		// Create the queues used by the com test task.
		xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
		xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );

		// Calculate baud rate register value from the equation in the datasheet
		ulBaudRateCounter = ( configCPU_CLOCK_HZ / ( serBAUD_DIV_CONSTANT * ulWantedBaud ) ) - ( unsigned long ) 1;

		// Set the baud rate.
		ucByte = ( unsigned char ) ( ulBaudRateCounter & ( unsigned long ) 0xff );	
		UBRR1L = ucByte;

		ulBaudRateCounter >>= ( unsigned long ) 8;
		ucByte = ( unsigned char ) ( ulBaudRateCounter & ( unsigned long ) 0xff );	
		UBRR1H = ucByte;

		// Enable the Rx interrupt.  The Tx interrupt will get enabled
		// later. Also enable the Rx and Tx.
		UCSR1B = ( serRX_INT_ENABLE | serRX_ENABLE | serTX_ENABLE );

		// Set the data bits to 8
		UCSR1C = serEIGHT_DATA_BITS;
	}
	portEXIT_CRITICAL();
*/
	// Unlike other ports, this serial code does not allow for more than one
	// com port.  We therefore don't return a pointer to a port structure and can
	// instead just return NULL.
	return NULL;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime )
{
	/* Only one port is supported. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime )
{
	/* Only one port is supported. */
	( void ) pxPort;

	/* Return false if after the block time there is no room on the Tx queue. */
	if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) != pdPASS )
	{
		return pdFAIL;
	}

	vInterruptOn();

	return pdPASS;
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
unsigned char ucByte;

	/* The parameter is not used. */
	( void ) xPort;

	/* Turn off the interrupts.  We may also want to delete the queues and/or
	re-install the original ISR. */

	portENTER_CRITICAL();
	{
		vInterruptOff();
		ucByte = UCSR1B;
		ucByte &= ~serRX_INT_ENABLE;
		UCSR1B = ucByte;
	}
	portEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

ISR(USART1_RX_vect, ISR_BLOCK)
{
signed char cChar;
signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Get the character and post it on the queue of Rxed characters.
	If the post causes a task to wake force a context switch as the woken task
	may have a higher priority than the task we have interrupted. */
	cChar = UDR;
//	cChar = Serial_RxByte();

	xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );

	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/

ISR(USART1_UDRE_vect, ISR_BLOCK)
{
signed char cChar, cTaskWoken;

	if( xQueueReceiveFromISR( xCharsForTx, &cChar, &cTaskWoken ) == pdTRUE )
	{
		/* Send the next character queued for Tx. */
		UDR = cChar;
//		Serial_TxByte(cChar);
	}
	else
	{
		/* Queue empty, nothing to send. */
		vInterruptOff();
	}
}

