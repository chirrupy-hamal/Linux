#include<stdio.h>
#include<unistd.h>
#include"common.h"

int main()
{
  int msgid = OpenMsg();
  char buf[1024] = {0};
  while(1)
  {
    printf("Please enter# ");
    fflush(stdout);
    ssize_t s = read(0, buf, sizeof(buf));
    if(s > 0)
    {
      SendMsg(msgid, buf, s, 1);
    }
    RecvMsg(msgid, buf, 1024, 2);
    printf("server# %s", buf);
    fflush(stdout);
  }
  DestroyMsg(msgid);
  return 0;
}
