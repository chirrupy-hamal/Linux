#include"SeqList.h"

int main()
{
  SeqList seqlist;
  
  //初始化成空表
  SeqListInit(&seqlist);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);

  //尾插
  SeqListPushBack(&seqlist, 2);
  SeqListPushBack(&seqlist, 3);
  SeqListPushBack(&seqlist, 4);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListFull(&seqlist);

  //头插
  SeqListPushFront(&seqlist, 0);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListFull(&seqlist);
  
  //依据下标插入
  SeqListInsert(&seqlist, 1, 1);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListFull(&seqlist);
  
  //尾删
  SeqListPopBack(&seqlist);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);

  //头删
  SeqListPopFront(&seqlist);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);

  //依据下标删除
  SeqListErase(&seqlist, 1);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);
  //SeqListErase(&seqlist, seqlist.size);

  //依据数据删除
  SeqListPushFront(&seqlist, 1);
  SeqListPushFront(&seqlist, 1);
  //删除第一个遇到的data
  SeqListRemove(&seqlist, 0);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);
  SeqListRemove(&seqlist, 1);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);
  //删除遇到的所有data
  SeqListRemoveAll(&seqlist, 1);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);

  //查找
  SeqListFind(&seqlist, 1);
  SeqListFind(&seqlist, 3);

  //修改
  SeqListPrint(&seqlist);
  SeqListAmend(&seqlist,0, 0);
  SeqListPrint(&seqlist);

  //销毁
  SeqListDestroy(&seqlist);
  SeqListPrint(&seqlist);
  SeqListSize(&seqlist);
  SeqListEmpty(&seqlist);

  return 0;
}
