
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#define NOP() do { __asm__ __volatile__ ("nop"); } while (0)

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define DISABLE_JTAG()  MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD); MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);

#define USB_PID_LEONARDO 0x0034
#define USB_PID_MICRO 0x0035
#define USB_VID 0x2341	// arduino LLC vid
#define USB_PID ARDUINO_MODEL_PID	// passed in by Makefile - 0x0034 for Leonardo, 0x0035 for MIcro

// the following have been changed for compatibility with the Micropendous
#define SELECT_USB_B		PORTE |= (1 << PE7); DDRE |= (1 << PE7);
#define DISABLE_VOLTAGE_TXRX	PORTE &= ~(1 << PE3); DDRE |= (1 << PE3);
#define BOARD_INIT()	DDRB |= (1<<1); DDRB |= (1<<0); CPU_PRESCALE(0); DISABLE_JTAG(); DISABLE_VOLTAGE_TXRX; SELECT_USB_B; 
#define LED0			PORTB &= ~(1<<1)
#define LED1			PORTB |= (1<<1)
#define TXLED0			NOP()
#define TXLED1			NOP()
#define RXLED0			NOP()
#define RXLED1			NOP()

#define TRANSFER_PGM		0x80
#define TRANSFER_RELEASE	0x40
#define TRANSFER_ZERO		0x20

void Transfer(u8 ep, const u8* data, int len);
void Recv(u8 ep, u8* dst, u8 len);
void Program(u8 ep, u16 page, u8 count);

#define CDC_ENABLED

#include "USBCore.h"
#include "USBDesc.h"


