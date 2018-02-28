#ifndef __UDP_DEMO_H

#define	__UDP_DEMO_H

#include "dhcpc.h"


void udp_demo_appcall(void);
void udp_app_init(void);
void dhcpc_configured(const struct dhcpc_state *s);

#define UIP_UDP_APPCALL		udp_demo_appcall




#endif
