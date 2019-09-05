#pragma once 
#include<stdio.h>
#include<assert.h>
#include<malloc.h>

typedef int DataType;

typedef struct LinkListNode
{
  DataType data;
  struct LinkListNode *next; 
} ListNode;

//无头结点的单链表，此时的头指针(pfirst)指向的是单链表中的第一个结点，而不是头结点

//初始化
void LinkListInit(ListNode **ppfirst)//注意形参类型
{
  assert(ppfirst);
  
  *ppfirst = NULL;//无头结点的单链表初始化就是pfirst = NULL;
                  //若是有头结点的单链表，其初始化为pfirst->next = NULL;
  printf("已完成初始化\n");
}

//销毁
void LinkListDestroy(ListNode **ppfirst)
{
  assert(ppfirst);
  
  if(NULL == *ppfirst)
  {
    printf("链表已空，无需销毁\n");
    return;
  }

  ListNode *del = NULL;
  while(NULL != *ppfirst)
  { 
    del = *ppfirst;
    *ppfirst = (*ppfirst)->next;//注意优先级
    free(del);
  }

  //程序走到这，*ppfirst == NULL
  printf("链表已销毁\n");
}

//插入
static ListNode *CreateNode(DataType data)
{
  ListNode *newNode = (ListNode*)malloc(sizeof(ListNode));
  assert(newNode);
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}
//头插
void LinkListPushFront(ListNode **ppfirst, DataType data)
{
  assert(ppfirst);
  //在此是不用断言*ppfirst的，
  //*ppfirst == NULL 就是空链表，执行下面的代码是没问题的，

  ListNode *newNode = CreateNode(data);
  newNode->next = *ppfirst;//因为是头插
  *ppfirst = newNode;
  printf("数据%d已头插链表中\n", data);
}
//尾插
void LinkListPushBack(ListNode **ppfirst, DataType data)
{
  assert(ppfirst);

  ListNode *newNode = CreateNode(data);

  //特殊情况，链表为空
  if(NULL == *ppfirst)
  {
    *ppfirst = newNode;
    printf("数据%d已尾插链表中\n", data);
    return;
  }

  //正常情况
  ListNode *cur = *ppfirst;
  //找最后一个结点
  while(NULL != cur->next)
  {
    cur = cur->next;
  }
  cur->next = newNode;
  printf("数据%d已尾插链表中\n", data);
}
//在某一结点前做插入
//前提pos结点得在链表中，也就是pos不为NULL，也就是链表不为空
void LinkListInsert(ListNode **ppfirst, ListNode *pos, DataType data)
{
  //这里传二级指针是为了处理一旦pos就是链表的第一个结点的话，
  //需要对first结点的pfirst指针做修改，所以传二级指针。
  assert(ppfirst);
  assert(*ppfirst);//链表不能为空
  assert(pos);

  //特殊情况，链表中只有一个结点
  if(*ppfirst == pos)
  {
    ListNode *newNode = CreateNode(data);
    newNode->next = *ppfirst;
    *ppfirst = newNode;
    printf("数据%d已插入链表中\n", data);
    return;
  }

  //正常情况
  ListNode *cur = *ppfirst;
  //找pos结点的前一个结点
  while(cur->next != pos)
  {
    cur = cur->next;
  }
  ListNode *newNode = CreateNode(data);
  newNode->next = pos;
  cur->next = newNode;
  printf("数据%d已插入链表中\n", data);
}

//头删
void LinkListPopFront(ListNode **ppfirst)
{
  assert(ppfirst);
  assert(*ppfirst);//不能是空链表

  ListNode *del = *ppfirst;
  *ppfirst = del->next;
  free(del);//谨记
  printf("头删\n");
}
//尾删
void LinkListPopBack(ListNode **ppfirst)
{
  assert(ppfirst);
  assert(*ppfirst);//不能是空链表

  //特殊情况，链表中只有一个结点
  if(NULL == (*ppfirst)->next)
  {
    free(*ppfirst);
    *ppfirst = NULL;
    printf("尾删\n");
    return;
  }

  //正常情况
  ListNode *del;
  ListNode *cur = *ppfirst;
  while(NULL != cur->next->next)
  {
    cur = cur->next;
  }
  del = cur->next;
  cur->next = NULL;
  free(del);
  //或者
  //free(cur->next); cur->next = NULL; 也就不再需要del变量了
  printf("尾删\n");
}
//删除指定结点
//前提pos结点得在链表中，也就是pos不为NULL，也就是链表不为空
void LinkListErase(ListNode **ppfirst, ListNode *pos)
{
  assert(ppfirst);
  assert(*ppfirst);
  assert(pos);

  //特殊情况，链表中只有一个结点
  if(*ppfirst == pos)
  {
    free(*ppfirst);
    *ppfirst = NULL;
    printf("已删除表指定某结点\n");
    return;
  }

  //正常情况
  ListNode *cur = *ppfirst;
  //找pos结点的前一个结点
  while(cur->next != pos)
  {
    cur = cur->next;
  }
  cur->next = pos->next;
  free(pos);
  printf("已删除表中指定结点\n");
}

//查找
ListNode *LinkListFind(ListNode *pfirst, DataType data)
{
  assert(pfirst);

  //链表即便有序，也无法进行二分查找
  //顺序遍历链表
  ListNode *cur = pfirst;
  for(; cur != NULL; cur = cur->next)
  {
    if(data == cur->data)
    {
      printf("在表中找到data为%d的结点\n", data);
      return cur;//返回第一次在链表中遇到的结点
    }
  }
 
  printf("在表中没有找到data为%d的结点\n", data);
  return NULL;
}

//调用查找函数，对找到的结点进行数据的修改

//判空
int LinkListEmpty(const ListNode *pfirst)
{
  if(NULL == pfirst)
  {
    printf("表为空\n");
  }
  return NULL == pfirst;
}

//单链表中的数据个数
int LinkListSize(const ListNode *pfirst)
{
  int i = 0;
  const ListNode *cur = pfirst;
  for(; cur != NULL; cur = cur->next)
  {
    i++;
  }
  printf("表中数据个数为%d\n", i);
  return i;
}

//打印单链表
void LinkListPrint(const ListNode *pfirst)
{
  if(NULL == pfirst)
  {
    printf("表中没有任何数据\n");
    return;
  }

  //顺序遍历链表
  printf("链表中的数据是：");
  const ListNode *cur = pfirst;
  for(; cur != NULL; cur = cur->next)
  {
      printf("%d ", cur->data);
  }
  printf("\n");
}
