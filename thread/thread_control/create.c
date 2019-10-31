//通过线程创建的demo来体会每一个线程都是一个执行流
//让每一个线程都运行一个死循环，查看是否都可以打印数据
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/syscall.h>

void *thr_entry(void *arg)
{
  while(1)
  {
    printf("i am common thread-------%s\n", (char*)arg);
    printf("%ld\n", syscall(SYS_gettid));
    sleep(1);
  }
  return NULL;
}
//线程退出，只是它对应的pcb退出了
//一个程序它的线程库不是默认链接的，程序默认链接的只有libc.so(C库，这是gcc给默认链接的)这一个库
//whereis libpthread.so指令
int main()
{
  pthread_t tid;
  char *param = "this is input param";
  int ret = pthread_create(&tid, NULL, thr_entry, (void *)param);
  if(0 != ret)
  {
    printf("pthread create error\n");//不能用perror
    return -1;
  }
  printf("tid: %lu\n", tid);
  printf("tid: %p\n", tid);
  while(1)
  {
    printf("i am main thread\n");//谁先打印不一定
    sleep(1);
  }
  return 0;
}
