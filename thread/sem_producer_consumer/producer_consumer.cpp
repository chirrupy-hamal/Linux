//这个demo实现生产者与消费者模型，这个模型的实现关键就在于线程安全的队列操作
#include<pthread.h>
#include<queue>
#include<iostream>

#define MAX_QUEUE 10

class BlockQueue
{
  private:
    std::queue<int> _queue;//内部使用链表实现的，没有上限，这样容易出问题，
                         //一瞬间大量数据过来，导致资源耗尽，程序崩溃。
    size_t _capacity;//限制队列中结点最大数量
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_producer;
    pthread_cond_t _cond_consumer;
  public:
    BlockQueue(int capacity = MAX_QUEUE)
      : _capacity(capacity)
    {
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_cond_producer, NULL);
      pthread_cond_init(&_cond_consumer, NULL);
    };
    ~BlockQueue()
    {
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_cond_producer);
      pthread_cond_destroy(&_cond_consumer);
    }
    void QueuePush(int data)//生产者
    {
      pthread_mutex_lock(&_mutex);
      while(_queue.size() == _capacity)//记住用while不是if
      {
        pthread_cond_wait(&_cond_producer, &_mutex);
      }
      _queue.push(data);
      pthread_cond_signal(&_cond_consumer);
      pthread_mutex_unlock(&_mutex);
    }
    void QueuePop(int &data)//消费者
    {
      pthread_mutex_lock(&_mutex);
      while(_queue.empty())
      {
        pthread_cond_wait(&_cond_consumer, &_mutex);
      }
      data = _queue.front();
      _queue.pop();
      pthread_cond_signal(&_cond_producer);
      pthread_mutex_unlock(&_mutex);
    }
};

void *thr_consumer(void *arg)
{
  BlockQueue *q = (BlockQueue*)arg;
  while(1)
  {
    int data;
    q->QueuePop(data);
    std::cout << "consumer: " << "pthread_self()" << "get data: " << data << "\n";
  }
  return NULL;
}

void *thr_producer(void *arg)
{
  BlockQueue *q = (BlockQueue*)arg;
  int i = 0;
  while(1)
  {
    q->QueuePush(i);
    i++;
    std::cout << "producer: " << "pthread_self()" << "put data: " << i << "\n";
  }
  return NULL;
}

int main()
{
  pthread_t consumer_tid[4], producer_tid[4];
 
  BlockQueue q;//不用全局变量了，用传参形式
  
  int ret;
  int i = 0;
  for(; i < 4;  ++i)
  {
    ret = pthread_create(&consumer_tid[i], NULL, thr_consumer, (void*)&q);
    if(0 != ret)
    {
      std::cout << "thread create error\n";
      return -1;
    }
  }

  for(i = 0 ; i < 4;  ++i)
  {
    ret = pthread_create(&producer_tid[i], NULL, thr_producer, (void*)&q);
    if(0 != ret)
    {
      std::cout << "thread create error\n";
      return -1;
    }
  }
  
  for(i = 0; i < 4;  ++i)
  {
    pthread_join(consumer_tid[i], NULL);
  }

  for(i = 0; i < 4;  ++i)
  {
    pthread_join(producer_tid[i], NULL);
  }

  return 0;//为了不让主线程return退出到这整个线程组退出，所以加上线程等待
}
