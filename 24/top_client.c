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

	//�����׽���
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		printf("creat error");
		exit(1);
	}

	//���÷�������ַ
	bzero(&server_addr,seziof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnum);
	server_addr.sin_addr.s_addr = inet_addr("192.168.000.000");//�󶨷�����ip��ַ��ת��֮��ֱ�Ӿ��������ֽ���

	//���ӷ�����
	if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		printf("connect error");
		exit(1);
	}

	//�������ݵ�������
	printf("please input char:\n");
	fgets(buffer, 128, stdin);
	send(sockfd, buffer, 128, 0);

	//�ر�����
	close(sockfd);

	return 0;

}

























