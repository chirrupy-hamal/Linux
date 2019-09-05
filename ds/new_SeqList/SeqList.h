#pragma once

typedef int DataType;

#define MAXSIZE (100)

typedef struct SeqList
{
  DataType array[MAXSIZE];
  int size;//1、保存了静态顺序表里面已存的数据个数
           //2、当前可用下标
} SeqList;

//初始化成空表
void SeqListInit(SeqList *pSeq);
//需要借助此函数修改静态顺序表，也就是结构体的内部值，
//所以传指针，而且指针也更省空间。

//销毁，销毁后也成空表
void SeqListDestroy(SeqList *pSeq);

//*********************************************************
//增/删/查/改
//*********************************************************

//尾插
void SeqListPushBack(SeqList *pSeq, DataType data);
//头插
void SeqListPushFront(SeqList *pSeq, DataType);
//依据下标插入
void SeqListInsert(SeqList *pSeq, int pos, DataType data);

//尾删
void SeqListPopBack(SeqList *pSeq);
//头删
void SeqListPopFront(SeqList *pSeq);
//依据下标删除
void SeqListErase(SeqList *pSeq, int pos);
//依据数据删除
//1、删除第一个遇到的data
void SeqListRemove(SeqList *pSeq, DataType data);
//2、删除遇到的所有data
void SeqListRemoveAll(SeqList *pSeq, DataType data);

//查找，找到了就返回第一个遇到的下标；没找到返回-1
int SeqListFind(SeqList *pSeq, DataType data);

//修改，依据下标修改数据
void SeqListAmend(SeqList *pSeq, int pos, DataType data);

//静态顺序表是否已满
int SeqListFull(const SeqList *pSeq);
//静态顺序表是否为空
int SeqListEmpty(const SeqList *pSeq);
//静态顺序表中的数据个数
int SeqListSize(const SeqList *pSeq);
//打印静态顺序表
void SeqListPrint(const SeqList *pSeq);
