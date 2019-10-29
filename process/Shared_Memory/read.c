#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>

#define IPC_KEY 0x12345678

int main()
{
  int shmid = shmget(IPC_KEY, 32, IPC_CREAT | 0664);
  //key值的生成有ftok函数，但是也可以直接定义。
  if(shmid < 0)
  {
    perror("shmget error");
    return -1;
  }
  void *shm_start = shmat(shmid, NULL, 0);
  if((void*)-1 == shm_start)
  {
    perror("shmat error");
    return -1;
  }
  while(1)
  {
    printf("%s\n", shm_start);
    sleep(1);
  }
  shmdt(shm_start);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}
