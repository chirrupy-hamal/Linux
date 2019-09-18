#include"common.h"
#include<sys/msg.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>

int CreateMsg()
{
  key_t key = ftok(FILEPATH, PROJ_ID);
  if(key < 0)
  {
    perror("ftok");
    return -1;
  }
  int msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
  //只是创建，不用打开
  //666是权限，既能读又能写，三个用户，不管哪一个
  //返回msgqueue消息队列的id
  if(msgid < 0)
  {
    perror("msgget");
    return -1;
  }
  return msgid;//msgid可以理解成消息队列的句柄
  //句柄就是遥控器
  //消息队列是在内核中的，你想在用户代码中直操纵不是很方便
  //ipcs -q指令，查看操作系统中当前都有哪些消息队列
  //第一列是ipckey 第二列是msgget返回的
  //消息队列的生命周期和进程没关系，生命周期随内核
  //除非手动关掉/操作系统重启
}

int OpenMsg()
{
  //此处需要保证打开的消息队列和刚才创建的消息队列是相同的
  key_t key = ftok(FILEPATH, PROJ_ID);
  if(key < 0)
  {
    perror("ftok");
    return -1;
  }
  int msgid = msgget(key, IPC_CREAT);
  if(msgid < 0)
  {
    perror("msgget");
    return -1;
  }
  return msgid;
}

int DestroyMsg(int msgid)
{
  //msgctl():这个函数是消息队列场景下配套使用的函数
  //这个函数的功能很多，不仅限于销毁
  //#include<sys/types.h>
  //#include<sys/ipc.h>
  //#include<sys/msg.h>
  //int msgctl(int msqid, int cmd, struct msqid_ds *buf);
  //参数1：消息队列句柄
  //参数2：通过cmd可以使用msgctl对消息队列进行不同的操作
  //参数3：参数2不同，参数3也就对应不同
  //IPC_RMID是删除，由于是删除，参数3就不需要了
  int ret = msgctl(msgid, IPC_RMID, NULL);
  if(ret < 0)//消息队列都不存在，删除也就失败了
  {
    perror("msgctl");
    return -1;
  }
  return 0;
}

int SendMsg(int msgid, char *buf, int size, int type)
{
  //msgsnd()把一条消息添加到消息队列中
  //#include<sys/types.h>
  //#include<sys/ipc.h>
  //#include<sys/msg.h>
  //int msgsnd(int msqid, const void *msgp,
  //          size_t msgsz, int msgflg);
  //奇葩用法
  //参数2：指向一个调用者定义的结构体
  //就是你要想调这个函数，你得让调用者先定义一个结构体出来
  //这个结构体必须是这样的：
  //struct msgbuf{
  //  long mtype;//消息类型，而且必须大于0
  //  char mtext[];//消息具体的数据，这个长度是自行设定的
  //}
  //参数3：表示char text的长度
  //参数4：这是个选项，默认不管，填0就行。
  Msgbuf msg_buf;
  if(size >= (int)sizeof(msg_buf.text))
  {
    printf("缓冲区长度不够\n");
    return -1;
  }
  msg_buf.type = type;
  strcpy(msg_buf.text, buf);
  int ret = msgsnd(msgid, &msg_buf, sizeof(msg_buf.text), 0);//阻塞
  if(ret < 0)
  {
    perror("msgsnd");
    return -1;
  }
  return 0;
}

int RecvMsg(int msgid, char *buf, int size, int type)
{
  (void)size;
  Msgbuf msg_buf;
  ssize_t ret = msgrcv(msgid, &msg_buf, sizeof(msg_buf.text), type, 0);//阻塞
  //参数3是单纯消息的长度，这个长度不包含保存消息类型的哪个长整型。
  //参数4：
  //type == 0，返回队列第一条消息；
  //type > 0，返回队列第一条类型等于type的消息；
  //type < 0，返回队列第一条类型小于等于type绝对值的消息，并且是满足条件的消息类型最小的消息。
  //参数5：这是个选项，默认不管，填0就行。
  if(ret < 0)
  {
    perror("msgrcv");
    return -1;
  }
  strcpy(buf, msg_buf.text);
  return 0;
}
