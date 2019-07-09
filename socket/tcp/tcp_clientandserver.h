//编写一个tcp socket类，能够实现socket的接口封装使用
//1、创建套接字
//2、为套接字绑定地址信息

//3、监听
//4、获取与客户端建立的连接

//5、向服务端发起连接请求

//6、发送数据
//7、接收数据
//8、关闭套接字

#pragma once
#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define CHECK_RET(q) if((q) == false){return -1;}

class TcpSocket
{

  private:
    int _sockfd;
  public:
    TcpSocket()
      : _sockfd(-1)
    {}

    //1、创建套接字
    bool Socket()
    {
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_sockfd < 0)
      {
        perror("socket error");
        return false;
      }
      return true;
    }

    //2、为套接字绑定地址信息
    bool Bind(std::string &ip, uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0)
      {
        perror("bind error");
        return false;
      }
      return true;
    }
    //3、监听
    bool Listen(int backlog = 5)
    {
      //man listen
      //int listen(int sockfd, int backlog);
      //bcklog：tcp的最大并发连接数。
      //建立一个队列，队列告诉操作系统，在同一时间来的请求并不能所有接收，队列结点是多少就能接收多少个连接。
      //这个队列只是限制同一时间的最大并发连接数，而不是tcp的最大连接数。
      //建立的队列不止一个，两个队列，有一个队列是，客户端请求一过来，服务端就建立一个socket放到这个队列里，但这个socket是连接没有完全建立成功的，整个队列的socket全都是如此。一旦客户端回复了ACK，才认为连接成功了，一旦连接建立成功，会把该队列的这个socket拿出来放到一开始的队列中，所以说原队列是放已经完成连接的socket序列。
      //backlog定义的是存放已经建立连接的大小，一旦满了，后序的连接全部丢弃。
      //未完成连接队列，已完成连接队列，在操作系统中，监听这一步会建立这两个序列
      //未完成来凝结队列一般会比已完成连接队列稍微多出两个左右的位置。
      //总结listen的第二个参数
      //tcp传输中同一时间tcp服务端的最大并发连接数
      //因为第二个参数，确定的是已完成连接队列的最大结点个数
      //客户端请求到来之后，服务端为这个请求建立一个socket连接，并且在连接完成后将信的socket放置到该队列中
      //意味着如果队列放满了。后序的连接就会被丢弃。
      //同一时间最多只能保存这么多的已完成连接。
      int ret = listen(_sockfd, backlog);
      if(ret < 0)
      {
        perror("listen error");
        return false;
      }
      return true;
    }
    //4、获取与客户端建立的连接
    bool Accept(TcpSocket *sock, struct sockaddr_in *clientaddr = NULL)//服务端有可能想看一下客户端地址信息是多少
    {
      //从已完成连接队列中获取已经与客户端建立连接的socket，取出之后，还可以再接收一个
      //man accept
      //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
      //addr：客户端的地址信息，到底这个套接字跟哪一个客户端建立的连接它告诉我们一下，也就是在这它会告诉我们一次，往后它就不会告诉我们了，因为连接已经建立成功了，该socket专门与客户端进行通信。
      //地址信息长度，输入输出型的参数，不但用于指定要获取的地址信息长度，并且它会获取实际的长度。
      //返回值：新建套接字的描述符，失败-1
      socklen_t len = sizeof(struct sockaddr_in);
      struct sockaddr_in addr;
      int newclient = accept(_sockfd, (struct sockaddr *)&addr, &len);
      if(newclient < 0)
      {
        perror("accept error");
        return false;
      }
      sock->_sockfd = newclient;
      if(clientaddr != NULL)
      {
        memcpy(clientaddr, &addr, len);
      }
      return true;
    }
    //5、向服务端发起连接请求
    bool Connect(std::string &ip, uint16_t port)
    {
      //int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
      //addr：服务端的地址信息
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0)
      {
        return false;
      }
      return true;
    }
    //6、发送数据
    ssize_t Send(const char *buf, size_t len)
    {
      //ssize_t send(int sockfd, const void *buf, size_t len, int flags);
      //因为tcp中服务端会为客户端新建socket进行专项通信，因此发送或接收数据就不需要用户来指定对端的地址信息了
      ssize_t ret = send(_sockfd, buf, len, 0);
      if(ret < 0)
      {
        perror("send error");
      }
      return ret;
    }
    //7、接收数据
    ssize_t Recv(char *buf, size_t len)
    {
      //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
      //返回值：0，连接断开
      ssize_t ret = recv(_sockfd, buf, len, 0);//读不到数据，堵塞
      if(ret < 0)
      {
        perror("recv perror");
      }
      else if(ret == 0)
      {
        printf("peer shutdown!\n");
      }
      return ret;
    }
    //8、关闭套接字
    bool Close()
    {
      if(_sockfd != -1)
      {
        close(_sockfd);
        _sockfd = -1;
      }
      return true;
    }
};
