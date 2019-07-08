//使用封装好的TcpSocket类实现tcp服务端程序
#include"tcp_clientandserver.h"
int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    printf("Usage: ./tcp_server ip port\n");
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  
//1、创建套接字
  TcpSocket sock;
  if(sock.Socket() == false)
  {
    return -1;
  }
  //CHECK_RET(sock.Socket());

//2、为套接字绑定地址信息
  CHECK_RET(sock.Bind(ip, port));
//3、开始监听
  CHECK_RET(sock.Listen());
//4、获取连接
  while(1)
  {
    TcpSocket clientsocket;//放在外面的话代表只能接收一个客户端

    //获取新连接，假如现在没有的话阻塞，实际上客户端的第二条数据已经发送了，只不过这边没有接收
    //当你在开一个窗口运行client的时候，相当于是把第一个clientsocket给丢了，并且我们也没有关闭它，会造成描述符的浪费
    //那会不会造成不能有更多的连接？？？
    if(sock.Accept(&clientsocket) == false)
    {
      continue;
    }
    char buff[1024] = {0};
    clientsocket.Recv(buff, 1023);
    printf("client say: %s\n", buff);
    memset(buff, 0x00, 1024);//清空buff
    printf("server say: ");
    fflush(stdout);
    scanf("%s",buff);
    clientsocket.Send(buff, strlen(buff));
    //clientsocket.Close();//但是现在就想和一个客户端多次通信？多进程和多线程。
  }
  sock.Close();
//5、收发数据
//6、关闭套接字

    return 0;
}
