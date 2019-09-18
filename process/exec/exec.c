#include<stdio.h>
#include<unistd.h>

int main(){
#if 0
  execl("/usr/bin/ls", "ls", "-l", "/", NULL);
  //ls的路径，通过代码完成这样一个操作：ls -l /
  //其中，argv0是"ls"，argv1是"-l"，argv2是"/"，
  //execl要求说虽然我这是一个变长的参数列表，但得用一个空指针作为命令行参数的结束标记，
  //如果没有，就会出现一些不可预期的问题，可能是会提示些什么东西，也可能直接就运行崩溃了。
  printf("=======================================");
  //为什么没有执行到？
  //execl后，printf那些代码都被替换没了，也就执行不到printf了。

  execlp("ls", "ls", "-l", "/", NULL);
  //lp和l替换后环境变量长成什么样？程序替换的是代码和数据,
  //环境变量也在虚拟地址空间中，在栈的更高地址位置上，环境变量没动啊，说明和原来是一样的。
  //exec如果执行成功了是没有返回值的，执行失败了，返回-1，
  //因为代码都被替换掉了，用来保存返回值的那个代码也都没了。
#endif
  char* env[] = {"AAAA=aaaa", NULL};//构造环境变量，不光是构造argv[]要用空指针结尾，构造环境变量也得加上NULL。 
  //execle("./exec_test", "./test", NULL, env);

  //char* argv[] = {"ls", "-l", "/", NULL};//命令行参数
  //execv("/usr/bin/ls", argv);
  //execvp("ls", argv);
  char* argv[] = {"./exec_test", NULL};
  execve("./exec_test", argv, env);
  return 0;
}
