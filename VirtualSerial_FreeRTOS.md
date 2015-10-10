The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/VirtualSerial_FreeRTOS` directory.

**Purpose:** `VirtualSerial_FreeRTOS` combines the [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB library with the preemptive [FreeRTOS](http://www.freertos.org/) kernel.  The preemptive FreeRTOS kernel allows for multitasking user code and LUFA USB functions.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS) or [Download](http://www.Micropendous.org/Distribution)

**USB Mode:** Device-mode enumerating as USB Virtual Serial Port

**Targets:** Any USB AVR board with more than 2kbytes of SRAM.

**Status:** _Working_.

## Usage ##

Compile and upload the `VirtualSerial_FreeRTOS` firmware onto your Micropendous board by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

`VirtualSerial_FreeRTOS` programmed devices will enumerate as a Virtual Serial Port and any data sent to the device using [Serial](Serial.md) will be echoed back.

## Firmware Design ##

This firmware is based on [VirtualSerial](VirtualSerial.md).  The preemptive [FreeRTOS](http://www.freertos.org/FreeRTOS) kernel was added to allow multi-tasking and USB and user code functionality.  Standard `stdin` and `stdout/stderr` streams are created in `main()` and linked to the USB Virtual Serial Port.  This means you can use standard `fprintf` and `fgetc` functions to read and write data over the USB connection.  Just place your code in `MainTaskLoop()`.  See [avr-libc stdio](http://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html) for detailed instructions on using stdio functions.

Alterations to [VirtualSerial.c](http://code.google.com/p/micropendous/source/diff?spec=svn856&r=856&format=side&path=/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS/VirtualSerial.c), [VirtualSerial.h](http://code.google.com/p/micropendous/source/diff?spec=svn856&r=856&format=side&path=/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS/VirtualSerial.h), and the [makefile](http://code.google.com/p/micropendous/source/diff?spec=svn856&r=856&format=side&path=/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS/makefile) are needed to enable FreeRTOS.  [FreeRTOSConfig.h](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS/FreeRTOSConfig.h?spec=svn856&r=856) must be added to define which FreeRTOS functionality to include.

The `makefile` changes are related to including FreeRTOS header directories and in the list of source files to compile.

```
MCU			= at90usb1287
BOARD			= MICROPENDOUS_REV2
F_CPU			= 16000000
...
FREERTOS_PATH		= ../../libs/FreeRTOS
FREERTOS_DEMO_DIR	= $(FREERTOS_PATH)/Demo/Common/Minimal
FREERTOS_SOURCE_DIR	= $(FREERTOS_PATH)/Source
FREERTOS_PORT_DIR	= $(FREERTOS_PATH)/Source/portable/GCC/AT90USB_ATmegaXXUY
FREERTOS_SOURCE		= $(FREERTOS_SOURCE_DIR)/tasks.c \
				$(FREERTOS_SOURCE_DIR)/queue.c \
				$(FREERTOS_SOURCE_DIR)/list.c \
				$(FREERTOS_SOURCE_DIR)/portable/MemMang/heap_1.c \
				$(FREERTOS_PORT_DIR)/port.c
SRC			= $(TARGET).c Descriptors.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS) $(FREERTOS_SOURCE)
CC_FLAGS		= -DUSE_LUFA_CONFIG_HEADER -IConfig/ -I$(FREERTOS_SOURCE_DIR)/include -I$(FREERTOS_SOURCE_DIR) -I$(FREERTOS_PORT_DIR) -I$(FREERTOS_DEMO_DIR)
```

_Note:_ DO NOT alter Timer1 settings in your code as it is used by FreeRTOS.

Connect to a board running this firmware with a [Serial Terminal](http://www.Micropendous.org/Serial) and any data you send will be looped back.

FreeRTOS is enabled in [main](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS/VirtualSerial.c#86) by creating tasks and starting the scheduler:

```
  // Create Tasks for FreeRTOS
  // The VirtualSerial/USB-CDC task is highest priority to ensure USB functions run in time
  xTaskCreate(MainTask, (signed portCHAR *) "MainTask", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );
  xTaskCreate(VirtualSerialTask, (signed portCHAR *) "ViSeTask", configMINIMAL_STACK_SIZE, NULL, (ViSe_TASK_PRIORITY | portPRIVILEGE_BIT), NULL );

  // Start the scheduler
  vTaskStartScheduler();

```

The FreeRTOS kernel runs [MainTask](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS/VirtualSerial.c#196) and [VirtualSerialTask](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial_FreeRTOS/VirtualSerial.c#172) tasks as they yield or delay, respectively.  `VirtualSerialTask` performs USB and Virtual Serial communication.  `MainTask` calls `MainTaskLoop` which reads and writes data to the USB Virtual Serial stream.

```
static void VirtualSerialTask(void *pvParameters)
{
  for (;;)
  {
    // want CDC and USB functions to run without interruption but
    // with interrupts enabled so ENTER/EXIT_CRITICAL won't work
    vTaskSuspendAll();
      CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
      USB_USBTask();
    xTaskResumeAll();

    vTaskDelay((portTickType) taskDelayPeriod );
  }
}


static void MainTask(void *pvParameters)
{
  for(;;)
  {
    MainTaskLoop();
    portYIELD();
  }
}


void MainTaskLoop(void)
{
  int count = 0;

  // If the host has sent data then echo it back
  // NOTE: AVRlibc stdio functions are not thread-safe
  // and must therefore be in a Suspend-Resume section
  vTaskSuspendAll();
    count = fread(&buffer, 1, CDC_TXRX_EPSIZE, &USBSerialStream);
  xTaskResumeAll();

  //TODO: you can process the received buffer data here

  vTaskSuspendAll();
  if (count > 0)
  {
    fwrite(&buffer, 1, count, &USBSerialStream);
  }

  xTaskResumeAll();

  // If HWB Button is pressed then send formatted strings
  if (Buttons_GetStatus())
  {
    // NOTE: AVRlibc stdio functions are not thread-safe
    // and must therefore be in a Suspend-Resume section
    vTaskSuspendAll();
      // send a constant string stored in FLASH
      fprintf_P(&USBSerialStream, PSTR("\r\nHWB has been pressed!\r\n"));
      // send a string that is dynamic and stored in SRAM
      fprintf(&USBSerialStream, "PORTD = %3x\r\n", PIND);
    xTaskResumeAll();
  }
}

```

On the host side, `serialpy.py` is an example of serial communication and is explained in [Serial](Serial.md).