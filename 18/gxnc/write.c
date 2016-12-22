#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>

#define TEXT_SZ 2048

struct shared_use_st
{
	int written_by_you;
	char some_text[TEXT_SZ];
}

int main()
{

	int running = 1;
	struct shared_use_st *shared_stuff;
	int shmid = shmget((key_t)1234, sizeof(struct shared_use_st), IPC_CREAT);
	char buffer[TEXT_SZ];

	//���������ڴ�
	if(shmid == -1)
	{
		printf("creat share memory fail");
		exit(1);
	}

	//ӳ�乲���ڴ�
	shared_stuff = (shared_stuff*)shmat( shmid,NULL,0);

	//ѭ��
	while(running)
	{
		//����û������ȴ�һ��
		while(shared_stuff->written_by_you == 1)
		{
			sleep(1);
		}			
	
		//��ȡ�û�����
		fgets(buffer,TEXT_SZ,stdin);

		//���빲���ڴ�
		strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
		shared_stuff->written_by_you = 1;

		//�˳�
		if(strncmp(buffer, "end", 3) == 0)
		{
			running = 0;
		}
	}

	//���빲���ڴ�
	shmdt((const void *)shared_stuff);
	
}

