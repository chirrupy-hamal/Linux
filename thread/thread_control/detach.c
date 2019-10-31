#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>

void *thr_entry(void *arg)
{
  //pthread_detach(pthread_self());//主线程走的快的话，还是能进行等待的，并且还能获得返回值
                                //这种操作是不正常的，因为你明明对返回值不关心了，你还非要等待。
  //char ptr[] = "nihao";//没有打印出
  char *ptr = "nihao";
  pthread_exit(ptr);
  while(1)
  {
    printf("i am common thread-------%s\n", (char*)arg);
    sleep(1);
  }
  return NULL;
}

int main()
{
  printf("%d\n", errno);
  pthread_t tid;
  char *param = "this is input param";
  int ret = pthread_create(&tid, NULL, thr_entry, (void *)param);
  if(0 != ret)
  {
    printf("pthread create error\n");//不能用perror
    return -1;
  }
  pthread_detach(tid);
  void *retval;
  ret = pthread_join(tid, &retval);
  if(EINVAL == ret)
  {
    printf("thread is not joinable\n");
  }
  printf("%d\n", errno);//并没有设置
  printf("ret: %d tid: %p-------retval: %s\n", ret, tid, retval);
  while(1)
  {
    printf("i am main thread\n");
    sleep(1);
  }
  return 0;
}
