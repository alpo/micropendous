/*---------------------------------------------------------------*/
/* FAT file system module test program R0.07c     (C)ChaN, 2009  */
/*---------------------------------------------------------------*/


#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include "monitor.h"
#include "diskio.h"
#include "ff.h"

#if !_USE_LFN || !_LFN_UNICODE
#error Wrong configuration
#endif


#if _MULTI_PARTITION	/* Volume - Partition resolution table */
const PARTITION Drives[] = {
	{0, 0},	/* Logical Drive 0 <== Physical Drive 0, Partition 0 */
	{0, 1},	/* Logical Drive 1 <== Physical Drive 0, Partition 1  */
	{1, 0}	/* Logical Drive 2 <== Physical Drive 1, Partition 0  */
};
#endif


/*---------------------------------------------------------*/
/* Work Area                                               */
/*---------------------------------------------------------*/

HANDLE hCon, hKey;			/* Handle to the console in/out */

DWORD acc_size;				/* Work register for scan_files() */
WORD acc_files, acc_dirs;
FILINFO Finfo;
wchar_t Lfname[256];

wchar_t Line[260];			/* Console input buffer */
wchar_t OutBuf[300];


FATFS FatFs[_DRIVES];		/* File system object for logical drive */
BYTE Buff[32768];			/* Working buffer */




/*---------------------------------------------------------*/
/* User Provided RTC Function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support an RTC.                     */
/* This function is not required in read-only cfg.         */

DWORD get_fattime (void)
{
	SYSTEMTIME tm;

	/* Get local time */
	GetLocalTime(&tm);

	/* Pack date and time into a DWORD variable */
	return 	  ((DWORD)(tm.wYear - 1980) << 25)
			| ((DWORD)tm.wMonth << 21)
			| ((DWORD)tm.wDay << 16)
			| (WORD)(tm.wHour << 11)
			| (WORD)(tm.wMinute << 5)
			| (WORD)(tm.wSecond >> 1);
}



/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */




static
void get_line (wchar_t *buff, int len)
{
	wchar_t c;
	int idx = 0;
	DWORD dw;


	for (;;) {
		ReadConsoleW(hKey, &c, 1, &dw, NULL);
		if (c == L'\r') break;
		if ((c == L'\b') && idx) {
			idx--;
		}
		if ((c >= L' ') && (idx < len - 1)) {
				buff[idx++] = c;
		}
	}
	buff[idx] = 0;
	WriteConsoleW(hCon, L"\n", 1, &dw, NULL);
}


static
FRESULT scan_files (
	wchar_t* path		/* Pointer to the path name working buffer */
)
{
	DIR dirs;
	FRESULT res;
	int i, l;


	if ((res = f_opendir(&dirs, path)) == FR_OK) {
		i = wcslen(path);
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (Finfo.fname[0] == '.') continue;
			if (!*Finfo.lfname) {
				l = mbstowcs(Finfo.lfname, Finfo.fname, strlen(Finfo.fname));
				if (l <= 0) return 99;
				Finfo.lfname[l] = 0;
			}
			if (Finfo.fattrib & AM_DIR) {
				acc_dirs++;
				*(path+i) = L'/'; wcscpy(path+i+1, Finfo.lfname);
				res = scan_files(path);
				*(path+i) = L'\0';
				if (res != FR_OK) break;
			} else {
				acc_files++;
				acc_size += Finfo.fsize;
			}
		}
	}

	return res;
}



static
void put_rc (FRESULT rc)
{
	const char *p;
	static const char str[] =
		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0";
	FRESULT i;

	for (p = str, i = 0; i != rc && *p; i++) {
		while(*p++);
	}
	printf("rc=%u FR_%s\n", (UINT)rc, p);
}



static
BOOL set_console_size (
	HANDLE hcon,
	int width,
	int height,
	int bline
)
{
	COORD dim = {width, bline};
	SMALL_RECT rect = {0, 0, width - 1, height - 1};


	if (SetConsoleScreenBufferSize(hCon, dim) && 
		SetConsoleWindowInfo(hCon, TRUE, &rect) ) return TRUE;

	return FALSE;
}





/*-----------------------------------------------------------------------*/
/* Main                                                                  */


int main (int argc, char *argv[])
{
	wchar_t *ptr, *ptr2;
	long p1, p2, p3;
	BYTE res, *bp;
	UINT s1, s2, cnt;
	WORD w;
	DWORD dw, ofs = 0, sect = 0, drv = 0;
	FATFS *fs;				/* Pointer to file system object */
	DIR dir;				/* Directory object */
	FIL file1, file2;		/* File objects */


	if ((hKey = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return 3;
	SetConsoleMode(hKey, ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	if ((hCon = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		return 3;
	set_console_size(hCon, 100, 35, 500);

	if (!assign_drives(argc, argv)) {
		printf("\nUsage: ffdev <phy drv#> [<phy drv#>] ...\n");
		return 2;
	}

	printf("\nFatFs module with Unicode API test monitor on Win32\n", Line);

	for (;;) {
		putchar('>');
		get_line(ptr = Line, sizeof(Line) / sizeof(Line[0]));

		switch (*ptr++) {

		case L'q' :	/* Exit program */
			return 0;

		case L'T' :
			while (*ptr == L' ') ptr++;

			// Quick test space

			break;

		case L'd' :
			switch (*ptr++) {
			case L'd' :	/* dd <drv> [<sector>] - Dump secrtor */
				if (!xatoi(&ptr, &p1)) p1 = drv;
				if (!xatoi(&ptr, &p2)) p2 = sect;
				res = disk_read((BYTE)p1, Buff, p2, 1);
				sect = p2 + 1; drv = p1;
				if (res) { printf("rc=%d\n", (WORD)res); break; }
				printf("Drive:%u Sector:%lu\n", p1, p2);
				if (disk_ioctl((BYTE)p1, GET_SECTOR_SIZE, &w) != RES_OK) break;
				for (bp = Buff, ofs = 0; ofs < w; ptr+=16, ofs+=16)
					put_dump(bp, ofs, 16);
				break;

			case L'i' :	/* di [<drive#>] - Initialize physical drive */
				if (!xatoi(&ptr, &p1)) break;
				res = disk_initialize((BYTE)p1);
				printf("rc=%d\n", res);
				if (disk_ioctl((BYTE)p1, GET_SECTOR_SIZE, &w) == RES_OK)
					printf("Sector size = %u\n", w);
				if (disk_ioctl((BYTE)p1, GET_SECTOR_COUNT, &dw) == RES_OK)
					printf("Number of sectors = %u\n", dw);
				break;
			}
			break;

		case L'b' :
			switch (*ptr++) {
			case L'd' :	/* bd <addr> - Dump R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				for (bp = &Buff[p1], ofs = p1, cnt = 32; cnt; cnt--, ptr+=16, ofs+=16)
					put_dump(bp, ofs, 16);
				break;

			case L'e' :	/* be <addr> [<data>] ... - Edit R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (xatoi(&ptr, &p2)) {
					do {
						Buff[p1++] = (BYTE)p2;
					} while (xatoi(&ptr, &p2));
					break;
				}
				for (;;) {
					printf("%04X %02X-", (WORD)(p1), (WORD)Buff[p1]);
					get_line(ptr = Line, sizeof(Line) / sizeof(Line[0]));
					if (*ptr == L'.') break;
					if (*ptr < L' ') { p1++; continue; }
					if (xatoi(&ptr, &p2))
						Buff[p1++] = (BYTE)p2;
					else
						printf("???\n");
				}
				break;

			case L'r' :	/* br <drv> <sector> [<n>] - Read disk into R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				printf("\nrc=%u\n", disk_read((BYTE)p1, Buff, p2, (BYTE)p3));
				break;

			case L'w' :	/* bw <sector> [<n>] - Write R/W buffer into disk */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				printf("\nrc=%u\n", disk_write((BYTE)p1, Buff, p2, (BYTE)p3));
				break;

			case L'f' :	/* bf <n> - Fill working buffer */
				if (!xatoi(&ptr, &p1)) break;
				memset(Buff, (BYTE)p1, sizeof(Buff));
				break;

			}
			break;

		case L'f' :
			switch (*ptr++) {

			case L'i' :	/* fi - Force initialized the logical drive */
				if (!xatoi(&ptr, &p1)) break;
				put_rc(f_mount((BYTE)p1, &FatFs[p1]));
				break;

			case L's' :	/* fs [<path>] - Show logical drive status */
				while (*ptr == L' ') ptr++;
				res = f_getfree(ptr, (DWORD*)&p1, &fs);
				if (res) { put_rc(res); break; }
				printf("FAT type = %u\nBytes/Cluster = %lu\nNumber of FATs = %u\n"
						"Root DIR entries = %u\nSectors/FAT = %lu\nNumber of clusters = %lu\n"
						"FAT start (lba) = %lu\nDIR start (lba,clustor) = %lu\nData start (lba) = %lu\n\n",
						(WORD)fs->fs_type,
#if _MAX_SS != 512
						(DWORD)fs->csize * fs->s_size,
#else
						(DWORD)fs->csize * 512,
#endif
						(WORD)fs->n_fats,
						fs->n_rootdir, fs->sects_fat, (DWORD)fs->max_clust - 2,
						fs->fatbase, fs->dirbase, fs->database
				);
				acc_size = acc_files = acc_dirs = 0;
				Finfo.lfname = Lfname;
				Finfo.lfsize = sizeof(Lfname);
				res = scan_files(ptr);
				if (res) { put_rc(res); break; }
				p2 = (fs->max_clust - 2) * fs->csize / 2;
				p3 = p1 * fs->csize / 2;
#if _MAX_SS != 512
				p2 *= fs->s_size / 512;
				p3 *= fs->s_size / 512;
#endif
				printf("%u files, %lu bytes.\n%u folders.\n"
					   "%lu KB total disk space.\n%lu KB available.\n\n",
						acc_files, acc_size, acc_dirs, p2, p3);
				break;

			case L'l' :	/* fl [<path>] - Directory listing */
				while (*ptr == L' ') ptr++;
				res = f_opendir(&dir, ptr);
				if (res) { put_rc(res); break; }
				p1 = s1 = s2 = 0;
				Finfo.lfname = Lfname;
				Finfo.lfsize = sizeof(Lfname);
				for(;;) {
					res = f_readdir(&dir, &Finfo);
					if ((res != FR_OK) || !Finfo.fname[0]) break;
					if (Finfo.fattrib & AM_DIR) {
						s2++;
					} else {
						s1++; p1 += Finfo.fsize;
					}

					printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s",
							(Finfo.fattrib & AM_DIR) ? 'D' : '-',
							(Finfo.fattrib & AM_RDO) ? 'R' : '-',
							(Finfo.fattrib & AM_HID) ? 'H' : '-',
							(Finfo.fattrib & AM_SYS) ? 'S' : '-',
							(Finfo.fattrib & AM_ARC) ? 'A' : '-',
							(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
							(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63, Finfo.fsize, Finfo.fname);
					for (p1 = strlen(Finfo.fname); p1 < 14; p1++)
						putchar(' ');
					WriteConsoleW(hCon, Lfname, wcslen(Lfname), &p1, NULL);
					putchar('\n');
				}
				printf("%4u File(s),%10lu bytes total\n%4u Dir(s)", s1, p1, s2);
				if (f_getfree(ptr, (DWORD*)&p1, &fs) == FR_OK)
					printf(", %10lu bytes free\n", p1 * fs->csize * 512);
				break;

			case L'o' :	/* fo <mode> <file> - Open a file */
				if (!xatoi(&ptr, &p1)) break;
				while (*ptr == L' ') ptr++;
				put_rc(f_open(&file1, ptr, (BYTE)p1));
				break;

			case L'c' :	/* fc - Close a file */
				put_rc(f_close(&file1));
				break;

			case L'e' :	/* fe - Seek file pointer */
				if (!xatoi(&ptr, &p1)) break;
				res = f_lseek(&file1, p1);
				put_rc(res);
				if (res == FR_OK)
					printf("fptr = %lu(0x%lX)\n", file1.fptr, file1.fptr);
				break;

			case L'v' :	/* fv - Truncate file */
				put_rc(f_truncate(&file1));
				break;

			case L'r' :	/* fr <len> - read file */
				if (!xatoi(&ptr, &p1)) break;
				p2 =0;
				while (p1) {
					if ((UINT)p1 >= sizeof(Buff)) {
						cnt = sizeof(Buff); p1 -= sizeof(Buff);
					} else {
						cnt = p1; p1 = 0;
					}
					res = f_read(&file1, Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				printf("%lu bytes read.\n", p2);
				break;

			case L'd' :	/* fd <len> - read and dump file from current fp */
				if (!xatoi(&ptr, &p1)) break;
				ofs = file1.fptr;
				while (p1) {
					if ((UINT)p1 >= 16) { cnt = 16; p1 -= 16; }
					else 				{ cnt = p1; p1 = 0; }
					res = f_read(&file1, Buff, cnt, &cnt);
					if (res != FR_OK) { put_rc(res); break; }
					if (!cnt) break;
					put_dump(Buff, ofs, cnt);
					ofs += 16;
				}
				break;

			case L'w' :	/* fw <len> <val> - write file */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				memset(Buff, (BYTE)p2, sizeof(Buff));
				p2 = 0;
				while (p1) {
					if ((UINT)p1 >= sizeof(Buff)) { cnt = sizeof(Buff); p1 -= sizeof(Buff); }
					else 				  { cnt = p1; p1 = 0; }
					res = f_write(&file1, Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				printf("%lu bytes written.\n", p2);
				break;

			case L'n' :	/* fn <old_name> <new_name> - Change file/dir name */
				while (*ptr == L' ') ptr++;
				ptr2 = wcschr(ptr, L' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == L' ') ptr2++;
				put_rc(f_rename(ptr, ptr2));
				break;

			case L'u' :	/* fu <name> - Unlink a file or dir */
				while (*ptr == L' ') ptr++;
				put_rc(f_unlink(ptr));
				break;

			case L'k' :	/* fk <name> - Create a directory */
				while (*ptr == L' ') ptr++;
				put_rc(f_mkdir(ptr));
				break;

			case L'a' :	/* fa <atrr> <mask> <name> - Change file/dir attribute */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				while (*ptr == L' ') ptr++;
				put_rc(f_chmod(ptr, (BYTE)p1, (BYTE)p2));
				break;

			case L't' :	/* ft <year> <month> <day> <hour> <min> <sec> <name> - Change timestamp */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.fdate = ((p1 - 1980) << 9) | ((p2 & 15) << 5) | (p3 & 31);
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.ftime = ((p1 & 31) << 11) | ((p2 & 63) << 5) | ((p3 >> 1) & 31);
				while (*ptr == L' ') ptr++;
				put_rc(f_utime(ptr, &Finfo));
				break;

			case L'x' : /* fx <src_name> <dst_name> - Copy file */
				while (*ptr == L' ') ptr++;
				ptr2 = wcschr(ptr, L' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == L' ') ptr2++;
				printf("Opening \""); WriteConsoleW(hCon, ptr, wcslen(ptr), &p1, NULL); printf("\"");
				res = f_open(&file1, ptr, FA_OPEN_EXISTING | FA_READ);
				if (res) {
					put_rc(res);
					break;
				}
				printf("\nCreating \""); WriteConsoleW(hCon, ptr2, wcslen(ptr2), &p1, NULL); printf("\"");
				res = f_open(&file2, ptr2, FA_CREATE_ALWAYS | FA_WRITE);
				if (res) {
					put_rc(res);
					f_close(&file1);
					break;
				}
				printf("\nCopying...");
				p1 = 0;
				for (;;) {
					res = f_read(&file1, Buff, sizeof(Buff), &s1);
					if (res || s1 == 0) break;   /* error or eof */
					res = f_write(&file2, Buff, s1, &s2);
					p1 += s2;
					if (res || s2 < s1) break;   /* error or disk full */
				}
				printf("\n%lu bytes copied.\n", p1);
				f_close(&file1);
				f_close(&file2);
				break;
#if _FS_RPATH
			case L'g' :	/* fg <path> - Change current directory */
				while (*ptr == L' ') ptr++;
				put_rc(f_chdir(ptr));
				break;

			case L'j' :	/* fj <drive#> - Change current drive */
				if (xatoi(&ptr, &p1)) {
					put_rc(f_chdrive((BYTE)p1));
				}
				break;
#endif
#if _USE_MKFS
			case L'm' :	/* fm <drive#> <partition rule> <cluster size> - Create file system */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				printf("The drive will be formatted. Are you sure? (Y/n)=");
				get_line(ptr = Line, sizeof(Line));
				if (*ptr != L'Y') break;
				put_rc(f_mkfs((BYTE)p1, (BYTE)p2, (WORD)p3));
				break;
#endif
			}
			break;

		}
	}

}


