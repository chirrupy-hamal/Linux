#include"LinkQueue.h"

int main()
{
  Queue queue; 
  
  QueueInit(&queue);
  printf("队列是否为空：%d\n", QueueEmpty(&queue));
  QueueSize(&queue);
  QueuePrint(&queue);

  QueuePush(&queue, 1);
  QueuePush(&queue, 2);
  QueuePush(&queue, 3);
  QueuePrint(&queue);  
  int data = QueueFront(&queue);
  printf("队首数据是：%d\n", data);

  QueuePop(&queue);
  QueuePrint(&queue);
  data = QueueFront(&queue);
  printf("队首数据是：%d\n", data);
  
  QueueDestroy(&queue);
  printf("队列是否为空：%d\n", QueueEmpty(&queue));
  QueueSize(&queue);
  QueuePrint(&queue);

  return 0;
}
