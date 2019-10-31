//1、通过demo体会没有互斥的情况下线程间资源抢占的数据二义问题
//2、体会互斥锁实现互斥带来的效果
//3、互斥锁实现流程
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t mutex;
int ticket = 100;

void *yellow_bull(void *arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    if(ticket > 0)
    {
      usleep(1000);
      printf("bull %ld get a ticket: %d\n", (long)arg, ticket);
      ticket--;
    }
    else
    {
      printf("no ticket, bull %ld exit\n", (long)arg);
      pthread_mutex_unlock(&mutex);//用户在加锁之后，需要在任意有可能退出线程的地方进行解锁。
      pthread_exit(NULL);
    }
    pthread_mutex_unlock(&mutex);
  }
}

int main()
{
  pthread_t tid[4];
  pthread_mutex_init(&mutex, NULL);
  int i = 0;
  for(; i < 4; ++i)
  {
    int ret = pthread_create(&tid[i], NULL, yellow_bull, (void*)i);
    if(0 != ret)
    {
      printf("thread create error\n");
      return -1;
    }
  }
  for(i = 0; i < 4; ++i)
  {
    pthread_join(tid[i], NULL);
  }
  pthread_mutex_destroy(&mutex);
  return 0;
}
