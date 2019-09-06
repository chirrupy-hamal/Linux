#pragma once 
#include<malloc.h>
#include<assert.h>

#include"LinkQueue.h"
#include"SeqStack.h"

typedef int DataType;

typedef struct BTreeNode
{
  DataType data;
  struct BTreeNode *left;
  struct BTreeNode *right;
} BTreeNode;

BTreeNode *CreateNode(DataType data)
{
  BTreeNode *node = (BTreeNode*)malloc(sizeof(BTreeNode));
  node->data = data;
  node ->left = node->right = NULL;
  return node;
}

//使用先序创建二叉树，但前提是把空结点也要加进数组中去
//在数组中，用-1代表空结点
//为什么要把空结点也要加入到数组中？
//原因是，惟一创建一棵二叉树，
//也就是说最终创建的二叉树就是你想要的，不可能是其他样式的。
//例如一棵二叉树是如下所示的样子
//   A
// B   C 
//D   E  F
//此棵二叉树的先序是A、B、D、C、E、F
//但是在没有添加空结点的前提下，先序序列也有可能还原成如下所示的另一颗二叉树
//     A
//    B 
//   D 
//  C 
// E 
//F
//还原出来的也就不是你原本期待的样子 

//中序不能创建二叉树，后序可以，
//例如：
//  A
//B   E
// C
//D
//这样一棵二叉树的中序是#B#D#C#A#E，其中#代表空
//而这样的一个中序序列还能还原出另一棵二叉树
//    E
//   A
//  C
// D 
//B
//所以中序是不能用来创建二叉树的
//哈使用上述代二叉树为例，它的后序是：###D#CB##EA
//我们把这个后序序列反转一下，得到：AE##BC#D###，先访问根节点，再右子树，再左子树

//二叉树的带空结点的先序可以完成二叉树的创建/拷贝
//二叉树的带空结点的后序可以完成二叉树的销毁

//两个返回值
//1、创建出的二叉树的根结点
//2、创建子树的过程中，使用的数据个数，这样才能知道数组从哪开始传参
BTreeNode *CreateTree(int preOrder[], int size, int *pUsedSize)
{
  BTreeNode *root;
  int leftUse, rightUse;//左右子树用掉的数据个数

  //出口1
  int rootValue = preOrder[0];
  if(-1 == rootValue)//也就是所谓的空结点
  {
    *pUsedSize = 1;//用掉一个数据
    return NULL;
  }

  //出口2
  //这个出口在数组中数据个数为0的时候才起作用
  if(0 == size)
  {
    *pUsedSize = 0;
    return NULL;
  }

  root = CreateNode(rootValue);//空结点根本就没有加入到二叉树中

  //用先序写的
  root->left = CreateTree(preOrder+1, size-1, &leftUse);
  root->right = CreateTree(preOrder+1+leftUse, size-1-leftUse, &rightUse);
  
  //用后序写的
  //root->right = CreateTree(preOrder+1, size-1, &rightUse);
  //root->left = CreateTree(preOrder+1+rightUse, size-1-rightUse, &leftUse);
  
  //上述中+1是因为在本次调用时使用了一个数据，也就是给root使用了

  //向上一层调用者汇报本次调用所使用的数据个数
  *pUsedSize = 1 + leftUse + rightUse;
  return root;
}

//前序遍历
void PreOrder(BTreeNode *root)
{
  //出口，还有就是如果是空树的话，那就啥都没有
  if(NULL == root)
  {
    return;
  }

  printf("%d ", root->data);
  PreOrder(root->left);
  PreOrder(root->right);
}

//中序
void InOrder(BTreeNode *root)
{
  if(NULL == root)
  {
    return;
  }

  InOrder(root->left);
  printf("%d ", root->data);
  InOrder(root->right);
}

//后序
void PostOrder(BTreeNode *root)
{
  if(NULL == root)
  {
    return;
  }

  PostOrder(root->left);
  PostOrder(root->right);
  printf("%d ", root->data);
}

//层序
//层序遍历是一种广度优先遍历(搜索)
//广度优先遍历(搜索)是用队列来实现的
void LevelOrder(BTreeNode *root)
{
  Queue queue;
  QueueInit(&queue);

  if(NULL == root)
  {
    return;
  }

  QueuePush(&queue, root);//放的是结点的地址，不是结点的data

  BTreeNode *front;
  while(!QueueEmpty(&queue))
  {
    front = (BTreeNode*)QueueFront(&queue);
    QueuePop(&queue);

    //front去拉下线，空结点是不用的
    //一定要先左后右，因为是层序遍历
    if(NULL != front->left)
    {
      QueuePush(&queue, front->left);
    }
    if(NULL != front->right)
    {
      QueuePush(&queue, front->right);
    }

    //层序遍历front结点
    printf("%d ", front->data);
  }
  printf("\n");

  //销毁队列，其实走到这，队列已经为空了，也就无需销毁了
}

//求二叉树的结点个数
//1、用子问题的方式进行求解
int GetSize1(BTreeNode *root)
{
  //出口，还有就是如果是空树的话，那就啥都没有
  if(NULL == root)
  {
    return 0;
  }

  //当子树只有一个结点的时候，就是叶子结点，
  //可以再加一个叶子结点的判断，也可以不写，
  //因为+1就已经是把叶子结点的高度加进去了，
  //只不过写的话就节省两次的函数调用 
  return GetSize1(root->left) + GetSize1(root->right) + 1;
  //或者
  //int left = GetSize1(root->left);
  //int right = GetSize2(root->right);
  //return left + right + 1;
}
//2、用遍历求解，任意一种遍历方式均可以
//此函数以后序为例

int count = 0;//使用这种遍历方式求解的话，需要用到全局变量，
//想要再计算一下结点个数，得先把count清零，
//所以说，这中遍历求解的方式不是很好。
void GetSize2(BTreeNode *root)
{
  if(NULL == root)
  {
    return;
  }

  GetSize2(root->left);
  GetSize2(root->right);
  ++count;
}

//求叶子结点的个数
int GetLeafSize(BTreeNode *root)
{
  //出口1，还有就是如果是空树的话，那就啥都没有
  if(NULL == root)
  {
    return 0;
  }

  //出口2
  if(NULL == root->left && NULL == root->right)
  {
    return 1;
  }

  return GetLeafSize(root->left) + GetLeafSize(root->right);
}

//求第k层上的结点个数
int GetLevelkSzie(BTreeNode *root, int k)
{
  assert(k >= 1);

  //出口1，还有就是如果是空树的话，那就啥都没有
  if(NULL == root)
  {
    return 0;
  }

  if(1 == k)
  {
    return 1;//任何子树的第一层都只有根结点这一个结点
  }

  //原问题的第k层是子问题的第k-1层
  int left = GetLevelkSzie(root->left, k-1);
  int right = GetLevelkSzie(root->right, k-1);
  return left + right;
}

//求树的高度/深度
#define MAX(a, b) ((a) > (b) ? (a) : (b))//用所有子树中最高的那个
int GetHeight(BTreeNode *root)
{
  //出口，还有就是如果是空树的话，那就啥都没有
  if(NULL == root)
  {
    return 0;
  }

  //当子树只有一个结点的时候，就是叶子结点，
  //可以再加一个叶子结点的判断，也可以不写，
  //因为+1就已经是把叶子结点的高度加进去了，
  //只不过写的话就节省两次的函数调用
  return MAX(GetHeight(root->left), GetHeight(root->right))+1;
}

//查找某一结点是否在二叉树中存在
//前提是，二叉树中的data不能重复
//如果找到了，返回结点地址；没找到的话，就返回NULL

//先匹配根，再左子树，再右子树
BTreeNode *Find(BTreeNode *root, DataType data)
{
  //出口1，还有就是如果是空树的话，那就啥都没有
  if(NULL == root)
  {
    return NULL;
  }

  //出口2
  if(root->data == data)
  {
    return root;
  }

  BTreeNode *result = Find(root->left, data);
  if(NULL != result)
  {
    //在左子树中找到了
    return result;
  }

  result = Find(root->right, data);
  if(NULL != result)
  {
    //在右子树中找到了
    return result;
  }

  return NULL;
}

//判断一棵树是不是完全二叉树，这其实就是层序遍历的变形
//使用层序遍历的方法，只不过还要把空结点加入到队列中，
//一旦NULL出现在队列中，那么它的后面也只能是NULL，
//这就是完全二叉树，而且空树既是完全二叉树，又是满二叉树。
int IsCompleteTree(BTreeNode *root)
{
  Queue queue;
  QueueInit(&queue);

  if(NULL == root)
  {
    return 1;
  }

  QueuePush(&queue, root);

  BTreeNode *front;
  while(!QueueEmpty(&queue))
  {
    front = (BTreeNode*)QueueFront(&queue);
    QueuePop(&queue);
    if(NULL == front)
    {
      break;//如果是完全二叉树的话，剩下的队列结点应全为NULL
    }

    QueuePush(&queue, front->left);
    QueuePush(&queue, front->right);
  }

  while(!QueueEmpty(&queue))
  {
    front = (BTreeNode*)QueueFront(&queue);
    QueuePop(&queue);
    if(NULL != front)
    {
      //一旦有不是NULL出现，那么就不是完全二叉树
      //1、先销毁队列
      QueueDestroy(&queue);
      //2、退出函数
      return 0;
    }
  }

  //销毁队列，其实走到这，队列已经为空了，也就无需销毁了
  return 1;
}

//非递归的前、中、后序，都是深度优先遍历(搜索)，用栈
//广度优先遍历(搜索)：例如说此时有3条路，
//走第一步的时候每条路都试一下，
//第二步也是如此。层序就是典型的一种

//深度优先遍历(搜索)：一条路走到黑，走不下去了，
//原路返回再走下一条路。迷宫就是典型的一种

//递归转非递归，就用栈模拟实现递归调用过程
//到底要不要用栈其实有两方面因素，有些情况下是不需要用到栈的，
//例如说求和公式就不需要用栈，单单使用循环就可以了。
//1、非递归用到栈和循环，只不过栈不是必须的
//2、单用循环解决不了的，就要把栈引进来
void PreOrderLoop(BTreeNode *root)
{
  SeqStack stack;
  SeqStackInit(&stack);

  BTreeNode *cur, *top;
  cur = root;
  while(NULL != cur || !SeqStackEmpty(&stack))
  {
    while(NULL != cur)
    {
      printf("%d ", cur->data);
      SeqStackPush(&stack, cur);//压栈压的是结点的地址，不是结点的data
      cur = cur->left;
    }
    top = (BTreeNode*)SeqStackTop(&stack);
    SeqStackPop(&stack);

    cur = top->right;
  }
  printf("\n");

  //销毁栈，程序走到这的话，stack已经为空，所以无需销毁
}

void InOrderLoop(BTreeNode *root)
{
  SeqStack stack;
  SeqStackInit(&stack);

  BTreeNode *cur, *top;
  cur = root;
  while(NULL != cur || !SeqStackEmpty(&stack))
  {
    while(NULL != cur)
    {
      SeqStackPush(&stack, cur);
      cur = cur->left;
    }
    top = (BTreeNode*)SeqStackTop(&stack);
    SeqStackPop(&stack);
    printf("%d ", top->data);

    cur = top->right;
  }
  printf("\n");

  //依旧不需要销毁栈
}

void PostOrderLoop(BTreeNode *root)
{
  SeqStack stack;
  SeqStackInit(&stack);

  BTreeNode *cur, *top;
  BTreeNode *last = NULL;//last是上一个被遍历的结点
  cur = root;
  while(NULL != cur || !SeqStackEmpty(&stack))
  {
    while(NULL != cur)
    {
      SeqStackPush(&stack, cur);
      cur = cur->left;
    }
    top = (BTreeNode*)SeqStackTop(&stack);
    if(NULL == top->right || top->right == last)
    {
      //如果右子树已经被遍历过了
      SeqStackPop(&stack);
      printf("%d ", top->data);
      last = top;//记录结点
    }
    else
    {
      //如果右子树没有被遍历过
      cur = top->right;
    }
  }
  printf("\n");

  //依旧不需要销毁栈
}
//后序遍历的作用
//1、从根结点到当前结点的路径保存在栈里
//2、求高度，就是求最长路径
//3、求镜像

//求二叉树的镜像，就是一y轴进行翻转
//空树的镜像还是空树
//一个结点的树也不动
void Mirror1(BTreeNode *root)
{
  //递归后序
  if(NULL == root)
  {
    return;
  }

  Mirror1(root->left);
  Mirror1(root->right);

  BTreeNode *t;//交换左右子树的中间变量
  t = root->left;
  root->left = root->right;
  root->right = t;
}
void Mirror2(BTreeNode *root)
{
  SeqStack stack;
  SeqStackInit(&stack);

  BTreeNode *cur, *top;
  BTreeNode *last = NULL;//last是上一个被遍历的结点
  cur = root;
  while(NULL != cur || !SeqStackEmpty(&stack))
  {
    while(NULL != cur)
    {
      SeqStackPush(&stack, cur);
      cur = cur->left;
    }
    top = (BTreeNode*)SeqStackTop(&stack);
    if(NULL == top->right || top->right == last)
    {
      //如果右子树已经被遍历过了
      SeqStackPop(&stack);

      BTreeNode *t = top->left;
      top->left = top->right;
      top->right = t;
      last = top;//记录结点
    }
    else
    {
      //如果右子树没有被遍历过
      cur = top->right;
    }
  }

  //依旧不需要销毁栈
}
