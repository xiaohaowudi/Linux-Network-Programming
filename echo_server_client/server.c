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


/*
	简单的socket程序，服务器把客户端发送过来的字符串变成大写的然后回传客户端
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

int main(void){
	int ret;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	unsigned int client_addr_len;
	int client_fd;		//客户端的套接字fd
	unsigned char recv_buf[1024];	//接收端的缓存
	int i;		
	
	
	int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//创建一个套接字实例
	
	
	if(socket_fd == -1){
		printf("socket_fd == -1\n");
		return -1;
	}
	
	//设置socket的地址
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5188);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//将套接字和本地的一个进程绑定
	if( bind(socket_fd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr)) ){
		printf("bind error!\n");
		return -1;
	}
	
	/*
		设置套接字为监听态的被动套接字，开始接受三次握手
		设置队列长度为3,并发进行三次握手的客户端最多3个
	*/
	ret = listen(socket_fd, 3); 
	if(ret){
		printf("listen error!\n");
		return -1;
	}
	
	/*
		从完成三次握手队列头部取一个客户端地址,若没有客户端连上来，就堵塞
	*/
	client_addr_len = sizeof(client_addr);
	client_fd = accept(socket_fd, (struct sockaddr*)(&client_addr), &client_addr_len);
	if(client_fd == -1){
		printf("accept error!\n");
		return -1;
	}
	else{	
		printf("\nclient info :\n");
		printf("%s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_addr.s_addr));
	}
	
	//循环接收客户端发送的数据
	while(1){
		ret = read(client_fd, recv_buf, sizeof(recv_buf)-1);
		if(ret == -1){
			printf("read error!\n");
			return -1;
		}
		
		printf("\nread %d bytes:\n", ret);
		printf("%s\n", recv_buf);

		for(i=0; i<strlen(recv_buf); i++){
			recv_buf[i] += ('A'-'a');
		}
		ret = write(client_fd, recv_buf, ret);
		ASSERT_ERROR(ret, "write error!");
	}
	
	close(socket_fd);
	
	return 0;
}