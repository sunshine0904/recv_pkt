#include "pkt.h"

#define u8_t  unsigned char
#define u16_t unsigned short
#define u32_t unsigned int

#define print_addr(addr) printf("            %d.%d.%d.%d    \n", \
  ((addr & 0xff000000) >> 24), ((addr & 0x00ff0000) >> 16), \
  ((addr & 0x0000ff00) >> 8),  (addr & 0x000000ff))

u32_t creat_rawsocket();
u32_t bind_socket_dev(u32_t socket,u8_t *device);
u32_t analysis_data(u8_t *data);
void ipv4_pkt_process(u8_t *buff);
void ipv6_pkt_process(u8_t *buff);
void arp_pkt_process(u8_t *buff);
void revarp_pkt_process(u8_t *buff);


u32_t main(int argc,char **argv)
{
	u8_t *dev = NULL;
	if(argc == 1)
	{
		dev = "eth0";
	}
	else if(argc > 1)
	{
		dev = argv[1];
	}
	
	u32_t raw_sock = 0;
	struct sockaddr_in recvaddr;
	u32_t len = sizeof(struct sockaddr);
	u8_t *buff = (u8_t *)malloc(BUFF_SIZE);
	u32_t ret = 0;	

	//creat raw socket
	raw_sock = creat_rawsocket();

	//bind device to eth0
	bind_socket_dev(raw_sock,dev);

	while(1)
	{
		if(0 > (ret = recvfrom(raw_sock,buff,BUFF_SIZE,0,NULL,NULL)))	
		{
			printf("recv error\n");
			continue;
		}

		analysis_data(buff);

	}
}

u32_t creat_rawsocket()
{
	u32_t raw_sock = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(raw_sock == 0)
	{
		printf("socket error\n");
		return -1;
	}
	printf("creat raw socket ok\n");
	return raw_sock;
}
u32_t bind_socket_dev(u32_t socket,u8_t *device)
{
	u32_t index = if_nametoindex(device);
	printf("device:%s index:%d\n",device,index);
	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));
	sll.sll_family = PF_PACKET;
	sll.sll_ifindex = index;
	sll.sll_protocol = htons(ETH_P_ALL);

	if(0 < bind(socket,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll)))
	{
		printf("bind device error\n");
		return -1;
	}
}
u32_t analysis_data(u8_t *data)
{
	//display ether header
	u32_t i = 0;
	struct ether_header *ethhdr = data;;
	printf("\n++++ether header++++\n");
	printf("eth_src_mac:");
	for(i = 0;i<6;i++)
	{
		printf("%02x ",ethhdr->ether_shost[i]);
	}
	//printf("\n");
	printf("    ");
	printf("eth_dst_mac:");
	for(i = 0;i<6;i++)
	{
		printf("%02x ",ethhdr->ether_dhost[i]);
	}
	printf("\n");
	printf("eth_type:%#06x\n",htons(ethhdr->ether_type));
	data += sizeof(struct ether_header);


	switch(htons(ethhdr->ether_type))
	{
		case ETHERTYPE_IP:
			ipv4_pkt_process(data);
			printf("it is ipv4 packet\n");
			break;
		case ETHERTYPE_IPV6:
			ipv6_pkt_process(data);
			printf("it is ipv6 packet\n");
			break;
		case ETHERTYPE_ARP:
			arp_pkt_process(data);
			printf("it is arp packet\n");
			break;
		case ETHERTYPE_REVARP:
			revarp_pkt_process(data);
			printf("it is revarp packet\n");
			break;
		default:
			printf("can't parse type! eth_type:%02x\n",htons(ethhdr->ether_type));
			break;
	}
}
void ipv4_pkt_process(u8_t *buff)
{
	printf("++++++ipv4 header++++++\n");
	struct iphdr *iph = buff;
	printf(" _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _");
	printf("\n");
	printf("|version | iphdr_len | Tos | total len  |");
	printf("\n");
	printf("    %d        %d        %d       %d     ",iph->version,iph->ihl,iph->tos,iph->tot_len);
	printf("\n");
	printf("|  identity      |      fragment offset |");
	printf("\n");
	printf("     %d                       %d        ",iph->id,iph->frag_off);

	printf("\n");
	printf("|   ttl   |    protocol  |   check      |\n");
	printf("     %d           %d           %d          ",iph->ttl,iph->protocol,iph->check);

	printf("\n");
	printf("|              src_ip                   |\n");
	print_addr(htonl(iph->saddr));
	printf("|              dst_ip                   |\n");
	print_addr(htonl(iph->daddr));
	printf("|---------------------------------------|\n");
}
void ipv6_pkt_process(u8_t *buff)
{
	
}
void arp_pkt_process(u8_t *buff)
{
	
}
void revarp_pkt_process(u8_t *buff)
{
	
}


