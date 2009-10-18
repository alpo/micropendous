#ifndef __LWIP_HIGH_LEVEL_H__
#define __LWIP_HIGH_LEVEL_H__

// lwIP API Header Files
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/netifapi.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "lwip/mem.h"
#include "netif/etharp.h"
#include "lwip/autoip.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"

#include "netif/enc28j60if.h"

// Defines for setting up the system clock.
#define SYSTICKHZ               (F_CPU / 262144UL)
#define SYSTICKMS               (1000UL / SYSTICKHZ)
#define SYSTICKUS               (1000000UL / SYSTICKHZ)
#define SYSTICKNS               (1000000000UL / SYSTICKHZ)


#define IPADDR_USE_STATIC       0
#define IPADDR_USE_DHCP         1
#define IPADDR_USE_AUTOIP       2

extern struct netif lwip_netif;


void lwIPInit(const unsigned char *pucMAC, 
         struct ip_addr* ipAddr,
         struct ip_addr* netMask, 
				 struct ip_addr* gwAddr,
         u8_t ulIPMode);
         
void lwIPClockInit(void);
void lwIPServiceTimers(void);

         

#endif //__LWIP_HIGH_LEVEL_H__