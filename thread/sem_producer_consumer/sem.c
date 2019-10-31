//这个demo中体会posix标准信号量的基本操作
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

#include<unistd.h>

int ticket = 100;
sem_t sem;

void *thr_start(void *arg)
{
  (void)arg;
  while(1)
  {
    sem_wait(&sem);
    if(ticket > 0)
    {
      printf("get a ticket: %d\n", ticket);
      ticket--;
    }
    else 
    {
      sem_post(&sem);
      pthread_exit(NULL);
    }
    sem_post(&sem);
  }
  return NULL;
}

int main()
{
  int i;
  pthread_t tid[4];
  int ret;
  sem_init(&sem, 0, 1);//把计数器初始化成1
  for(i = 0; i < 4; ++i)
  {
    ret = pthread_create(&tid[i], NULL, thr_start, NULL);
    if(0 != ret)
    {
      printf("pthread create error\n");
      return -1;
    }
  }

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
  pthread_join(tid[3], NULL);

  sem_destroy(&sem);
  return 0;
}
