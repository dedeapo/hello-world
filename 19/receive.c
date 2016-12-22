


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
	//接收消息队列
	Ssize_t msgrcv(msqid, &msgs, sizeof(struct msgt), 0, 0);

	//打印消息队列
	printf("msg text:%s\n",msgs.msgtext);
	}
}


void main()
{
	//打开消息队列
	msgid = msgget(1024,IPC_EXCL);

	//创建三个字进程
	for(i=0; i<3; i++)
	{
		cpid = fork();
		if(cpid<0)
			printf("creat error");
		else if(cpid == 0)
			childprocess();
	}
}
