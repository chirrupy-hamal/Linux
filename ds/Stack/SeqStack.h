#pragma once 
#include<stdio.h>
#include<assert.h>

typedef int DataType;

#define MAXSIZE (100)

typedef struct SeqStack
{
  DataType array[MAXSIZE];
  int top;//1、保存了顺序栈里面已存的数据个数
          //2、当前可用下标
} SeqStack;//对栈顶进行插入/删除操作
           //栈顶插入、删除栈顶

//初始化
void SeqStackInit(SeqStack *pStack)
{
  pStack->top = 0;
}

//销毁
void SeqStackDestroy(SeqStack *pStack)
{
  pStack->top = 0;
}

//插入，也就是顺序表的尾插
void StackPush(SeqStack *pStack, DataType data)
{
  assert(pStack->top < MAXSIZE);
  
  pStack->array[pStack->top++] = data;
}

//删除，也就是顺序表的尾删
void SeqStackPop(SeqStack *pStack)
{
  assert(pStack->top > 0);

  pStack->top--;
}

//获取栈顶元素
DataType SeqStackTop(const SeqStack *pStack)
{
  assert(pStack->top > 0);

  return pStack->array[pStack->top-1];
}

//查，同静态顺序表

//判满
int SeqStackFull(const SeqStack *pStack)
{
  return pStack->top == MAXSIZE;
}

//判空
int SeqStackEmpty(const SeqStack *pStack)
{
  return pStack->top == 0;
}

//顺序栈中的数据个数
int SeqStackSize(const SeqStack *pStack)
{
  return pStack->top;
}

//打印顺序栈
