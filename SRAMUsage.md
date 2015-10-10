The SRAM IC used is either the [IS61C1024AL-12JLI](http://www.issi.com/pdf/61-64C1024AL.pdf) or the [CY7C1019D-10ZSXI](http://www.cypress.com/?mpn=CY7C1019D-10ZSXI) which have 17-bit address space.

AVRs have 16-bit address space so a total of 64kbytes of memory space is available.  The AVR uses memory-mapped IO so some of the address space is used by peripherals.  The AVR's internal SRAM also uses some of the address space, 8kbytes in the case of the AT90USB1287.  This means that the full 128kbytes of external SRAM cannot be easily accessed.  Several options are available.

Included in the Micropendous [distribution](http://code.google.com/p/micropendous/downloads/list) is [UsefulMicropendousDefines.h](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/LUFA/LUFA/UsefulMicropendousDefines.h) which contains `#define`s to make SRAM usage easier and compatible across boards.

```
 PORTE_EXT_SRAM_SETUP;  // set up control port
 ENABLE_EXT_SRAM;       // enables external SRAM (use after above)
 DISABLE_EXT_SRAM;      // suspend external SRAM (data is retained)
 SELECT_EXT_SRAM_BANK0; // select bank 0
 SELECT_EXT_SRAM_BANK1; // select bank 1
 CURRENT_SRAM_BANK;     // returns 0 or 1 depending on current bank
```

With the external SRAM suspended/disabled you can reuse PORTA, PORTC, and PE0, PE1, PE2.  However, note that PE2 is also the HWB button so be very careful it is not pressed by users while the external SRAM is being accessed.

## Simple Way - 56kbytes made available ##

Allow the `avr-ld` [linker](http://www.nongnu.org/avr-libc/user-manual/using_tools.html) and [malloc()](http://www.nongnu.org/avr-libc/user-manual/malloc.html) to take care of everything.  The most useful setup is to place the heap in external SRAM and everything else in internal SRAM.  Example firmware based on [USBVirtualSerial](USBVirtualSerial.md) that does the following is [available](http://micropendous.googlecode.com/files/Micropendous-ExtSRAMUsageDemo.zip).

In your `makefile`:

```
#---------------- External Memory Options ----------------
# for the AT90USB1287 with its 8KB of SRAM, external addresses start at 0x2100
# 0xFFFF(65535) - 0x2100(8448) ==> 4-byte boundary 0xDEFC(57084) ~~> 56Kbytes of available external SRAM
# Stack grows towards 0 in internal SRAM, .data+.bss are the first 48894 bytes of external SRAM, and the heap is final 8kb of external SRAM:
EXTMEMOPTS +=-Wl,--defsym=__stack=0x8020FC,--section-start,.data=0x802100,--defsym=__heap_start=0x80DFFF,--defsym=__heap_end=0x80FFFF
```

When modifying LUFA demos make sure to change the `makefile` appropriately by setting the `BOARD` variable to `MICROPENDOUS`.

To make sure `malloc()` initializers have access to external memory, place External Memory (XMEM) initialization code into an `.init` section in your main `.c` file above `main()`:

```
// set up external SRAM prior to anything else to make sure malloc() has access to it
void EnableExternalSRAM (void) __attribute__ ((naked)) __attribute__ ((section (".init3")));
void EnableExternalSRAM(void)
{
  PORTE_EXT_SRAM_SETUP;  // set up control port
  ENABLE_EXT_SRAM;       // enable the external SRAM
  XMCRA = ((1 << SRE));  // enable XMEM interface with 0 wait states
  XMCRB = 0;
  SELECT_EXT_SRAM_BANK0; // select Bank 0
}
```

Create a global pointer to your external SRAM memory array.  For unsigned 32-bit integers the code would be:
```
static uint32_t* ExtMemArray;
```

Then simply use [malloc()](http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html) to allocate up to 56 kbytes of external SRAM:

```
ExtMemArray = (uint32_t *) malloc(57056); // 57056/4 = 14264
if (ExtMemArray == NULL) {
  abort(); // shut down the AVR to complain malloc() failed
}

```

You can now access `ExtMemArray` which is composed of 14,264 unsigned 32-bit integers throughout your code.
```
ExtMemArray[0]     = (uint32_t)4294967295;
ExtMemArray[14264] = (uint32_t)1234567890;
printf("ExtMemArray[0]=%10lu\r\n", ((uint32_t)ExtMemArray[0]));
```


## Slightly Complex Way - 112kbytes made available ##

Access the external memory directly.  The full 128kbytes (17-bit addresses) cannot be accessed through the AVR's XMEM interface as the AVR uses 16-bit addresses.  This means the 128kbyte SRAM needs to be split into two banks by controlling the highest address bit manually.


Near the start of `main()` enable the external SRAM interface (XMEM):

```
// Enable External SRAM
PORTE_EXT_SRAM_SETUP;  // set up control port
ENABLE_EXT_SRAM;       // enable the external SRAM
XMCRA = ((1 << SRE));  // enable XMEM interface with 0 wait states
XMCRB = 0;
```

To use the external SRAM:

  * create a global pointer to the start of the external SRAM
```
static uint8_t* ExtMemArray = (uint8_t*)EXT_SRAM_START;
```

  * simply use the pointer as an array throughout your code.
```
// store the value 20 as the first item in Bank0
SELECT_EXT_SRAM_BANK0;
ExtMemArray[1] = (uint8_t)20;
```

  * for example, the following will test external SRAM with data
```
uint16_t ArraySize = (uint16_t)(EXT_SRAM_SIZE);
SRAMfailAddress firstSRAMfailAddress = 0;

SELECT_EXT_SRAM_BANK0;

// Test SRAM with an upward count
for (i = 0; i < ArraySize; i++) {
  ExtMemArray[i] = (uint8_t)i;
}

for (i = 0; i < ArraySize; i++) {
  if (ExtMemArray[i] != (uint8_t)i) {
    // the & give the address of array[i]
    SRAMfailAddress = (uint16_t)(&array[i]);
  }
}
```

  * control which bank is currently being used with the previously mentioned defines
```
// 100'th item in Bank0 will be 20 and 100'th item in Bank1 will be 40.
// Note the same array pointer is used.  Bank selection controls the
// 17th address bit in order to access different memory in the SRAM IC.
SELECT_EXTSRAM_BANK0;
ExtMemArray[100] = (uint8_t)20;

SELECT_EXTSRAM_BANK1;
ExtMemArray[100] = (uint8_t)40;
```


## Complex Way - All 128kbytes made available ##

To use addresses lower than the tail 56kbytes you will have to control the address lines manually.  There is no simple and efficient way to do this automatically and is not worth the effort unless you absolutely need the additional 16kbytes.  If you know of some clever and efficient solution or a simple solution, please post a comment!