//一个基于udp协议的网络通信服务端程序

#include<stdio.h>
#include<unistd.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>//也是memset的头文件
#include<arpa/inet.h>

int main()
{
  //1、创建套接字man socket
  //头文件<sys/socket.h>
  //int socket(int domain, int type, int protocol);
  //domain：地址域，AF_INET IPV4的网络协议；AF_INET6 IPV6的网络协议。
  //type：套接字类型，SOCK_STREAM 流式套接字，默认协议是tcp；SOCK_DGRAM 数据报套接字，默认协议是udp。
  //protocol：协议，tcp/udp，如果前面是数据报套接字，协议只能用udp，同理tcp也是如此。
  //          流式套接字默认给0就是tcp；
  //          数据报套接字默认给0就是udp。
  //          vim /usr/include/netinet/in.h
  //          IPPROTO_TCP(宏，6) tcp协议；
  //          IPPROTO_UDP(宏，17) udp协议。
  //返回值：成功，返回新socket的文件描述符，这个文件描述符也是这个socket操作的句柄。
  //        成功，套接字描述符(非负整数)；失败，-1。
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sockfd < 0)
  {
    perror("socket error\n");
    return -1;
  }
  
  //2、为套接字绑定地址信息man 2 bind
  //int bind(int sockfd, const struct socksddr *addr, socklen_t addrlen);
  //sockfd：套接字描述符。
  //addr：地址信息。
  //      struct sockaddr{sa_family_t sa_family; char sa_data[14];}
  //      其中，sa_family是用来存放地址域的，4字节；sa_data是用来数据填充的。
  //      struct sockaddr_in  //IPV4的地址信息结构
  //      {
  //        __SOCKADDR_COMMON (sin_);//预留4个字节的地址域
  //        in_port_t sin_port;//其中，typedef uint16_t in_port_t; sin_port是端口号
  //        struct in_addr sin_addr;//网络地址
  //        unsigned char sin_zero[sizeof(struct sockaddr) -
  //                               __SOCKADDR_COMMON_SIZE -
  //                               sizeof(in_port_t) -
  //                               sizeof(struct in_addr)];//真正剩下的字节数，专门做填充的。
  //      };
  //      struct in_addr
  //      {
  //        in_addr_t s_addr;//其中，typedef uint32_t in_addr_t;
  //      };
  //addrlen：地址信息的长度。
  //返回值：成功，0；失败，-1。
  
  //先定义地址信息
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9000);//推荐不要使用0~1024之间的，因为已经被操作系统预留出来给某些特殊的协议去使用。
                       //咱们能用，只是不推荐使用。
                       //网络字节序
                       //man htons
                       //uint16_t htons(uint16_t hostshort); 从主机字节序转换成网络字节序的无符号短整型数据。
                       //htons()等函数在内部都已经实现了判断，因为大端的话就不用转换了。
//在早期的32位操作系统上，long型数据是4字节的。
//现在64位操作系统的long型数据已经是8字节了，就不能再使用htonl()函数进行转换了。

                       //uint32_t htonl(uint32_t hostlong);
  addr.sin_addr.s_addr =htonl(0xac1191ee);//我们能够绑定的地址必须是我们电脑上所有的地址，ifconfig。
                                          //网络字节序。
                                          //man inet_addr
                                          //in_addr_t inet_addr(const char *cp); 将一个点分十进制字符串的ip地址转换成网络字节序的ip地址。
  addr.sin_addr.s_addr = inet_addr("172.17.145.238");
  socklen_t len = sizeof(struct sockaddr_in);
  int ret = bind(sockfd, (struct sockaddr*)&addr, len);
  if(ret < 0)
  {
    perror("bind error\n");
    return -1;
  }

  //3、接收数据
  //man recvfrom
  //ssize_t(signed int) recvfrom(int sockfd, void *buf, size_t len, int flags, 
  //                                struct sockaddr *src_add, socklen_t *addrlen );
  //sockfd：套接字描述符。通过描述符找到socked结构体，进而找到缓冲区队列，然后从缓冲区队列当中去取到数据。
  //buf：接收数据之后要放到哪一块缓冲区里面。
  //len：要接收的数据长度。
  //flags：标志位，一般情况下我们默认给0，代表的是阻塞接收：没有数据就在这等着，直到有数据了并且接收了为止。
  //src_addr：地址信息，用于获取这些数据是谁发的。
  //addrlen：地址信息的长度，输入输出符合型参数。
  //         最终该参数是真正的地址信息长度。而且真正的地址信息还会被放到src_addr参数中。
  //返回值是实际的接收长度，len只是我们想要确定，说，我想获取多少多少字节的数据，但缓冲区没有那么多，那么缓冲区实际的字节数就是返回值。失败，-1。
  while(1)
  {
  char buff[1024] = {0};
  struct sockaddr_in client_addr;
  recvfrom(sockfd, buff, 1023, 0, (struct sockaddr*)&client_addr, &len);
  printf("client say：%s\n", buff);
  

  //4、发送数据
  memset(buff, 0x00, 1024);//清空buff
  scanf("%s", buff);
  //man sendto 
  //ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, 
  //               const struct sockaddr *dest_addr, socklen_t addrlen);
  //len：要发送的数据长度。
  //flags：默认给0，阻塞发送。
  //dest_addr：目的主机地址信息。
  sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&client_addr, len);
  }

  //5、关闭套接字
  close(sockfd);

  return 0;
}
