#include<stdio.h>
#include<stdlib.h>//getenv头文件

//main函数的参数，你可以说0/2/3个。
//argc：命令行参数的个数
//argv：命令行参数具体内容
//env：当前进程对应的环境变量
int main(int argc, char *argv[], char *env[])
{
  (void)argc;
  (void)argv;
  //并不会真正地编译成一些有实在意义的代码，但是能够切实地把警告去掉，
  //编译器说你这参数没有使用啊，在写代码的时候，定义的每个参数和变量都应该被使用才对，
  //如果定义了却又没有使用，那就说明定义的可以去掉，如果这里的argc和argv你就真的不想用，但有又不想有警告。
  
  //其中，argv和env这两个字符串数组的结构是非常相似的，都是在最后一个元素的后面有一个NULL指针作为结束的标记。
#if 0
  for(int i = 0; NULL != env[i]; ++i)
    printf("%s\n", env[i]);//当前进程对应的环境变量，和bash进程的环境变量还是有所不同的。
#endif
  //按照键值对的方式进行查找
  //根据key来查找对应的value
  char *ret = getenv("HOME");//HOME：指定用户的主工作目录，即用户登陆到Linux系统中时，默认的目录。
  //注意：HOME大小写必须得完全匹配，否则是查不到的。
  printf("%s\n", ret);
  //按照键值对的方式进行修改
  putenv("HOME=/"); 
  printf("%s\n", getenv("HOME"));
  return 0;
}
