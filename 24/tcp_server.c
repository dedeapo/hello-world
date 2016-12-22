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
	int sin_size;//接收数据的长度

	//创建套接字
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd == -1)
	{
		printf("creat error");
		exit(1);
	}

	//设置要绑定的地址
	bzero(&server_addr,seziof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnum);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定所有ip地址

	//绑定地址
	sin_size = sizeof(struct sockaddr);
	bind(sockfd, (struct sockaddr *)&server_addr, &sin_size);

	//监听窗口
	listen(sockfd, 5);

	while(1)
	{
		//等待连接
		new_fd = accept(sockfd, (struct sockaddr *)(&client_addr), sizeof(struct sockaddr));
		printf("server get connected from %s\n", inet_ntoa(client_addr.sin_addr));

		//接收数据
		nbyte = recv(new_fd, buffer, 128, 0);
		buffer[nbyte] = '\0';
		printf("server received: %s\n", buffer);

		//结束连接
		close(new_fd);

	}
	
	close(sockfd);

	return 0;
}



