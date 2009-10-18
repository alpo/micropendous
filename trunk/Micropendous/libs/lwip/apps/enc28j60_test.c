#include <stdio.h>
#include <stdlib.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <lwip_high_level.h>

#include "httpd.h"

#define ETH_HWADDR_0 0x00
#define ETH_HWADDR_1 0xbd
#define ETH_HWADDR_2 0x3b
#define ETH_HWADDR_3 0x33
#define ETH_HWADDR_4 0x05
#define ETH_HWADDR_5 0x71



int main(void)
{
  /* Disable watchdog if enabled by bootloader/fuses */
  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  /* Disable Clock Division */
  clock_prescale_set(clock_div_1);

  
	unsigned char pucMACArray[6];	

	pucMACArray[0] = ETH_HWADDR_0;
	pucMACArray[1] = ETH_HWADDR_1;
	pucMACArray[2] = ETH_HWADDR_2;
	pucMACArray[3] = ETH_HWADDR_3;
	pucMACArray[4] = ETH_HWADDR_4;
	pucMACArray[5] = ETH_HWADDR_5;

	struct ip_addr ip_addr;
	struct ip_addr net_mask; 
	struct ip_addr gw_addr;

	IP4_ADDR(&gw_addr, 10,0,2,2);
	IP4_ADDR(&ip_addr, 10,0,2,31);
	IP4_ADDR(&net_mask, 255,255,255,0);


	/* Network interface variables */

	//
	// Initialze the lwIP library, using DHCP.
	//
	lwIPInit(pucMACArray, &ip_addr, &net_mask, &gw_addr, IPADDR_USE_STATIC);//IPADDR_USE_STATIC //IPADDR_USE_DHCP
	httpd_init();

  // init periodic timer
  lwIPClockInit();
	sei();

	//
	// Loop forever
	//
	while(1)
	{
    enc28j60_if_input(&lwip_netif);

#if NO_SYS
    //
    // If no RTOS is enabled, then we do all lwIP procesing in the interrupt.
    //

    //
    // Service the lwIP timers.
    //
    lwIPServiceTimers();
#endif
	}

	return 0;
}
