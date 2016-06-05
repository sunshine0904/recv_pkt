#include "pkt.h"

#if 1
#define u8_t  unsigned char
#define u16_t unsigned short
#define u32_t unsigned int
#endif

int main()
{
	u32_t raw_sock = 0;
	struct sockaddr_in recvaddr;
	u32_t len = sizeof(struct sockaddr);
	u8_t *buff = (u8_t *)malloc(BUFF_SIZE);
	u32_t i = 0,ret = 0;	

	raw_sock = socket(PF_PACKET,SOCK_RAW,IPPROTO_IP);
	printf("raw_socket:%d\n",raw_sock);
	if(raw_sock == 0)
	{
		printf("socket error\n");
		return -1;
	}

	struct ifreq ifr;
	strcpy(ifr.ifr_name,"wlan0");
	ifr.ifr_flags = IFF_UP|IFF_PROMISC|IFF_BROADCAST|IFF_RUNNING;
	if(ioctl(raw_sock,SIOCSIFFLAGS,&ifr) == -1)
	{
		printf("set promisc failed\n");
		return -1;
	}



	while(1)
	{
		printf("hahhahh\n");
		ret = recvfrom(raw_sock,buff,BUFF_SIZE,0,(struct sockaddr *)&recvaddr,&len);	
		if(ret < 0)
		{
			printf("recv error\n");
		}
		for(i = 0;i<14;i++)
		{
			printf("%02x ",buff[i]);
		}
		printf("\n");


	}
}
