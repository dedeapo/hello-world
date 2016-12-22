#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define portnum 3333


int main()
{

	int sockfd,new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int nbyte;//接收到的字节数
	char buffer[128];//接受缓存

	//创建套接字
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		printf("creat error");
		exit(1);
	}

	//配置服务器地址
	bzero(&server_addr,seziof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnum);
	server_addr.sin_addr.s_addr = inet_addr("192.168.000.000");//绑定服务器ip地址，转换之后直接就是网络字节序

	//连接服务器
	if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		printf("connect error");
		exit(1);
	}

	//发送数据到服务器
	printf("please input char:\n");
	fgets(buffer, 128, stdin);
	send(sockfd, buffer, 128, 0);

	//关闭连接
	close(sockfd);

	return 0;

}

























