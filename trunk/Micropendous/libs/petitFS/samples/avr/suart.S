;---------------------------------------------------------------------------;
; Software implemented UART via an ISP cable.
; Written by ChaN (http://elm-chan.org/)
;---------------------------------------------------------------------------;
;
;            1MHz  2MHz  4MHz  6MHz  8MHz  10MHz  12MHz  16MHz  20MHz
;   2.4kbps   138     -     -     -     -      -      -      -      -
;   4.8kbps    68   138     -     -     -      -      -      -      -
;   9.6kbps    33    68   138   208     -      -      -      -      -
;  19.2kbps     -    33    68   102   138    173    208      -      -
;  38.4kbps     -     -    33    50    68     85    102    138    172
;  57.6kbps     -     -    21    33    44     56     68     91    114
; 115.2kbps     -     -     -     -    21     27     33     44     56

.nolist
#include <avr/io.h>
.list


#define _SREG	_SFR_IO_ADDR(SREG)
#define _DDRB	_SFR_IO_ADDR(DDRB)
#define _PINB	_SFR_IO_ADDR(PINB)
#define _PORTB	_SFR_IO_ADDR(PORTB)
#define	BPS	68	/* Bit delay. (see above) */


;---------------------------------------------------------------------------;
; Transmit a byte in serial format of N81
;
;Prototype: void xmit (uint8_t data);
;Size: 16 words

.global xmit
.func xmit
xmit:
	in	r0, _SREG	;Save flags

	com	r24		;C = start bit
	ldi	r25, 10		;Bit counter
	ldi	r23, BPS*2-1	;Pre idle time
	cli			;Start critical section
	rjmp	2f
1:	ldi	r23, BPS-1	;----- Bit transferring loop 
2:	dec	r23     	;Wait for a bit time
	brne	2b		;/
	brcs	3f		;Pull/Release TxRx pin
	cbi	_DDRB, 4	;
3:	brcc	4f		;
	sbi	_DDRB, 4	;/
4:	lsr	r24     	;Get next bit into C
	dec	r25     	;All bits sent?
	brne	1b	     	;  no, coutinue

	out	_SREG, r0	;End of critical section
	ret
.endfunc


;---------------------------------------------------------------------------;
; Receive a byte
;
;Prototype: uint8_t rcvr (void);
;Size: 19 words

.global rcvr
.func rcvr
rcvr:
	in	r0, _SREG	;Save flags

	ldi	r24, 0x80	;Receiving shift reg
	cli			;Start critical section

1:	sbis	_PINB, 4	;Wait for rising edge on TxRx pin
	rjmp	1b
2:	sbic	_PINB, 4	;Wait for falling edge on TxRx pin
	rjmp	2b
	ldi	r25, BPS/2	;Wait for half bit time
3:	dec	r25
	brne	3b

4:	ldi	r25, BPS	;----- Bit receiving loop
5:	dec	r25     	;Wait for a bit time
	brne	5b		;/
	lsr	r24     	;Next bit
	sbic	_PINB, 4	;Get a bit into r24.7
	ori	r24, 0x80
	brcc	4b	     	;All bits received?  no, continue

	out	_SREG, r0	;End of critical section
	ret
.endfunc

