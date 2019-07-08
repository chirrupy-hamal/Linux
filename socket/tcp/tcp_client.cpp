//使用TcpSocket类实现tcp的客户端程序
//1、创建套接字
//2、发起连接请求
//3、收发数据
//4、关闭连接
#include"tcp_clientandserver.h"

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    printf("Usage: ./tcp_client ip port");
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSocket sock;
  if(sock.Socket() == false)
  {

    return -1;
  };
  printf("hehe\n");
  if(sock.Connect(ip, port) == false)
  {
    

    return -1;
  }
  while(1)
  {
    std::string buf;
    std::cout << "client say:";
    fflush(stdout);
    std::cin >> buf;
    sock.Send(buf.c_str(), buf.length());//length和size一样
    char rbuf[1024] = {0};
    sock.Recv(rbuf, 1023);
    printf("server say: %s\n", rbuf);


  }
  sock.Close();
  return 0;
}
