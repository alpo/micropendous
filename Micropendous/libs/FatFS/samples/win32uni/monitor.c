#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include "monitor.h"



int xatoi (const wchar_t **str, long *res)
{
	DWORD val;
	WORD c;
	BYTE radix, s = 0;


	while ((c = **str) == ' ') (*str)++;
	if (c == L'-') {
		s = 1;
		c = *(++(*str));
	}
	if (c == '0') {
		c = *(++(*str));
		if (c <= L' ') {
			*res = 0; return 1;
		}
		if (c == L'x') {
			radix = 16;
			c = *(++(*str));
		} else {
			if (c == L'b') {
				radix = 2;
				c = *(++(*str));
			} else {
				if ((c >= L'0')&&(c <= L'9'))
					radix = 8;
				else
					return 0;
			}
		}
	} else {
		if ((c < L'1')||(c > L'9'))
			return 0;
		radix = 10;
	}
	val = 0;
	while (c > L' ') {
		if (c >= L'a') c -= 0x20;
		c -= L'0';
		if (c >= 17) {
			c -= 7;
			if (c <= 9) return 0;
		}
		if (c >= radix) return 0;
		val = val * radix + c;
		c = *(++(*str));
	}
	if (s) val = 0 - val;
	*res = val;
	return 1;
}




void put_dump (const BYTE *buff, DWORD ofs, int cnt)
{
	BYTE n;


	printf("%08lX ", ofs);
	for(n = 0; n < cnt; n++)
		printf(" %02X", buff[n]);
	printf("  ");
	for(n = 0; n < cnt; n++)
		putchar(((buff[n] < 0x20)||(buff[n] >= 0x7F)) ? '.' : buff[n]);
	printf("\n");
}
