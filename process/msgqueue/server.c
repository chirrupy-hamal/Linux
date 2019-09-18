#include"common.h"
#include<stdio.h>
#include<unistd.h>

int main()
{
  int msgid = CreateMsg();
  char buf[1024] = {0};
  while(1)
  {
    RecvMsg(msgid, buf, 1024, 1);
    printf("client# %s", buf);
    fflush(stdout);
    printf("Please enter# ");
    fflush(stdout);
    ssize_t s = read(0, buf, sizeof(buf));
    if(s > 0)
    {
      SendMsg(msgid, buf, s, 2);
    }
  }
  DestroyMsg(msgid);
  return 0;
}