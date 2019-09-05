#pragma once 
#include<stdio.h>
#include<malloc.h>
#include<assert.h>

typedef int DataType;

#define INIT_CAPACITY (3)

typedef struct SeqListD
{
  DataType *parray;
  int capacity;//当前容量，等同于静态顺序表的MAXSIZE
  int size;//同静态顺序表
} SeqListD;

//初始化
void SeqListDInit(SeqListD *pSeq)
{
  assert(pSeq);

  //获得容器空间
  pSeq->capacity = INIT_CAPACITY;
  pSeq->parray = (DataType*)malloc(sizeof(DataType) * pSeq->capacity);
  assert(pSeq->parray);

  pSeq->size = 0;
  printf("初始化完成\n");
}

//销毁
void SeqListDDestroy(SeqListD *pSeq)
{
  assert(pSeq);

  free(pSeq->parray);//ANSI C规定free参数可以是NULL，
                     //此时free什么也不做。
  pSeq->parray = NULL;
  pSeq->capacity = 0;
  pSeq->size = 0;
  printf("已销毁\n");
}

//扩容
static void ExpandIfRequire(SeqListD *pSeq)
{
  //这个不属于一个对外的接口，所以把它定义成static，
  //标明一下它不是动态顺序表要抛出去的一个接口，仅是供内部调用它。
  //但是引用该文件的主函数文件还是可以使用此函数的，只是在此标明一下而已，详见static关键字
  
  assert(pSeq);
  assert(pSeq->parray);

  //扩容条件
  if(pSeq->size < pSeq->capacity)
  {
    return;//还没满，此时就不需要扩容
  }

  //扩容
  pSeq->capacity *= 2;//翻一倍;
  
  //申请新空间
  DataType *newarray = (DataType*)malloc(sizeof(DataType) * pSeq->capacity);
  assert(newarray);
  printf("从原来的%d扩容到%d\n", pSeq->capacity, pSeq->capacity * 2);
  //数据搬移
  int i = 0;
  for(; i < pSeq->size; ++i)
  {
    newarray[i] = pSeq->parray[i];
  }
  //释放旧空间，把申请的新空间和动态顺序表关联起来
  free(pSeq->parray);//注意要先释放
  pSeq->parray = newarray;
}

//尾插
void SeqListDPushBack(SeqListD *pSeq, DataType data)
{
  ExpandIfRequire(pSeq);//是否需要进行扩容已经在该函数中实现
  
  pSeq->parray[pSeq->size] = data;
  pSeq->size++;
  printf("尾插数据%d\n", data);
}
//头插
void SeqListDPushFront(SeqListD *pSeq, DataType data)
{
  ExpandIfRequire(pSeq);
 
  int i = 0;
  for(i = pSeq->size - 1; i >= 0; --i)
  {
    pSeq->parray[i+1] = pSeq->parray[i];
  }

  pSeq->parray[0] = data;
  pSeq->size++;
  printf("头插数据%d\n", data);
}
//依据下标插入
void SeqListDInsert(SeqListD *pSeq, int pos, DataType data)
{
  assert(pos >= 0 && pos <= pSeq->size);

  ExpandIfRequire(pSeq);

  int i = 0;
  for(i = pSeq->size - 1;  i >= pos; --i)
  {
    pSeq->parray[i+1] = pSeq->parray[i];
  }

  pSeq->parray[pos] = data;
  pSeq->size++;
  printf("在表中下标位置为%d处插入数据%d\n", pos, data);
}

//尾删
void SeqListDPopBack(SeqListD *pSeq)
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
void SeqListDPopFront(SeqListD *pSeq)
{
  assert(pSeq);
  assert(pSeq->parray);

  if(0 == pSeq->size)
  {
    printf("表已空，无法进行头删\n");
  }

  int i = 0;
  for(i = 1; i <= pSeq->size - 1; ++i)
  {
    pSeq->parray[i-1] = pSeq->parray[i];
  }

  pSeq->size--;
  printf("头删\n");
}
//依据下标删除
void SeqListDErase(SeqListD *pSeq, int pos)
{
  assert(pSeq);
  assert(pSeq->parray);
  assert(pSeq->size > 0);
  assert(pos >= 0 && pos < pSeq->size);

  int i = 0;
  for(i = pos + 1; i <= pSeq->size - 1;  ++i)
  {
    pSeq->parray[i-1] = pSeq->parray[i];
  }

  pSeq->size--;
  printf("已删除表中下标位置为%d的数据\n", pos);
}

//查找，找到了就返回第一个遇到的下标；没找到返回-1
int SeqListDFind(SeqListD *pSeq, DataType data)
{
  assert(pSeq);
  assert(pSeq->parray);

  int i = 0;
  for(; i < pSeq->size; ++i)
  {
    if(data == pSeq->parray[i])
    {
      printf("数据%d在表中的下标位置是%d\n", data, i);
      return i;
    }
  }

  printf("数据%d在表中不存在\n", data);
  return -1;
}

//依据数据删除
//1、删除第一个遇到的data
void SeqListDRemove(SeqListD *pSeq, DataType data)
{
    int pos = SeqListDFind(pSeq, data);
    if(-1 == pos)
    {
      printf("删除失败\n");
      return;
    }

    SeqListDErase(pSeq, pos);
    printf("已删除表中第一个%d数据\n", data);
}
//2、删除遇到的所有data
void SeqListDRemoveAll(SeqListD *pSeq, DataType data)
{
  assert(pSeq);
  assert(pSeq->parray);

  int i = 0, j = 0;
  for(; i < pSeq->size; ++i)
  {
    if(data != pSeq->parray[i])
    {
      pSeq->parray[j++] = pSeq->parray[i];
    }
  }

  pSeq->size = j;
  printf("已删除表中所有%d数据\n", data);
}

//修改，依据下标修改数据
void SeqListDAmend(SeqListD *pSeq, int pos, DataType data)
{
  assert(pSeq);
  assert(pSeq->parray);
  assert(pSeq->size > 0);
  assert(pos >= 0 && pos <= pSeq->size);

  pSeq->parray[pos] = data;
  printf("表中%d位置的数据已被修改为%d\n", pos, data);
}

//判空
int SeqListDEmpty(const SeqListD *pSeq)
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

//动态顺序表中的数据个数
int SeqListDSize(const SeqListD *pSeq)
{
  assert(pSeq);
  
  printf("表中数据个数为%d\n", pSeq->size);

  return pSeq->size;
}

//打印动态顺序表
void SeqListDPrint(const SeqListD *pSeq)
{
  assert(pSeq);
  assert(pSeq->parray);

  if(0 == pSeq->size)
  {
    printf("空表\n");
    return;
  }

  printf("表中数据是：");
  int i = 0;
  for(; i < pSeq->size; ++i)
  {
    printf("%d ", pSeq->parray[i]);
  }
  printf("\n");
}
