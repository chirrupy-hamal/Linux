#include"BTree.h"

int main()
{
  int preOrder[] ={1, 5, -1, -1, 2, 3, -1, 4};
  //{1, 2, 3, -1, 4, 5, -1, -1, -1, 6, -1, -1, 7, 8, -1, -1, 9, -1, 10};
  //       1
  //  2         7
  //3    6    8    9
  //  4              10
  // 5 
  //镜像
  //        1
  //   7         2
  // 9    8    6    3
  //10            4
  //               5 
  int size = sizeof(preOrder)/sizeof(int);
  int usedSize;//就是size
  BTreeNode *root = CreateTree(preOrder, size, &usedSize);

  printf("%d\n", usedSize);

  PreOrder(root);
  printf("\n");
  PreOrderLoop(root);
  InOrder(root);
  printf("\n");
  InOrderLoop(root);
  PostOrder(root);
  printf("\n");
  PostOrderLoop(root);
  
  Mirror1(root);
  PreOrderLoop(root);
  Mirror1(root);
  Mirror2(root);
  PreOrderLoop(root);
  Mirror2(root);

  printf("GetSize1:%d\n", GetSize1(root));
  GetSize2(root);
  printf("GetSize2:%d\n", count);
  
  printf("GetLeafSize:%d\n", GetLeafSize(root));

  printf("GetLevel5Size:%d\n", GetLevelkSzie(root, 5));

  printf("GetHeight:%d\n", GetHeight(root));

  BTreeNode *result = Find(root, 1);
  printf("%d\n", result->data);

  printf("IsCompleteTree:%d\n", IsCompleteBinaryTree(root));
}
