/*---------------------------------------------------------------*/
/* Petit FAT file system module test program R0.01 (C)ChaN, 2009 */
/*---------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "diskio.h"
#include "pff.h"
#include "xitoa.h"
#include "suart.h"

#define	SYSCLK		8000000UL

/*---------------------------------------------------------*/
/* Work Area                                               */
/*---------------------------------------------------------*/

volatile WORD Timer;

char Line[64];		/* Console input buffer */



static
void put_rc (FRESULT rc)
{
	const prog_char *p;
	static const prog_char str[] =
		"OK\0" "DISK_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "STREAM_ERR\0" "INVALID_OBJECT\0"
		"NOT_ENABLED\0" "NO_FILE_SYSTEM\0";
	FRESULT i;

	for (p = str, i = 0; i != rc && pgm_read_byte_near(p); i++) {
		while(pgm_read_byte_near(p++));
	}
	xprintf(PSTR("rc=%u FR_%S\n"), (WORD)rc, p);
}


static
void put_drc (BYTE res)
{
	xprintf(PSTR("rc=%d\n"), res);
}


static
void get_line (char *buff, BYTE len)
{
	BYTE c, i;

	i = 0;
	for (;;) {
		c = rcvr();
		if (c == '\r') break;
		if ((c == '\b') && i) i--;
		if ((c >= ' ') && (i < len - 1))
				buff[i++] = c;
	}
	buff[i] = 0;
	xmit('\n');
}


void put_dump (const BYTE *buff, DWORD ofs, UINT cnt)
{
	BYTE n;


	xitoa(ofs, 16, -8); xputc(' ');
	for(n = 0; n < cnt; n++) {
		xputc(' ');	xitoa(buff[n], 16, -2); 
	}
	xputs(PSTR("  "));
	for(n = 0; n < cnt; n++)
		xputc(((buff[n] < 0x20)||(buff[n] >= 0x7F)) ? '.' : buff[n]);
	xputc('\n');
}



/* Stream output function (console) */
static
BYTE chrout (BYTE c)
{
	/* Send the byte to the console */
	xputc(c);

	/* Return with success */
	return 1;
}


#if 0
/* Stream output function (MP3 decoder) */
static
BYTE mp3_stream (BYTE c)
{
	/* Wait for decoder ready. In case of stream sticks busy for a time, the function should abort with error (0) */
	while (!MP3READY) ;

	/* Send the byte to the decoder */
	MP3PORT = c;

	/* Return with success */
	return 1;
}
#endif


/*-----------------------------------------------------------------------*/
/* Main                                                                  */


void main (void)
{
	char *ptr;
	long p1, p2;
	BYTE res;
	WORD s1, s2, s3, ofs;
	FATFS fs;			/* File system object */
	DIR dir;			/* Directory object */
	FILINFO fno;		/* File information */


	OSCCAL = 169;

	PORTB = 0b001011;	/* - z H L H u */
	DDRB =  0b001110;


	xfunc_out = (void(*)(char))xmit;
	xputs(PSTR("\nPFF test monitor\n"));

	for (;;) {
		xputc('>');
		get_line(Line, sizeof(Line));
		ptr = Line;

		switch (*ptr++) {

		case 'd' :
			switch (*ptr++) {
			case 'i' :	/* di - Initialize physical drive */
				res = disk_initialize();
				put_drc(res);
				break;

			case 'd' :	/* dd <sector> <ofs> - Dump partial secrtor 64 bytes */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				s2 = p2;
				res = disk_readp(Line, p1, s2, 64);
				if (res) { put_drc(res); break; }
				s3 = s2 + 64;
				for (ptr = Line; s2 < s3; s2 += 16, ptr += 16, ofs += 16) {
					s1 = (s3 - s2 >= 16) ? 16 : s3 - s2;
					put_dump((BYTE*)ptr, s2, s1);
				}
				break;
			}
			break;

		case 'f' :
			switch (*ptr++) {

			case 'i' :	/* fi - Mount the volume */
				put_rc(pf_mount(&fs));
				break;

			case 'o' :	/* fo <file> - Open a file */
				while (*ptr == ' ') ptr++;
				put_rc(pf_open(ptr));
				break;

			case 'd' :	/* fd - Read file and dump it */
				ofs = fs.fptr;
				res = pf_read(Line, 64, &s1);	/* Read 64 byte data to the buffer */
				if (res != FR_OK) { put_rc(res); break; }
				ptr = Line;						/* Dump the read data */
				while (s1) {
					s2 = (s1 >= 16) ? 16 : s1;
					s1 -= s2;
					put_dump((BYTE*)ptr, ofs, s2);
					ptr += 16; ofs += 16;
				}
				break;

			case 't' :	/* ft - Forward file data to the streaming function */
				fs.flag |= FA_STREAM;	/* Set stream mode */
				do {
					res = pf_read(chrout, 32768, &s1);	/* Forward data to the stream */
					if (res != FR_OK) { put_rc(res); break; }
				} while (s1 == 32768);
				fs.flag &= ~FA_STREAM;	/* Clear stream mode */
				break;
#if _USE_LSEEK
			case 'e' :	/* fe - Move file pointer of the file */
				if (!xatoi(&ptr, &p1)) break;
				res = pf_lseek(p1);
				put_rc(res);
				if (res == FR_OK)
					xprintf(PSTR("fptr = %lu(0x%lX)\n"), fs.fptr, fs.fptr);
				break;
#endif
#if _USE_DIR
			case 'l' :	/* fl [<path>] - Directory listing */
				while (*ptr == ' ') ptr++;
				res = pf_opendir(&dir, ptr);
				if (res) { put_rc(res); break; }
				s1 = 0;
				for(;;) {
					res = pf_readdir(&dir, &fno);
					if (res != FR_OK) { put_rc(res); break; }
					if (!fno.fname[0]) break;
					if (fno.fattrib & AM_DIR)
						xprintf(PSTR("   <DIR>   %s\n"), fno.fname);
					else 
						xprintf(PSTR("%9lu  %s\n"), fno.fsize, fno.fname);
					s1++;
				}
				xprintf(PSTR("%u item(s)\n"), s1);
				break;
#endif
			}
			break;
		}
	}

}


