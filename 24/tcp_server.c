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
	int nbyte;//���յ����ֽ���
	char buffer[128];//���ܻ���
	int sin_size;//�������ݵĳ���

	//�����׽���
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd == -1)
	{
		printf("creat error");
		exit(1);
	}

	//����Ҫ�󶨵ĵ�ַ
	bzero(&server_addr,seziof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnum);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//������ip��ַ

	//�󶨵�ַ
	sin_size = sizeof(struct sockaddr);
	bind(sockfd, (struct sockaddr *)&server_addr, &sin_size);

	//��������
	listen(sockfd, 5);

	while(1)
	{
		//�ȴ�����
		new_fd = accept(sockfd, (struct sockaddr *)(&client_addr), sizeof(struct sockaddr));
		printf("server get connected from %s\n", inet_ntoa(client_addr.sin_addr));

		//��������
		nbyte = recv(new_fd, buffer, 128, 0);
		buffer[nbyte] = '\0';
		printf("server received: %s\n", buffer);

		//��������
		close(new_fd);

	}
	
	close(sockfd);

	return 0;
}



