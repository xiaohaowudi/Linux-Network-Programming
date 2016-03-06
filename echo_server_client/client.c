/*
struct sockaddr_in {
	short int sin_family;                       //Address family
	unsigned short int sin_port;       			//Port number
	struct in_addr sin_addr;              		//Internet address
	unsigned char sin_zero[8];         			//Same size as struct sockaddr
};

struct in_addr {
	unsigned long s_addr;
};
*/

#define		ASSERT_ERROR(err, str) \
	do{ \
		if(err == -1){ \
			printf("%s\n", str);\
			return -1;\	   
		}\		
	}while(0); 

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(void){
	int ret;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	unsigned int client_addr_len;
	int client_fd;		//客户端的套接字fd
	unsigned char send_buf[1024];	//接收端的缓存		
	
	
	int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//创建一个套接字实例
	
	
	if(socket_fd == -1){
		printf("socket_fd == -1\n");
		return -1;
	}
	
	//设置服务器端的地址
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5188);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//客户端的端口号是随机分配的，不需要绑定特定的端口号
	ret = connect(socket_fd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr)); 
	ASSERT_ERROR(ret, "connect error!");
	
	while(1){
		scanf("%s", send_buf);
		ret = write(socket_fd, send_buf, strlen(send_buf)+1);
		ASSERT_ERROR(ret, "write error!");

		ret = read(socket_fd, send_buf, sizeof(send_buf));
		ASSERT_ERROR(ret, "read error!");
		printf("get echo mes from server:\n");
		printf("%s\n", send_buf);
	}
	
	close(socket_fd);
	return 0;
}