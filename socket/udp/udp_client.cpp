//使用C++封装一个udp的socket类

#include<iostream>
#include<string>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

class UdpSocket
{
  private:
    int _sockfd;
  public:
    UdpSocket()
      :_sockfd(-1)
    {}

    //1、创建套接字
    bool Socket()
    {
      _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

    //3、发送数据
    ssize_t Send(const char *buf, size_t len, struct sockaddr_in *addr)
    {
      int ret;
      socklen_t addr_len = sizeof(struct sockaddr_in);
      ret = sendto(_sockfd, buf, len, 0, (struct sockaddr*)addr, addr_len);
      return ret;
    }

    //4、接受数据
    ssize_t Recv(char *buf, size_t len, struct sockaddr_in *addr = NULL)
    {
      int ret;
      struct sockaddr_in _addr;
      socklen_t addr_len = sizeof(struct sockaddr_in);
      ret = recvfrom(_sockfd, buf, len, 0, (struct sockaddr*)&_addr, &addr_len);
      if(addr)
      {
        memcpy((void*)addr, (void*)&_addr, addr_len);
      }
      return ret;
    }

    //5、关闭套接字
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

#define CHECK_RET(q) if((q) == false){return -1;}

int main()
{
  UdpSocket sock;

  CHECK_RET(sock.Socket());
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(9000);
  server_addr.sin_addr.s_addr = inet_addr("172.17.145.238");
  while(1)
  {
    std::string str;
    std::cin >> str;
    sock.Send(str.c_str(), str.length(), &server_addr);
    char buff[1024] = {0};
    sock.Recv(buff, 1024);
    printf("server say: %s\n", buff);
  }
  sock.Close();

  return 0;
}
