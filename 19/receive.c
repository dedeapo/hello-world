


int msgid=0;
struct msgt
{
	long msgtype;
	char msgtext[1024];
}


void childprocess()
{
	struct msgt msgs;

	while(1)
	{
	//������Ϣ����
	Ssize_t msgrcv(msqid, &msgs, sizeof(struct msgt), 0, 0);

	//��ӡ��Ϣ����
	printf("msg text:%s\n",msgs.msgtext);
	}
}


void main()
{
	//����Ϣ����
	msgid = msgget(1024,IPC_EXCL);

	//���������ֽ���
	for(i=0; i<3; i++)
	{
		cpid = fork();
		if(cpid<0)
			printf("creat error");
		else if(cpid == 0)
			childprocess();
	}
}
