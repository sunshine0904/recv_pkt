#ifndef PKT_H
#define PKT_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>





//layout two protocol
#include <net/ethernet.h> //ethernet header
#include <net/if_arp.h> //arp header

//layout three protocol
#include <linux/ip.h> //ip header
#include <linux/ipv6.h> // ipv6 header
#include <linux/icmp.h> //icmp header
#include <linux/icmpv6.h> //icmpv6 header
#include <linux/igmp.h> //igmp message

//layout four protocol
#include <linux/tcp.h> //tcp header
#include <linux/udp.h> //udp header

//layout application protocol
//such as telnet、http、 dns、ftp、ssl and so on.


#define BUFF_SIZE 4*1024

#if 0
//ethernet header 14bytes
struct ethernet
{
	u8_t  smac[6];
	u8_t  dmac[6];
	u8_t  type[2];
}

//ip
#endif


#endif 

