//通过信号量实现生产者与消费者模型
#include<vector>
#include<pthread.h>
#include<pthread.h>
#include<semaphore.h>
#include<iostream>
#include<unistd.h>

#define MAX_QUEUE 10

class RingQueue
{
  private:
    std::vector<int> _queue;
    int _capacity;
    int _read;
    int _write;
    sem_t _lock;//用信号量实现互斥
    sem_t _idle_space;//用信号量实现同步，也就是用环形队列的空闲空间，生产者入队数据之前，初始化成capacity。
    sem_t _data_space;//用于判断是否队列有数据，消费者获取数据之前判断有数据的空间有多少，判断能否获得数据，初始化为0。
  public:
    RingQueue(int maxque = MAX_QUEUE)
      : _queue(maxque)
      , _capacity(maxque)
      , _read(0)
      , _write(0)
    {
      sem_init(&_lock, 0, 1);
      sem_init(&_idle_space, 0, _capacity);
      sem_init(&_data_space, 0, 0);
    }
    ~RingQueue()
    {
      sem_destroy(&_lock);
      sem_destroy(&_idle_space);
      sem_destroy(&_data_space);
    }
    void QueuePush(int data)
    {
      sem_wait(&_idle_space);
      sem_wait(&_lock);
      _queue[_write] = data;
      _write = (_write+1) % _capacity;
      //是否需要判断_write==_read？因为写满了，就不能再往里面写了
      //是不需要的，因为_idle_space就判断了当前还有没有能使用的空闲空间
      sem_post(&_lock);
      sem_post(&_data_space);//数据空间计数+1，唤醒消费者取数据
      //如果有多个生产者和多个消费者的话，就会存在一个问题，
      //_queue.push_back(data);和_write = (_write+1) % _capacity;并非原子操作，
      //单拿生产者来说，有可能多个生产者往同一结点中写入数据，该结点就出问题了
      //所以需要进行加锁，
      //sem_wait(&_lock);
      //而且要在sem_wait(&_idle_space);之后，
      //因为一旦你先对_lock进行加锁，万一此时又没有空闲空间，那么程序就会卡死在这
    }
    void QueuePop(int &data)
    {
      sem_wait(&_data_space);
      sem_wait(&_lock);
      data = _queue[_read];
      _read = (_read+1) % _capacity;
      sem_post(&_lock);
      sem_post(&_idle_space);//空闲空间计数+1，唤醒生产者产生数据
    }
};

void *thr_consumer(void *arg)
{
  RingQueue *q = (RingQueue*)arg;
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
  RingQueue *q = (RingQueue*)arg;
  int i = 0;
  while(1)
  {
    q->QueuePush(i);
    i++;
    //usleep(1);
    std::cout << "producer: " << "pthread_self()" << "put data: " << i << "\n";
  }
  return NULL;
}

int main()
{
  pthread_t consumer_tid[4], producer_tid[4];

  RingQueue q;

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
  return 0;
}
