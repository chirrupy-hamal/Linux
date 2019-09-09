#pragma once 
#include<assert.h>
#include<malloc.h>

typedef struct Heap
{
  int array[100];
  int size;
} Heap;

void HeapInit(Heap *pH, int source[], int size)
{
  int i = 0;
  for(; i < size; ++i)
  {
    pH->array[i] = source[i];
  }
  pH->size = size;
}

void HeapAdjustDown(Heap *pH, int root)
{
  int left = 2*root + 1;
  if(left >= pH->size)
  {
    //越界，没有左孩子，也就没有右孩子
    return;
  }

  //走到这，就说明有左孩子
  int maxChild = left;
  
  if(2*root + 2 < pH->size && pH->array[2*root+2] > pH->array[left])
  {
    //右孩子也有，并且还比左孩子大
    maxChild = 2*root + 2;
  }

  if(pH->array[root] >= pH->array[maxChild])
  {
    //根节点比左右孩子中最大的还大
    return;
  }

  int t = pH->array[root];
  pH->array[root] = pH->array[maxChild];
  pH->array[maxChild] = t;

  //还没结束，
  //因为有可能你换完之后，maxChild位置的子树就不满足大堆了。
  HeapAdjustDown(pH, maxChild);
}

void HeapAdjustDown2(Heap *pH, int root)
{
  int parent = root;
  while(1)
  {
    int left = 2*parent + 1;
    if(left >= pH->size)
    {
      return;
    }
    
    int maxChild = left;
    
    if(2*parent + 2 < pH->size && pH->array[2*parent+2] > pH->array[left])
    {
      maxChild = 2*parent + 2;
    }
    
    if(pH->array[parent] >= pH->array[maxChild])
    {
      return;
    }

    int t = pH->array[parent];
    pH->array[parent] = pH->array[maxChild];
    pH->array[maxChild] = t;

    parent = maxChild;
  }
}

void HeapMake(Heap *pH)
{
  int i = (pH->size-2)/2;//叶子结点本身就是堆，
                         //所以从最后一个非叶子结点开始向下调整，所以说对数组来说，是从后往前开始向下调整进行建堆，
                         //最后一个非叶子结点在数组中的位置？最后一个叶子结点 在数组的位置是size-1，
                         //那该叶子结点的父结点在数组中的位置是((size-1)-1)/2。
  for(; i >= 0; --i)
  {
    HeapAdjustDown2(pH, i);
  }
}

void HeapPrint(Heap *pH)
{
  int i = 0;
  for(; i < pH->size; ++i)
  {
    printf("%d ", pH->array[i]);
  }
  printf("\n");
}

void HeapPop(Heap *pH)
{
  //堆的删除：删除时，每次删除堆顶元素
  //删除方法：
  //1、将堆中最后一个元素替代堆顶元素
  //2、将堆中元素个数减少一个，相当于将堆中最后一个元素删除
  //3、此时堆结构可能被破坏，再向下调整，使其满足堆的性质。
  pH->array[0] = pH->array[pH->size-1];
  --pH->size;
  HeapAdjustDown(pH, 0);
}

//查看堆顶元素
int HeapTop(const Heap *pH)
{
  return pH->array[0];
}

static void Swap(int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

void HeapAdjustUp(Heap *pH, int child)
{
  int parent;

  while(child > 0)
  {
    parent = (child-1)/2;
    if(pH->array[parent] >= pH->array[child])
    {
      return;
    }

    Swap(pH->array+parent, pH->array+child);
    child = parent;
  }
}

void HeapPush(Heap *pH, int data)
{
  assert(pH->size < 100);
  pH->array[pH->size++] = data;
  HeapAdjustUp(pH, pH->size-1);
}

void ArrayAdjustDown(int array[], int size, int root)
{
  int parent = root;
  while(1)
  {
    int left = 2*parent + 1;
    if(left >= size)
    {
      return;
    }

    int maxChild = left;

    if(2*parent + 2 < size && array[2*parent+2] > array[left])
    {
      maxChild = 2*parent + 2;
    }

    if(array[parent] >= array[maxChild])
    {
      return;
    }

    Swap(array+parent, array+maxChild);

    parent = maxChild;
  }
}  

//堆排序的时间复杂度是O(nlog2n)，
//这基本上已经是一个最快的时间复杂度了，排序是不可能再快了。

//堆排序分为两步：以此函数为例
//1、首先把序列变成一个大堆。建大(小)堆的时间复杂度是O(n)
//2、再不断交换堆顶的最大元素和最后一个元素，每次交换后再从根节点进行向下调整，
//这样的话，最大的元素就跑到数组最后面，这样最终的结果就是数组以一个升序的方式进行排列。
//步骤2的时间复杂度为O(nlog2n)。
//所以堆排的时间复杂度就是O(n) + O(nlog2n) = O(nlog2n)
void HeapSort(int array[], int size)
{
  //上一个函数的作用是在原数组上进行排序，不需要你再申请空间，否则时间复杂度就是O(n)
  int i = 0;
  //建大堆
  //那又为什么升序要建大堆？
  //因为建小堆的话，数组最左边的一定是最小元素，但是你要做的是升序，
  //所以你接下来不需要像建大堆那样换堆元素，但是你得重新建小堆，
  //如果是这样的话，原本步骤2的时间复杂度就会变成O(n*n)，这样的话，堆排序的时间复杂度就会是O(n*n)。
  
  //常见的时间复杂度按数量级递增排列有
  //O(1)、O(log2n)、O(n)、O(nlog2n)、O(n*n)、O(n*n*n)、……、指数O(2n)、O(n!)、O(nn)
  for(i = (size-2)/2; i >= 0; --i)
  {
    ArrayAdjustDown(array, size, i);
  }

  //开始排序
  int j = 0;
  for(; j < size; ++j)
  {
    int s = size-1-j;
    Swap(array, array+s);//堆顶元素和堆最后一个元素交换
    ArrayAdjustDown(array, size-1-j, 0);//从根继续建大堆，-1是为了不把数组最后元素算进堆中，
                                        //否则你刚刚换到数组最后的那个最大元素有可能被调整到数组的其他位置，
                                        //也就是(size-2)/2这个位置。
  }
}

//找最小的K个数据，需要建大堆

//和排序不太一样，排序是在原数组上进行调整，TopK最好不要在原数组上进行调整

//往往处理TopK的时候都是海量数据，
//内存中都放不下才用TopK，
//如果只是少量数据的话，排序就可以解决。
int *TopK(int array[], int size, int k)
{
  int *heapArray = (int*)malloc(k * sizeof(int));
  assert(heapArray);

  int i = 0;
  for(; i < k; ++i)
  {
    //先把数组array的前k个数据搬过去，
    //先建一个大堆出来。
    heapArray[i] = array[i];
  }

  int j;
  for(j = (k-2)/2; j >= 0; --j)
  {
    //建大堆
    ArrayAdjustDown(heapArray, k, j);
  }

  int m;
  for(m = k; m < size; ++m)
  {
    if(heapArray[0] <= array[m])
    {
      continue;
    }

    heapArray[0] = array[m];
    ArrayAdjustDown(heapArray, k, 0);
  }

  return heapArray;
}
