/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*  THIS IS STILL A WORK IN PROGRESS

  LUFA Library (C) 2009 Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Porting of lwbt (C) 2009 by Opendous Inc. (www.opendous.org)

  For more info and instructions visit www.Micropendous.org/Bluetooth

  Permission to use, copy, modify, and distribute this software
  and its documentation for any purpose and without fee is hereby
  granted, provided that the above copyright notice appear in all
  copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/*
	Bluetooth Dongle host demo application.
	
	** NOT CURRENTLY FUNCTIONAL - DO NOT USE **
*/


/*
#define SUCCESS	0
#define FAILURE	1
volatile uint8_t malloc_status = FAILURE;

	if ((array = (uint8_t *)malloc(EXT_SRAM_SIZE * sizeof(uint8_t))) != NULL) {
		malloc_status = SUCCESS;
	} else {
		malloc_status = FAILURE;
	}
*/







#include "Bluetooth.h"

#define LED0_ON		PORTD |= (1 << PD0);
#define LED0_OFF	PORTD &= ~(1 << PD0);
#define LED1_ON		PORTD |= (1 << PD1);
#define LED1_OFF	PORTD &= ~(1 << PD1);
#define LED2_ON		PORTD |= (1 << PD2);
#define LED2_OFF	PORTD &= ~(1 << PD2);
#define LED3_ON		PORTD |= (1 << PD3);
#define LED3_OFF	PORTD &= ~(1 << PD3);
#define LED4_ON		PORTD |= (1 << PD4);
#define LED4_OFF	PORTD &= ~(1 << PD4);

#define _NOP() __asm__ __volatile__ ("nop")


/* Defines related to SRAM */
#define EXT_SRAM_START						0x2100
#define EXT_SRAM_END							0xFFFF
#define EXT_SRAM_SIZE							(uint16_t)(EXT_SRAM_END - EXT_SRAM_START)
#define SELECT_EXTSRAM_BANK0		PORTE &= ~(1 << PE7);
#define SELECT_EXTSRAM_BANK1		PORTE |= (1 << PE7);


Bluetooth_Device_t Bluetooth_DeviceConfiguration =
	{
		Class:   (DEVICE_CLASS_SERVICE_CAPTURING | DEVICE_CLASS_MAJOR_COMPUTER | DEVICE_CLASS_MINOR_COMPUTER_PALM),
		PINCode: "0000",
		Name:    "LUFA Bluetooth Dem1"
	};



	
struct  phybusif_cb*  cb;
u8_t  bt_timer  =  0;
u16_t  http_timer  =  0;
u16_t  dma0bsz  =  128; // DMA 0 Buf size - TODO - shouldn't this be bigger, and why was it TCR0?
u8_t  bt_ip_timer  =  0;



int main(void)
{
	SetupHardware();
	
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	puts_P(PSTR(ESC_FG_CYAN "Bluetooth Host Demo running.\r\n" ESC_FG_WHITE));
		  

//	xTaskCreate(USBTask, (signed portCHAR *) "usbtask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
	xTaskCreate(StckTask, (signed portCHAR *) "stcktask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
	xTaskCreate(MgmtTask, (signed portCHAR *) "mgmttask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
	xTaskCreate(ScndTask, (signed portCHAR *) "scndtask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
	xTaskCreate(MainTask, (signed portCHAR *) "maintask", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );

	// Start the scheduler
	vTaskStartScheduler();

//for (;;) {
//Main_Task();
//	Bluetooth_Stack_Task();
//	Bluetooth_Management_Task();
//	USB_USBTask();
//}

	// Should never get here!
	return 0;
}



void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* disable JTAG to allow corresponding pins to be used - PF4, PF5, PF6, PF7 */
	#if ((defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) ||  \
			defined(__AVR_ATmega32U6__)))
		// note the JTD bit must be written twice within 4 clock cycles to disable JTAG
		// you must also set the IVSEL bit at the same time, which requires IVCE to be set first
		// port pull-up resistors are enabled - PUD(Pull Up Disable) = 0
//		MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD);
//		MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);
	#endif


	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
		DDRA = 0;
		PORTA = 0;
		DDRB = 0;
		PORTB = 0;
		DDRC = 0;
		PORTC = 0;
		DDRD = 0xFF; // output
		PORTD = 0;
		DDRE = (1 << PE6); // set PE6 to HIGH to disable external SRAM, if connected
		PORTE = (1 << PE6);
		DDRF = 0;
		PORTF = 0;
	#endif

	/* enable external SRAM interface */
	DDRE = 0xFF;
    PORTE = ((1 << PE7) | (1 << PE0) | (1 << PE1) | (1 << PE2) | (0 << PE6)); // PE6 is chip enable, PE0,1,2 are SRAM-related
    XMCRA = ((1 << SRE));// | (1 << SRW10) | (1 << SRW11));
    //XMCRB = (1 << XMBK);
    XMCRB = 0;

	/* Hardware Initialization */
	SerialStream_Init(9600, false);
	LEDs_Init();
	USB_Init();

	LED0_OFF;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;



	mem_init();
	memp_init();
	pbuf_init(); 
	netif_init();
	ip_init();
	tcp_init();
	puts_P(PSTR("TCP/IP initialized.\r\n"));

	lwbt_memp_init();
	phybusif_init();
	if(hci_init() != ERR_OK) {
		puts_P(PSTR("HCI initialization failed!\r\n"));
	}

	l2cap_init();
	sdp_init();
	rfcomm_init();
	ppp_init();

	puts_P(PSTR("Bluetooth initialized.\r\n"));

	httpd_init();
	puts_P(PSTR("Applications started.\r\n"));

	cb = mem_malloc(sizeof(struct phybusif_cb));
//	cb->dmabuf = get_dm0ichars();	// must pass in USB-based data
	phybusif_reset(cb);

	bt_ip_start();
}



void USBTask(void *pvParameters)
{
	portTickType xDelayPeriod = taskDelayPeriod;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
	        Bluetooth_Stack_Task();
                Bluetooth_Management_Task();
                USB_USBTask();

		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		// vTaskDelay(xDelayPeriod);
	}
}



void StckTask(void *pvParameters)
{
	portTickType xDelayPeriod = 1000;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		if ((PORTD & (1 << PD4)) ^ (1 << PD4)) {
			LED4_ON;
		} else {
			LED4_OFF;
		}
		l2cap_tmr();
		rfcomm_tmr();
		ppp_tmr();
		//Bluetooth_Stack_Task();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}



void MgmtTask(void *pvParameters)
{
	portTickType xDelayPeriod = 100;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		if ((PORTD & (1 << PD3)) ^ (1 << PD3)) {
			LED3_ON;
		} else {
			LED3_OFF;
		}
		tcp_tmr();	// needs to run every 100ms
		//Bluetooth_Management_Task();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}



void ScndTask(void *pvParameters)
{
	portTickType xDelayPeriod = 2400;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		if ((PORTD & (1 << PD2)) ^ (1 << PD2)) {
			LED2_ON;
		} else {
			LED2_OFF;
		}
		Run_Once_Per_Second();
		bt_ip_tmr();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}



void MainTask(void *pvParameters)
{
	// MainTask runs at lowest priority so no delays are needed as the
	// FreeRTOS kernel is preemptive
	
	//portTickType xDelayPeriod = taskDelayPeriod;
	//portTickType xLastWakeTime;
	//xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		Main_Task();
		// vTaskDelayUntil is faster and deterministic
		//vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}




// TODO - CoRoutines are not enabled, but FreeRTOS complains during compile
void vApplicationIdleHook(void)
{
	vCoRoutineSchedule();
}



void EVENT_USB_Host_DeviceAttached(void)
{
	puts_P(PSTR(ESC_FG_GREEN "Device Attached.\r\n" ESC_FG_WHITE));

	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}



void EVENT_USB_Host_DeviceUnattached(void)
{
	puts_P(PSTR(ESC_FG_GREEN "\r\nDevice Unattached.\r\n" ESC_FG_WHITE));

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}



void EVENT_USB_Host_DeviceEnumerationComplete(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_READY);
}



void EVENT_USB_Host_HostError(uint8_t ErrorCode)
{
	USB_ShutDown();

	printf_P(PSTR(ESC_FG_RED "Host Mode Error\r\n"
	                         " -- Error Code %d\r\n" ESC_FG_WHITE), ErrorCode);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	for(;;);
}



void EVENT_USB_Host_DeviceEnumerationFailed(uint8_t ErrorCode, uint8_t SubErrorCode)
{
	printf_P(PSTR(ESC_FG_RED "Dev Enum Error\r\n"
	                         " -- Error Code %d\r\n"
	                         " -- Sub Error Code %d\r\n"
	                         " -- In State %d\r\n" ESC_FG_WHITE), ErrorCode, SubErrorCode, USB_HostState);
	
	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}



void Bluetooth_Management_Task(void)
{
	uint8_t ErrorCode;

	switch (USB_HostState)
	{
		case HOST_STATE_Addressed:
			puts_P(PSTR("Getting Device Data.\r\n"));
		
			/* Get and process the configuration descriptor data */
			if ((ErrorCode = ProcessDeviceDescriptor()) != SuccessfulDeviceRead)
			{
				if (ErrorCode == ControlErrorDuringDeviceRead)
				  puts_P(PSTR(ESC_FG_RED "Control Error (Get Device).\r\n"));
				else
				  puts_P(PSTR(ESC_FG_RED "Invalid Device.\r\n"));

				printf_P(PSTR(" -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);
				
				/* Indicate error via status LEDs */
				LEDs_SetAllLEDs(LEDS_LED1);

				/* Wait until USB device disconnected */
				USB_HostState = HOST_STATE_WaitForDeviceRemoval;
				break;
			}

			puts_P(PSTR("Bluetooth Dongle Detected.\r\n"));

			/* Set the device configuration to the first configuration (rarely do devices use multiple configurations) */
			if ((ErrorCode = USB_Host_SetDeviceConfiguration(1)) != HOST_SENDCONTROL_Successful)
			{
				printf_P(PSTR(ESC_FG_RED "Control Error (Set Configuration).\r\n"
				                         " -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);

				/* Indicate error via status LEDs */
				LEDs_SetAllLEDs(LEDS_LED1);

				/* Wait until USB device disconnected */
				USB_HostState = HOST_STATE_WaitForDeviceRemoval;
				break;
			}
				
			puts_P(PSTR("Getting Config Data.\r\n"));
		
			/* Get and process the configuration descriptor data */
			if ((ErrorCode = ProcessConfigurationDescriptor()) != SuccessfulConfigRead)
			{
				if (ErrorCode == ControlErrorDuringConfigRead)
				  puts_P(PSTR(ESC_FG_RED "Control Error (Get Configuration).\r\n"));
				else
				  puts_P(PSTR(ESC_FG_RED "Invalid Device.\r\n"));

				printf_P(PSTR(" -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);
				
				/* Indicate error via status LEDs */
				LEDs_SetAllLEDs(LEDS_LED1);

				/* Wait until USB device disconnected */
				USB_HostState = HOST_STATE_WaitForDeviceRemoval;
				break;
			}

			puts_P(PSTR("Bluetooth Dongle Enumerated.\r\n"));

			USB_HostState = HOST_STATE_Configured;
			break;
	}
}


/* Tasks that need to be run once per second */
void Run_Once_Per_Second(void)
{
	if ((PORTD & (1 << PD0)) ^ (1 << PD0)) {
		LED0_ON;
	} else {
		LED0_OFF;
	}
}



/* Main_Task will run at lowest priority once initialization is complete */
void Main_Task(void)
{
	for (uint8_t i = 0; i < 255; i++){ PORTB = 0;}

	if ((PORTD & (1 << PD0)) ^ (1 << PD0)) {
		LED1_ON;
	} else {
		LED1_OFF;
	}

//	Bluetooth_Stack_Task();
//	Bluetooth_Management_Task();
	USB_USBTask();

}








/*-----------------------------------------------------------------------------------*/
/* 
 * phybusif_init():
 *
 * Initializes the physical bus interface
 */
/*-----------------------------------------------------------------------------------*/
void phybusif_init(void)
{
}


/*-----------------------------------------------------------------------------------*/
/* 
 * phybusif_reset():
 *
 * Resets the physical bus interface control block to its initial state.
 */
/*-----------------------------------------------------------------------------------*/
err_t phybusif_reset(struct phybusif_cb *cb)
{
/*
  // Init new ctrl block
  // Alloc new pbuf. lwIP will handle dealloc
  if((cb->p = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL)) == NULL) {
    DEBUGF(PHYBUSIF_DEBUG, ("phybusif_reset: Could not allocate memory for pbuf\n"));
    return ERR_MEM; // Could not allocate memory for pbuf
  }
  cb->q = cb->p; // Make p the pointer to the head of the pbuf chain and q to the tail
  
  cb->tot_recvd = 0;
  cb->recvd = 0; 

  cb->bcount = 0; 
  cb->bsize = 0;

  cb->state = W4_PACKET_TYPE;
*/
  return ERR_OK;
}


/*-----------------------------------------------------------------------------------*/
/* 
 * phybusif_reset():
 *
 * Handle incoming DMA data by parsing the HCI header and forwarding it to the HCI.
 */
/*-----------------------------------------------------------------------------------*/
err_t phybusif_input(struct phybusif_cb *cb)
{
  u16_t i = 0;
  u8_t n;
/*
  for(i = cb->bcount; i < cb->bsize; i++) {
    switch(cb->state) {
    case W4_PACKET_TYPE:
      switch(cb->dmabuf[i]) {
      case HCI_ACL_DATA_PACKET:
	cb->state = W4_ACL_HDR;
	break;
      case HCI_EVENT_PACKET:
	cb->state = W4_EVENT_HDR;
	break;
      default:
        DEBUGF(PHYBUSIF_DEBUG, ("phybusif_input: Unknown packet type 0x%x\n", cb->dmabuf[i]));
	break;
      }
      break;
    case W4_EVENT_HDR:
      ((u8_t *)cb->q->payload)[cb->recvd] = cb->dmabuf[i];
      cb->tot_recvd++;
      cb->recvd++;
      if(cb->recvd == HCI_EVENT_HDR_LEN) {
	cb->evhdr = cb->p->payload;
	pbuf_header(cb->p, -HCI_EVENT_HDR_LEN);
	cb->recvd = cb->tot_recvd = 0;
	if(cb->evhdr->len > 0) {
	  cb->state = W4_EVENT_PARAM;
	} else {
	  hci_event_input(cb->p); // Handle incoming event
	  pbuf_free(cb->p);
	  phybusif_reset(cb);
	  cb->bcount = i + 1;
	  return ERR_OK; // Since there most likley won't be any more data in the input buffer
	}
      }
      break;
    case W4_EVENT_PARAM:
      ((u8_t *)cb->q->payload)[cb->recvd] = cb->dmabuf[i];
      cb->tot_recvd++;
      cb->recvd++;
      if(cb->recvd == cb->q->len) { // Pbuf full. alloc and add new tail to chain
        cb->recvd = 0;
        if((cb->q = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL)) == NULL) {
	  DEBUGF(PHYBUSIF_DEBUG, ("phybusif_input: Could not allocate memory for event parameter pbuf\n"));
	  return ERR_MEM; // Could not allocate memory for pbuf
	}
	pbuf_chain(cb->p, cb->q);
	pbuf_free(cb->q);
      }
      if(cb->tot_recvd == cb->evhdr->len) {
	hci_event_input(cb->p); // Handle incoming event
        pbuf_free(cb->p);
	phybusif_reset(cb);
	cb->bcount = i + 1;
        return ERR_OK; // Since there most likley won't be any more data in the input buffer
      }
      break;
    case W4_ACL_HDR:
      ((u8_t *)cb->q->payload)[cb->recvd] = cb->dmabuf[i];
      cb->tot_recvd++;
      cb->recvd++;
      if(cb->recvd == HCI_ACL_HDR_LEN) {
	cb->aclhdr = cb->p->payload;
	pbuf_header(cb->p, -HCI_ACL_HDR_LEN);
	cb->recvd = cb->tot_recvd = 0;
	if(cb->aclhdr->len > 0) {
	  cb->state = W4_ACL_DATA;
	} else {
	  DEBUGF(PHYBUSIF_DEBUG, ("phybusif_reset: Forward Empty ACL packet to higher layer\n"));
	  hci_acl_input(cb->p); // Handle incoming ACL data
	  phybusif_reset(cb);
	  cb->bcount = i + 1;
	  return ERR_OK; // Since there most likley won't be any more data in the input buffer
	}
      }
      break;
    case W4_ACL_DATA:
      ((u8_t *)cb->q->payload)[cb->recvd] = cb->dmabuf[i];
      cb->tot_recvd++;
      cb->recvd++;
      if(cb->recvd == cb->q->len) { // Pbuf full. alloc and add new tail to chain
        cb->recvd = 0;
        if((cb->q = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL)) == NULL) {
	  DEBUGF(PHYBUSIF_DEBUG, ("phybusif_input: Could not allocate memory for ACL data pbuf\n"));
	  return ERR_MEM; // Could not allocate memory for pbuf
	}
        pbuf_chain(cb->p, cb->q);
	pbuf_free(cb->q);
      }
      if(cb->tot_recvd == cb->aclhdr->len) {
	DEBUGF(PHYBUSIF_DEBUG, ("phybusif_input: Forward ACL packet to higher layer\n"));
	hci_acl_input(cb->p); // Handle incoming ACL data
	phybusif_reset(cb);
	cb->bcount = i + 1;
        return ERR_OK; // Since there most likley won't be any more data in the input buffer
      }
      break;
    default:
      DEBUGF(PHYBUSIF_DEBUG, ("phybusif_input: Unknown state\n\n"));
      break;
    }
  } // for
  // All data from dma input buffer handled, go wait for more
  cb->bcount = i;
*/
  return ERR_OK;
}


/*-----------------------------------------------------------------------------------*/
/* 
 * phybusif_output():
 *
 * Called by HCI to output data on the physical bus (UART).
 */
/*-----------------------------------------------------------------------------------*/
void phybusif_output(struct pbuf *p, unsigned int len)
{
  static struct pbuf *q;
  static int i;
  static unsigned char *ptr;
  unsigned char c;
/*
  // Send pbuf on UART
  for(q = p; q != NULL; q = q->next) {
    ptr = q->payload;
    for(i = 0; i < q->len && len; i++) {
      c = *ptr++;
      UART_SEND(c);
      --len;
    }
  }
*/
}

