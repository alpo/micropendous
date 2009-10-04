/*-----------------------------------------------------------------------*/
/* PFF - Low level disk control module for Win32        (C)ChaN, 2009    */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include "diskio.h"
#include <windows.h>
#include <winioctl.h>



/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/


#define	IHV	INVALID_HANDLE_VALUE
#define	BUFSIZE 512U

typedef struct {
	DSTATUS	status;
	WORD sz_sector;
	DWORD n_sectors;
	HANDLE h_drive;
} STAT;

static
HANDLE hMutex, hTmrThread;

static volatile
STAT Stat[10];


static
DWORD TmrThreadID;

static
BYTE Buffer[BUFSIZE];


/*-----------------------------------------------------------------------*/
/* Timer Functions                                                       */
/*-----------------------------------------------------------------------*/


DWORD WINAPI tmr_thread (LPVOID parms)
{
	DWORD dw;
	int drv;


	for (;;) {
		for (drv = 0; drv < 10; drv++) {
			Sleep(1);
			if (Stat[drv].h_drive == IHV || Stat[drv].status & STA_NOINIT || WaitForSingleObject(hMutex, 100) != WAIT_OBJECT_0) continue;

			if (!DeviceIoControl(Stat[drv].h_drive, IOCTL_STORAGE_CHECK_VERIFY, NULL, 0, NULL, 0, &dw, NULL))
				Stat[drv].status |= STA_NOINIT;
			ReleaseMutex(hMutex);
			Sleep(100);
		}
	}
}



BOOL get_status (volatile STAT *stat) {
	DISK_GEOMETRY parms;
	PARTITION_INFORMATION part;
	DWORD dw;
	HANDLE h = stat->h_drive;


	if (h == IHV
		|| !DeviceIoControl(h, IOCTL_STORAGE_CHECK_VERIFY, NULL, 0, NULL, 0, &dw, NULL)
		|| !DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &part, sizeof(part), &dw, NULL)
		|| !DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &parms, sizeof(parms), &dw, NULL)) {
		stat->status = STA_NOINIT;
		return FALSE;
	}
	stat->sz_sector = (WORD)parms.BytesPerSector;
	stat->n_sectors = (DWORD)(part.PartitionLength.QuadPart / parms.BytesPerSector);
	stat->status = 0;
	return TRUE;
}




/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


BOOL assign_drives (int argc, char *argv[])
{
	int pd, n, nd;
	char str[30];
	HANDLE h;


	hMutex = CreateMutex(0, FALSE, NULL);
	if (hMutex == IHV) return FALSE;

	pd = nd = 0;
	while (pd < 10 && pd < argc - 1) {
		n = atoi(argv[pd + 1]);
		if (!n) return FALSE;
		sprintf(str, "\\\\.\\PhysicalDrive%u", n);
		printf("Disk#%u <== %s", pd, str);
		h = CreateFile(str, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
		Stat[pd].h_drive = h;
		if (h == IHV) {
			Stat[pd].status = STA_NOINIT;
			printf(" (Not Available)\n");
		} else {
			if (get_status(&Stat[pd])) {
				printf(" (%u Sectors, %u Bytes/Sector)\n", Stat[pd].n_sectors, Stat[pd].sz_sector);
			} else {
				printf(" (Not Ready)\n");
			}
			nd++;
		}
		pd++;
	}

	hTmrThread = CreateThread(NULL, 0, tmr_thread, 0, 0, &TmrThreadID);
	if (hTmrThread == IHV) nd = 0;

	return nd ? TRUE : FALSE;
}





/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS sta;


	if (WaitForSingleObject(hMutex, 5000) != WAIT_OBJECT_0) return STA_NOINIT;

	get_status(&Stat[0]);
	sta = Stat[0].status;
	if (Stat[0].sz_sector != 512)
		sta = STA_NOINIT;

	ReleaseMutex(hMutex);
	return sta;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	void* dest,			/* Pointer to the destination object */
	DWORD sector,		/* Sector number (LBA) */
	WORD sofs,			/* Offset in the sector */
	WORD count			/* Byte count (bit15:destination) */
)
{
	DRESULT res;
	BYTE *buff;
	BYTE (*stream)(BYTE);
	DWORD rnc;
	LARGE_INTEGER dofs;


	if (Stat[0].sz_sector != 512 || Stat[0].status & STA_NOINIT || WaitForSingleObject(hMutex, 3000) != WAIT_OBJECT_0) return RES_NOTRDY;

	stream = (count & 0x8000) ? dest : 0;
	count &= 0x7FFF;
//	printf("[%d:%d:%d]", sector, sofs, count);
	if (sector >= Stat[0].n_sectors || !count || sofs + count > 512) {
		res = RES_PARERR;
	} else {
		dofs.QuadPart = (LONGLONG)sector * 512;
		if (SetFilePointer(Stat[0].h_drive, dofs.LowPart, &dofs.HighPart, FILE_BEGIN) != dofs.LowPart) {
			res = RES_ERROR;
		} else {
			if (!ReadFile(Stat[0].h_drive, Buffer, 512, &rnc, NULL) || 512 != rnc) {
				res = RES_ERROR;
			} else {
				if (stream) {
					do {
						if (!stream(Buffer[sofs++])) break;
					} while (--count);
					res = !count ? RES_OK : RES_STRERR;
				} else {
					buff = dest;
					memcpy(buff, &Buffer[sofs], count);
					res = RES_OK;
				}
			}
		}
	}

	ReleaseMutex(hMutex);
	return res;
}

