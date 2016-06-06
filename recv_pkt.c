#include "pkt.h"

#if 1
#define u8_t  unsigned char
#define u16_t unsigned short
#define u32_t unsigned int
#endif


int creat_rawsocket();
int bind_socket_dev(u32_t socket,u8_t *device);



int main()
{
	u32_t raw_sock = 0;
	struct sockaddr_in recvaddr;
	u32_t len = sizeof(struct sockaddr);
	u8_t *buff = (u8_t *)malloc(BUFF_SIZE);
	u32_t i = 0,ret = 0;	
	u8_t *dev = "eth0";


	struct ether_header *ethhdr;



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

		//display ether header
		ethhdr = buff;
		printf("\n++++ether header++++\n");
		printf("eth_src_mac:");
		for(i = 0;i<6;i++)
		{
			printf("%02x ",ethhdr->ether_shost[i]);
		}
		printf("\n");
		printf("eth_dst_mac:");
		for(i = 0;i<6;i++)
		{
			printf("%02x ",ethhdr->ether_dhost[i]);
		}
		printf("\n");
		printf("eth_type:%#06x\n",htons(ethhdr->ether_type));


	}
}

int creat_rawsocket()
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
int bind_socket_dev(u32_t socket,u8_t *device)
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
