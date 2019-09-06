#pragma once 
#include<stdio.h>
#include<assert.h>
#include<malloc.h>

typedef void* Datatype;

typedef struct LinkQueueNode
{
  Datatype data;
  struct LinkQueueNode *next;
} QNode;

typedef struct LinkQueue
{
  QNode *front;
  QNode *rear;
} Queue;

//初始化
void QueueInit(Queue *pQueue)
{
  assert(pQueue);

  pQueue->front = pQueue->rear = NULL;
}

//销毁
void QueueDestroy(Queue *pQueue)
{
  assert(pQueue);

  QNode *cur, *next;
  for(cur = pQueue->front; NULL != cur; cur = next)
  {
    next = cur->next;
    free(cur);
  }
  pQueue->front = pQueue->rear = NULL;
}

static QNode *Createnode(Datatype data)
{
  QNode *node = (QNode*)malloc(sizeof(QNode));
  assert(node);
  node->data = data;
  node->next = NULL;
  return node;
}
//尾插
void QueuePush(Queue *pQueue, Datatype data)
{
  assert(pQueue);
  
  QNode *newNode = Createnode(data);

  //特殊情况，一个结点都没有
  if(NULL == pQueue->front)
  {
    pQueue->front = pQueue->rear = newNode;
    return;
  }

  //正常情况
  pQueue->rear->next = newNode;
  pQueue->rear = newNode;//修改尾指针
}

//头删
void QueuePop(Queue *pQueue)
{
  assert(NULL != pQueue);
  assert(NULL != pQueue->front);

  QNode *del = pQueue->front;
  pQueue->front = pQueue->front->next;
  free(del);

  //特殊情况，未删除前队列中只剩下一个结点
  if(NULL == pQueue->front)
  {
    pQueue->rear = NULL;
  }
}

//获取在队首的数据
Datatype QueueFront(const Queue *pQueue)
{
  assert(NULL != pQueue);
  assert(NULL != pQueue->front);

  return pQueue->front->data;
}

//查，同链表

//判空
int QueueEmpty(const Queue *pQueue)
{
  return NULL == pQueue->front;
}

//链队列中的数据个数
int QueueSize(const Queue *pQueue)
{
  QNode *cur;
  int size = 0;
  for(cur = pQueue->front; NULL != cur; cur = cur->next)
  {
    ++size;
  }
  printf("队列中的数据个数为：%d\n", size);
  return size;
}
