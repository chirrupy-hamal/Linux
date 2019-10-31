//通过这个demo来体会线程退出的几种方式
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *thr_entry(void *arg)
{
  pthread_cancel((pthread_t)arg);
  while(1)
  {
    printf("i am common thread-------%s\n", (char*)arg);
    sleep(1);
  }
  return NULL;
}

int main()
{
  pthread_t mtid = pthread_self();
  //获取调用线程的线程id
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, thr_entry, (void *)mtid);
  if(0 != ret)
  {
    printf("pthread create error\n");//不能用perror
    return -1;
  }
  printf("tid: %lu\n", tid);
  printf("tid: %p\n", tid);
  while(1)
  {
    printf("i am main thread\n");
    sleep(1);
  }
  return 0;
}
