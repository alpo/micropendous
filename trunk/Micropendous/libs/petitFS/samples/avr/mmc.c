/*-----------------------------------------------------------------------*/
/* PFF - Low level disk control module for ATtiny85     (C)ChaN, 2009    */
/*-----------------------------------------------------------------------*/

#include <avr/io.h>
#include "diskio.h"
#include "xitoa.h"


/* SPI control functions (defined in usi.S) */
void init_spi (void);
void xmit_spi (BYTE);
BYTE rcv_spi (void);


/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD9	(0x40+9)	/* SEND_CSD */
#define CMD10	(0x40+10)	/* SEND_CID */
#define CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */


/* Port Controls  (Platform dependent) */
#define SELECT()	PORTB &= ~0x08	/* MMC CS = L */
#define	DESELECT()	PORTB |=  0x08	/* MMC CS = H */


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

BYTE CardType;



/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
BYTE wait_ready (void)
{
	BYTE res;
	WORD tmr;


	rcv_spi();
	tmr = 2500;
	do
		res = rcv_spi();
	while (res != 0xFF && tmr);

	return res;
}



/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void release_spi (void)
{
	DESELECT();
	rcv_spi();
}


/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (
	BYTE cmd,		/* Command byte */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready */
	DESELECT();
	SELECT();
	if (wait_ready() != 0xFF) return 0xFF;

	/* Send command packet */
	xmit_spi(cmd);						/* Start + Command index */
	xmit_spi((BYTE)(arg >> 24));		/* Argument[31..24] */
	xmit_spi((BYTE)(arg >> 16));		/* Argument[23..16] */
	xmit_spi((BYTE)(arg >> 8));			/* Argument[15..8] */
	xmit_spi((BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	xmit_spi(n);

	/* Receive command response */
	n = 10;								/* Wait for a valid response in timeout of 10 attempts */
	do {
		res = rcv_spi();
	} while ((res & 0x80) && --n);

	return res;			/* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	BYTE n, cmd, ty, ocr[4];
	WORD tmr;


	init_spi();		/* Initialize USI */

	for (n = 100; n; n--) rcv_spi();	/* Dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {			/* Enter Idle state */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDHC */
			for (n = 0; n < 4; n++) ocr[n] = rcv_spi();		/* Get trailing return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* The card can work at vdd range of 2.7-3.6V */
				for (tmr = 25000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--) ;	/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (tmr && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = rcv_spi();
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 */
				}
			}
		} else {							/* SDSC or MMC */
			if (send_cmd(ACMD41, 0) <= 1) 	{
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 */
			}
			for (tmr = 25000; tmr && send_cmd(cmd, 0); tmr--) ;	/* Wait for leaving idle state */
			if (!tmr || send_cmd(CMD16, 512) != 0)			/* Set R/W block length to 512 */
				ty = 0;
		}
	}
	CardType = ty;
	release_spi();

	return ty ? 0 : STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read partial sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	void *dest,		/* Pointer to the destination object to put data */
	DWORD lba,		/* Start sector number (LBA) */
	WORD ofs,		/* Byte offset in the sector (0..511) */
	WORD cnt		/* Byte count (1..512), b15:destination flag */
)
{
	DRESULT res;
	BYTE *p, dev, rc;
	WORD cf, tmr;
	BYTE (*f)(BYTE);


	dev = (cnt & 0x8000) ? 1 : 0;	/* Destination type */
	cnt &= 0x7FFF;
	if (!cnt || ofs + cnt > 512) return RES_PARERR;

	if (!(CardType & CT_BLOCK)) lba *= 512;		/* Convert to byte address if needed */

	res = RES_ERROR;
	if (send_cmd(CMD17, lba) == 0) {		/* READ_SINGLE_BLOCK */

		tmr = 1000;
		do {							/* Wait for data packet in timeout of 200ms */
			rc = rcv_spi();
		} while (rc == 0xFF && --tmr);

		if (rc == 0xFE) {

			cf = 512 + 2 - ofs - cnt;

			while (ofs--) rcv_spi();	/* Skip top of sector */

			if (dev) {					/* Receive middle of the sector */
				f = dest;
				do
					res = f(rcv_spi());
				while (--cnt && res);
				cf += cnt;
			} else {
				p = dest;
				do
					*p++ = rcv_spi();
				while (--cnt);
			}

			do							/* Skip bottom of sector and CRC */
				rcv_spi();
			while (--cf);

			res = cnt ? RES_STRERR : RES_OK;
		}
	}

	release_spi();

	return cnt ? RES_ERROR : RES_OK;
}

