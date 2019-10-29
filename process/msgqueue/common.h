//server要做的事情
//1、从消息队列中读取数据
//2、根据读到的数据进行计算，生成响应的数据
//3、把响应的数据写回到消息队列

//client要做的事情
//1、从标准输入读入数据
//2、把数据写到消息队列中
//3、从消息队列中读取数据
//4、把结果打印到显示器上

//客户端和服务器是要进行数据的交互、数据通信
//其中，主动发送数据的一方是客户端
//被动接收数据的以防就是服务器
#pragma once

#define FILEPATH "."
#define PROJ_ID 0

enum Type
{
  Request = 1,//请求，客户端给服务器发送的数据
  Response = 2,//响应，服务器给客户端返回的数据
  //当前业务场景下涉及到的类型就这两种
};

typedef struct Msgbuf
{
  long type;
  char text[1024];
} Msgbuf;

int CreateMsg();//一个进程创建，
int OpenMsg();//一个进程打开就行了
int DestroyMsg(int msgid);
int SendMsg(int msgid, char *buf, int size, int type);
//发消息的时候，服务器和客户端都有可能会调
//所以得根据调用者的不同来传入不同的类型
//消息队列是全双工
int RecvMsg(int msgid, char *buf, int size, int type);
