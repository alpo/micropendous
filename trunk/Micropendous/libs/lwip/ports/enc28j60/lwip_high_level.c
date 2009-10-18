#include <lwip_high_level.h>
#include <avr/interrupt.h>


struct netif lwip_netif;

static volatile unsigned long g_ulLocalTimer = 0;
static unsigned long g_ulTCPTimer = 0;
#if HOST_TMR_INTERVAL
static unsigned long g_ulHostTimer = 0;
#endif
#if LWIP_ARP
static unsigned long g_ulARPTimer = 0;
#endif
#if LWIP_AUTOIP
static unsigned long g_ulAutoIPTimer = 0;
#endif
#if LWIP_DHCP
static unsigned long g_ulDHCPCoarseTimer = 0;
#endif
#if LWIP_DHCP
static unsigned long g_ulDHCPFineTimer = 0;
#endif

static unsigned long g_ulIPMode = IPADDR_USE_STATIC;

void lwIPClockInit(void)
{
  TCCR0B = (1<<CS02)|(1<<CS00); //set prescaler (1024)
  TIMSK0 |=(1<<TOIE0);          //enable overflow interrupt
  TCCR0A = 0;                   //start the timer

  g_ulLocalTimer = 0;
	
}

void lwIPHostTimerHandler(void);

void lwIPServiceTimers(void)
{
    //
    // Service the host timer.
    //
#if HOST_TMR_INTERVAL
    if((g_ulLocalTimer - g_ulHostTimer) >= HOST_TMR_INTERVAL)
    {
        g_ulHostTimer = g_ulLocalTimer;
        lwIPHostTimerHandler();
    }
#endif

    //
    // Service the ARP timer.
    //
#if LWIP_ARP
    if((g_ulLocalTimer - g_ulARPTimer) >= ARP_TMR_INTERVAL)
    {
        g_ulARPTimer = g_ulLocalTimer;
        etharp_tmr();
    }
#endif

    //
    // Service the TCP timer.
    //
    if((g_ulLocalTimer - g_ulTCPTimer) >= TCP_TMR_INTERVAL)
    {
        g_ulTCPTimer = g_ulLocalTimer;
        tcp_tmr();
    }

    //
    // Service the AutoIP timer.
    //
#if LWIP_AUTOIP
    if((g_ulLocalTimer - g_ulAutoIPTimer) >= AUTOIP_TMR_INTERVAL)
    {
        g_ulAutoIPTimer = g_ulLocalTimer;
        autoip_tmr();
    }
#endif

    //
    // Service the DCHP Coarse Timer.
    //
#if LWIP_DHCP
    if((g_ulLocalTimer - g_ulDHCPCoarseTimer) >= DHCP_COARSE_TIMER_MSECS)
    {
        g_ulDHCPCoarseTimer = g_ulLocalTimer;
        dhcp_coarse_tmr();
    }
#endif

    //
    // Service the DCHP Fine Timer.
    //
#if LWIP_DHCP
    if((g_ulLocalTimer - g_ulDHCPFineTimer) >= DHCP_FINE_TIMER_MSECS)
    {
        g_ulDHCPFineTimer = g_ulLocalTimer;
        dhcp_fine_tmr();
    }
#endif
}


void lwIPInit(const unsigned char *pucMAC, 
         struct ip_addr* ipAddr,
         struct ip_addr* netMask, 
				 struct ip_addr* gwAddr,
         u8_t ulIPMode)
{

    //
    // Initialize lwIP library modules
    //
    lwip_init();

    //
    // Setup the network address values.
    //
#if LWIP_DHCP || LWIP_AUTOIP
    if(ulIPMode != IPADDR_USE_STATIC)
    {
        ipAddr->addr = 0;
        netMask->addr = 0;
        gwAddr->addr = 0;
    }
#endif

    //
    // Create, configure and add the Ethernet controller interface with
    // default settings.
    //
		lwip_netif.hwaddr[0]=pucMAC[0];
		lwip_netif.hwaddr[1]=pucMAC[1];
		lwip_netif.hwaddr[2]=pucMAC[2];
		lwip_netif.hwaddr[3]=pucMAC[3];
		lwip_netif.hwaddr[4]=pucMAC[4];
		lwip_netif.hwaddr[5]=pucMAC[5];

    netif_add(&lwip_netif, ipAddr, netMask, gwAddr, 0,
              enc28j60_if_init, ip_input);

    netif_set_default(&lwip_netif);

    g_ulIPMode = ulIPMode;

    //
    // Start DHCP, if enabled.
    //
#if LWIP_DHCP
    if(g_ulIPMode == IPADDR_USE_DHCP)
    {
        dhcp_start(&lwip_netif);
    }
#endif

    //
    // Start AutoIP, if enabled and DHCP is not.
    //
#if LWIP_AUTOIP
    if(g_ulIPMode == IPADDR_USE_AUTOIP)
    {
        autoip_start(&lwip_netif);
    }
#endif

    //
    // Bring the interface up.
    //
    netif_set_up(&lwip_netif);
}

//void lwIPEthernetIntHandler(void)
ISR(TIMER0_OVF_vect)
{
	cli();
	g_ulLocalTimer += SYSTICKMS;
	sei();
}


void DisplayIPAddress(unsigned long addr)
{
//TODO: write this code
}

//*****************************************************************************
//
// Required by lwIP library to support any host-related timer functions.
//
//*****************************************************************************
void lwIPHostTimerHandler(void)
{
    static unsigned long ulLastIPAddress = 0;
    unsigned long ulIPAddress;

    ulIPAddress = lwip_netif.ip_addr.addr;

    if(ulIPAddress == 0)
    {
    }

    //
    // Check if IP address has changed, and display if it has.
    //
    else if(ulLastIPAddress != ulIPAddress)
    {
        ulLastIPAddress = ulIPAddress;
        //DisplayIPAddress(ulIPAddress);
        //ulIPAddress = lwIPLocalNetMaskGet();
        //DisplayIPAddress(ulIPAddress);
        //ulIPAddress = lwIPLocalGWAddrGet();
        //DisplayIPAddress(ulIPAddress);
    }
}

