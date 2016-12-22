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

	//创建共享内存
	if(shmid == -1)
	{
		printf("creat share memory fail");
		exit(1);
	}

	//映射共享内存
	shared_stuff = (shared_stuff*)shmat( shmid,NULL,0);

	//循环
	while(running)
	{
		//内容没读走则等待一秒
		while(shared_stuff->written_by_you == 1)
		{
			sleep(1);
		}			
	
		//获取用户输入
		fgets(buffer,TEXT_SZ,stdin);

		//放入共享内存
		strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
		shared_stuff->written_by_you = 1;

		//退出
		if(strncmp(buffer, "end", 3) == 0)
		{
			running = 0;
		}
	}

	//脱离共享内存
	shmdt((const void *)shared_stuff);
	
}

