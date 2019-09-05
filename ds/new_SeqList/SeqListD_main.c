#include"SeqListD.h"

int main()
{
  SeqListD seqlistD;
  
  //初始化成空表
  SeqListDInit(&seqlistD);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);

  //尾插
  SeqListDPushBack(&seqlistD, 2);
  SeqListDPushBack(&seqlistD, 3);
  SeqListDPushBack(&seqlistD, 4);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);

  //头插
  SeqListDPushFront(&seqlistD, 0);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  
  //依据下标插入
  SeqListDInsert(&seqlistD, 1, 1);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  
  //尾删
  SeqListDPopBack(&seqlistD);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);

  //头删
  SeqListDPopFront(&seqlistD);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);

  //依据下标删除
  SeqListDErase(&seqlistD, 1);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);
  //SeqListDErase(&seqlistD, seqlistD.size);

  //依据数据删除
  SeqListDPushFront(&seqlistD, 1);
  SeqListDPushFront(&seqlistD, 1);
  //删除第一个遇到的data
  SeqListDRemove(&seqlistD, 0);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);
  SeqListDRemove(&seqlistD, 1);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);
  //删除遇到的所有data
  SeqListDRemoveAll(&seqlistD, 1);
  SeqListDPrint(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);

  //查找
  SeqListDFind(&seqlistD, 1);
  SeqListDFind(&seqlistD, 3);

  //修改
  SeqListDPrint(&seqlistD);
  SeqListDAmend(&seqlistD,0, 0);
  SeqListDPrint(&seqlistD);

  //销毁
  SeqListDDestroy(&seqlistD);
  SeqListDSize(&seqlistD);
  SeqListDEmpty(&seqlistD);

  return 0;
}
