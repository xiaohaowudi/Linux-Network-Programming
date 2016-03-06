#include <stdio.h>
#include <arpa/inet.h>

int main(void){
	unsigned int x = 0x12345678;
	unsigned char *p = (unsigned char*)(&x);
	
	printf("%x %x %x %x\n", p[0], p[1], p[2], p[3]);
	
	unsigned int y = htonl(x);
	p = (unsigned char*)(&y);
	printf("%x %x %x %x\n", p[0], p[1], p[2], p[3]);
	
	unsigned int addr = inet_addr("192.168.0.100");		//点分十进制转换成网络字节序的数值
	p = (unsigned char*)(&addr);
	printf("%x %x %x %x\n", p[0], p[1], p[2], p[3]);
	
	
	struct in_addr ipaddr;
	ipaddr.s_addr = addr;
	printf("%s\n", inet_ntoa(ipaddr));		//网络字节序数值转换为点分十进制字符串
	
	return 0;
}
