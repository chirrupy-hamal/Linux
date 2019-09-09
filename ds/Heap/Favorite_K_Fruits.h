#pragma once 
#include<assert.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

/* 
static char *fruit[] = {"apple", "pear", "banana", "orange"};
int size = sizeof(fruit)/sizeof(cfruit[0]);

void Generate()
{
  int i = 0;
  srand(20180831);
  for(; i < size; ++i)
  {
  printf(" \"%s\",\n", fruits[rand()%size]);
  }
}
 */
//上述就是一个生成favorite_fruits的

static char *favorite_fruits[] = 
{
  "pear",
  "pear",
  "apple",
  "banana",
  "apple",
  "apple",
  "pear",
  "banana",
  "apple",
  "banana",
  "pear",
  "orange",
  "banana",
  "apple",
  "apple",
  "orange",
  "apple",
  "orange",
  "orange",
  "banana",
  "pear",
  "banana",
  "pear",
  "pear",
  "apple",
  "apple",
  "orange",
  "orange",
  "pear",
  "banana",
  "apple",
  "orange",
  "banana",
  "banana",
  "orange",
  "pear",
  "orange",
  "pear",
  "pear",
  "apple",
  "apple",
  "orange",
  "apple",
  "banana",
  "orange",
  "pear",
  "banana",
  "apple",
  "apple",
  "apple",
  "banana",
  "banana",
  "pear",
  "pear",
  "apple",
  "pear",
  "banana",
  "apple"
};

int favorite_size = sizeof(favorite_fruits)/sizeof(favorite_fruits[0]);

typedef struct ResultItem
{
  char fruit[20];//key
  int count;//value
} ResultItem;

typedef struct Node
{
  ResultItem data;
  struct Node *left;
  struct Node *right;
} Node;

//插入
void Insert(Node **root, char *fruit)
{
  assert(root);

  Node *node = (Node*)malloc(sizeof(Node));
  strcpy(node->data.fruit, fruit);
  node->data.count = 1;
  node->left = node->right = NULL;

  //root不能为空
  if(*root == NULL)
  {
    *root = node;
    return;
  }

  int r = strcmp((*root)->data.fruit, fruit);
  assert(0 != r);//保证不会插入fruit重复的值
  if(r < 0)
  {
    Insert(&((*root)->left), fruit);
  }
  else
  {
    Insert(&((*root)->right), fruit);
  }
}

//查找
ResultItem *Find(Node *root, char *fruit)
{
  if(NULL == root)
  {
    return NULL;
  }

  int r = strcmp(root->data.fruit, fruit);
  if(0 == r)
  {
    return &(root->data);//注意返回的是&data，而不是二叉树中的结点
  }
  if(r < 0)
  {
    return Find(root->left, fruit);//一定要和插入相匹配
  }
  else 
  {
    return Find(root->right, fruit);
  }
}

void PreOrder(ResultItem *result, const Node *root)
{
  //result是输出型参数
  if(NULL == root)
  {
    return;
  }

  int i;
  //此时我是知道水果种类只有4种的
  switch(root->data.fruit[0])//而且首字母还都不一样
  {
    case 'a':
      i = 0;
      break;
    case 'b':
      i = 1;
      break;
    case 'o':
      i = 2;
      break;
    case 'p':
      i = 3;
      break;
  }
  result[i] = root->data;
  
  PreOrder(result, root->left);
  PreOrder(result, root->right);
}

ResultItem *StaticFruitsCount(char *fruits[], int size, int *pResultSize)
{
  //pResultSize是输出型参数，告诉调用者一共有几种水果
  Node *root = NULL;//用于建树的根结点

  //假设有个集合Set
  //1、顺序表
  //2、二叉搜索树
  //3、哈希表
  //面试的时候用哈希表来解决，因为二叉搜索树的话，背后有可能不平衡，
  //这样的话，效率就不行了，那还不如直接用数组，也就是顺序表。
  
  //面试的时候第一反应看一下用顺序表能不能解决，因为代码好写，
  //如果时间复杂度也没问题的话，就用顺序表，否则就用哈希表。

  //为了能够练习一下二叉搜索树，就用二叉搜索树来构建Set
  int i = 0;
  for(; i < size; ++i)
  {
    char *fruit = fruits[i];
    //先看看二叉搜索树中有没有这种水果
    ResultItem *item = Find(root, fruit);
    if(NULL != item)
    {
      //该搜索二叉树中已经有了这种水果
      ++item->count;//次数+1
    }
    else 
    {
      //该搜索二叉树中没有这种水果，
      //那就把这种信出现的水果插入到二叉搜索树中，
      //并将count置1。
      Insert(&root, fruit);//置1操作在Insert函数内部已经实现
    }
  }

  //用前序遍历的方式把二叉搜索树中的data取出来
  ResultItem *result = (ResultItem*)malloc(4 * sizeof(ResultItem));
  //这个4是原本就知道水果的种类只有4种，所以也就无需计算了
  PreOrder(result, root);

  *pResultSize = 4;//把4返回给调用者，
  return result;//这个返回值是接下来建小堆需要的，而且result里面的数据存放顺序已经在PreOrder函数中规定好了
}

//建小堆
void AdjustDown(ResultItem array[], int size, int root)
{
  //在原数组的基础上进行建小堆
  int parent = root;
  while(1)
  {
    int left = 2*parent + 1;
    if(left >= size)
    {
      //下标越界，没有左孩子，也就没有右孩子
      return;
    }
    //一定有左孩子
    int maxChild = left;
    if(2*parent + 2 < size && array[2*parent+2].count > array[left].count)
    {
      //右孩子也有，而且右孩子还比左孩子大
      maxChild = 2*parent + 2;
    }
    if(array[parent].count <= array[maxChild].count)
    {
      return;
    }
    //换
    ResultItem t = array[parent];
    array[parent] = array[maxChild];
    array[maxChild] = t;
    parent = maxChild;
  }
}

ResultItem *TopK(ResultItem *result, int size, int k)
{
  assert(k <= size);
  
  ResultItem *heap = (ResultItem*)malloc(sizeof(ResultItem) * k);
  assert(heap);
  int i = 0;
  for(; i < k; ++i)
  {
    heap[i] = result[i];//k别超size，把数据进行搬移
  }

  //建小堆
  int j = 0;
  for(j = (k-2)/2; j >= 0; --j)
  {
    AdjustDown(heap, k, j);
  }

  //接下来开始topk
  int m = 0;
  for(m = k; m < size; ++m)
  {
    if(result[m].count > heap[0].count)
    {
      //这是可以等于的，因为可能会出现班级里喜欢两种不同水果的人一样多，
      //所以外部判断要有=，
      //而且在内部中再加一层判断
      //if(result[m].count == heap[0].count && result[m].fruit[0] < heap[0].fruit[0])//按首字母
      //{heap[0] = result[m];AdjustDown(heap, k, 0);}
      heap[0] = result[m];
      AdjustDown(heap, k, 0);
    }
  }
  return heap;
}

void FindTopK()
{
  int result_size;//统计水果的种类个数
  ResultItem *result = StaticFruitsCount(favorite_fruits, favorite_size, &result_size);
  
  ResultItem *topk = TopK(result, result_size, 2);

  int i = 0;
  for(; i < 2; ++i)
  {
    printf("%s %d\n", topk[i].fruit, topk[i]. count);
  }
}
