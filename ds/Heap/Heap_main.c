#include"Heap.h"

int main()
{
  int array[] = {53, 17, 78, 9, 45, 65, 87, 23, 31};
  int size = sizeof(array)/sizeof(array[0]);
  Heap heap;
  HeapInit(&heap, array, size);
  HeapMake(&heap);
  HeapPrint(&heap);
  //printf("%d\n", HeapTop(&heap));

  HeapPop(&heap);
  //HeapPrint(&heap);
  //printf("%d\n", HeapTop(&heap));

  HeapPush(&heap, 32);
  //HeapPrint(&heap);

  HeapSort(array, size);
  int i = 0;
  for(; i < size; ++i)
  {
    printf("%d ", array[i]);
  }
  printf("\n");

  int *p = TopK(array, size, 3); 
  for(i = 0; i < 3; ++i)
  {
    printf("%d ", p[i]);
  }
  printf("\n");
  return 0;
}
