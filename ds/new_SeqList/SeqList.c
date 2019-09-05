#include<stdio.h>//NULL也定义在内
#include<assert.h>

#include"SeqList.h"

//初始化
void SeqListInit(SeqList *pSeq)
{
  assert(NULL != pSeq);
  
  pSeq->size = 0;
  printf("初始化完成\n");
}

//销毁
void SeqListDestroy(SeqList *pSeq)
{
  assert(pSeq);

  pSeq->size = 0;
  printf("已销毁\n");
}

//尾插
void SeqListPushBack(SeqList *pSeq, DataType data)
{
  assert(pSeq);

  //特殊情况，表已满
  if(MAXSIZE == pSeq->size)
  {
    printf("表已满，无法进行尾插\n");
    assert(0);//极端一点，终止程序运行
  }

  //正常情况
  pSeq->array[pSeq->size] = data;
  pSeq->size++;
  printf("尾插数据%d\n", data);
}
//头插
void SeqListPushFront(SeqList *pSeq, DataType data)
{
  assert(pSeq);

  if(MAXSIZE == pSeq->size)
  {
    printf("表已满，无法进行头插\n");
    assert(0);
  }

  //[size-1, 0]->[size, 1]
  int i = 0;
  for(i = pSeq->size - 1; i >= 0; --i)
  {
    pSeq->array[i+1] = pSeq->array[i];
  }

  pSeq->array[0] = data;
  pSeq->size++;
  printf("头插数据%d\n", data);
}
//依据下标插入
void SeqListInsert(SeqList *pSeq, int pos, DataType data)
{
  assert(pSeq);
  assert(pSeq->size < MAXSIZE);
  assert(pos >= 0 && pos <= pSeq->size);

  int i = 0;
  for(i = pSeq->size - 1; i >= pos; --i)
  {
    pSeq->array[i+1] = pSeq->array[i];
  }

  pSeq->array[pos] = data;
  pSeq->size++;
  printf("在表中下标位置为%d处插入数据%d\n", pos, data);
}

//尾删
void SeqListPopBack(SeqList *pSeq)
{
  assert(pSeq);

  //特殊情况，表为空
  if(0 == pSeq->size)
  {
    printf("表已空，无法进行尾删\n");
    assert(0);
  }

  //正常情况
  pSeq->size--;
  printf("尾删\n");
}
//头删
void SeqListPopFront(SeqList *pSeq)
{
  assert(pSeq);

  if(pSeq->size == 0)
  {
    printf("表已空，无法进行头删\n");
    assert(0);
  }

  //[1, size-1]->[0, size-2]
  int i = 0;
  for(i = 1; i <= pSeq->size - 1; ++i)
  {
    pSeq->array[i-1] = pSeq->array[i];
  }

  pSeq->size--;
  printf("头删\n");
}
//依据下标删除
void SeqListErase(SeqList *pSeq, int pos)
{
  assert(pSeq);
  assert(pSeq->size > 0);
  assert(pos >= 0 && pos < pSeq->size);
  //注意：pos不能等于pSeq->size
    
  int i = 0;
  for(i = pos + 1; i <= pSeq->size - 1; ++i)
  {
    pSeq->array[i-1] = pSeq->array[i];
  }

  pSeq->size--;
  printf("已删除表中下标位置为%d的数据\n", pos);
}
//依据数据删除
//1、删除第一个遇到的data
void SeqListRemove(SeqList *pSeq, DataType data)
{
  int pos = SeqListFind(pSeq, data);
  if(-1 == pos)
  {
    printf("删除失败\n");
    return;
  }

  SeqListErase(pSeq, pos);
  printf("已删除表中第一个%d数据\n", data);

}
//2、删除遇到的所有data
void SeqListRemoveAll(SeqList *pSeq, DataType data)
{
  assert(pSeq);

  int i = 0, j = 0;
  for(; i < pSeq->size; ++i)
  {
    if(data != pSeq->array[i])
    {
      pSeq->array[j++] = pSeq->array[i];
    }
  }

  pSeq->size = j;
  printf("已删除表中所有%d数据\n", data);
}

//查找，找到了就返回第一个遇到的下标；没找到返回-1
int SeqListFind(SeqList *pSeq, DataType data)
{
  //二分查找的前提是有序的
  //顺序遍历查找
  assert(pSeq);

  int i = 0;
  for(; i < pSeq->size; ++i)
  {
    if(data == pSeq->array[i])
    {
      printf("数据%d在表中的下标位置是%d\n", data, i);
      return i;
    }
  }

  printf("数据%d在表中不存在\n", data);
  return -1;
}

//修改，依据下标修改数据
void SeqListAmend(SeqList *pSeq, int pos, DataType data)
{
  assert(pSeq);
  assert(pSeq->size > 0);
  assert(pos >= 0 && pos <= pSeq->size);

  pSeq->array[pos] = data;
  printf("表中%d位置的数据已被修改为%d\n", pos, data);
}

//判满
int SeqListFull(const SeqList *pSeq)
{
  assert(pSeq);

  if(MAXSIZE == pSeq->size)
  {
    printf("表已满\n");
    return 1;//表示满
  }
  
  printf("表没满\n");
  return 0;
}

//判空
int SeqListEmpty(const SeqList *pSeq)
{
  assert(pSeq);

  if(0 == pSeq->size)
  {
    printf("表为空\n");
    return 1;//表示空
  }
  
  printf("表没空\n");
  return 0;
}

//静态顺序表中的数据个数
int SeqListSize(const SeqList *pSeq)
{
  assert(pSeq);
  
  printf("表中数据个数为%d\n", pSeq->size);

  return pSeq->size;
}

//打印静态顺序表
void SeqListPrint(const SeqList *pSeq)
{
  assert(pSeq);

  if(0 == pSeq->size)
  {
    printf("表中无任何数据，空表\n");
    return;
  }

  printf("表中数据是：");
  int i = 0;
  for(; i < pSeq->size; ++i)
  {
    printf("%d ", pSeq->array[i]);
  }
  printf("\n");
}
