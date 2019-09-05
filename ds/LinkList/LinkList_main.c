#include"LinkList.h"

int main()
{
  ListNode *first;
  
  //初始化
  LinkListInit(&first);
  LinkListPrint(first);
  LinkListSize(first);

  //头插
  LinkListPushFront(&first, 1);
  LinkListPushFront(&first, 0);
  
  //尾插
  LinkListPushBack(&first, 2);
  LinkListPushBack(&first, 4);
  
  //在某一结点前做插入
  ListNode *result = LinkListFind(first, 4);
  LinkListInsert(&first, result, 3);
  LinkListPrint(first);
  LinkListSize(first);

  //头删
  LinkListPopFront(&first);

  //尾删
  LinkListPopBack(&first);

  //删除某一结点
  result = LinkListFind(first, 2);
  LinkListErase(&first, result);
  LinkListFind(first, 2);
  LinkListPrint(first);
  LinkListSize(first);

  //销毁
  LinkListDestroy(&first);
  LinkListPrint(first);
  LinkListSize(first);
  LinkListEmpty(first);

  return 0;
}
