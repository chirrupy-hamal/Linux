#include<iostream>
#include<queue>
#include<pthread.h>
#include<stdlib.h>//rand()和srand()
#include<unistd.h>
#include<stdio.h>

typedef void (*handler_t)(int data);
//任务类
class Task
{
  private:
    int _data;
    handler_t _handler;
  public:
    void SetTask(int data, handler_t handler)
    {
      _data = data;
      _handler = handler;
    }
    void Run()
    {
      _handler(_data);
    }
};

#define THR_MAX 5

class ThreadPool
{
  private:
    int _thr_max;//线程最大数量
    std::queue<Task> _task_queue;//线程安全队列
    pthread_mutex_t _mutex;
    pthread_cond_t _pro_cond;
    pthread_cond_t _con_cond;
    bool _quit_flag;//标志，线程池里面的所有线程现在到底是否要退出
    int _cur_thr;//线程池中当前还有多少线程
  public:
    ThreadPool(int max = THR_MAX)
      : _thr_max(max)
      , _quit_flag(false)
      , _cur_thr(max)
  {
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_con_cond, NULL);
    pthread_cond_init(&_pro_cond, NULL);
  }
    ~ThreadPool()
    {
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_con_cond);
      pthread_cond_destroy(&_pro_cond);
    }
    bool PoolInit()//线程的创建
    {
      int ret;
      pthread_t tid;
      for(int i = 0; i < _thr_max; ++i)
      {
        ret = pthread_create(&tid, NULL, thr_start, (void*)this);
        if(0 != ret)
        {
          std::cerr << "thread create error\n";
          return false;
        }
        pthread_detach(tid);//线程退出后，资源自动释放
      }
      return true;
    }
    void TaskPush(Task &t)//任务入队操作
    {
      pthread_mutex_lock(&_mutex);
      _task_queue.push(t);
      pthread_cond_signal(&_con_cond);
      pthread_mutex_unlock(&_mutex);
    }
    void TaskPop(Task &t)//任务出队操作
    {
      //出队已经受保护了，不需要再加锁
      t = _task_queue.front();
      _task_queue.pop();
    }
    static void *thr_start(void *arg)//线程入口函数
    {
      ThreadPool *pool = (ThreadPool*)arg;
      while(1)//5个线程，10个任务，如果没有while的话，就只能处理5个任务
      {
        //pthread_mutex_lock(&pool->_mutex);
        pool->QueueBlock();
        //while(pool->_task_queue.empty())//
        while(pool->QueueIsEmpty())
        {
          //pthread_cond_wait(&(pool->_con_cond), &(pool->_mutex));//因为是消费者线程，所以等待在_con_cond上面
          pool->ConsumerWait();
        }
        Task t;
        pool->TaskPop(t);
        //pthread_mutex_unlock(&(pool->_mutex));
        pool->QueueUnBlock();
        t.Run();
      }
      return NULL;
    }
    void PoolQuit()
    { 
      if(false == _quit_flag)
      {
        _quit_flag = true;
      }
      while(_cur_thr > 0)
      {
        ConsumerWakeUp();//唤醒消费者线程
      }
    }
  public:
    void QueueBlock()
    {
      pthread_mutex_lock(&_mutex);
    }
    void QueueUnBlock()
    {
      pthread_mutex_unlock(&_mutex);
    }
    bool QueueIsEmpty()
    {
      return _task_queue.empty();
    }
    void ConsumerWait()
    {
      //线程陷入等待之前，先判断一下用户是否要销毁线程池
      if(true == _quit_flag)
      {
        _cur_thr--;
        pthread_mutex_unlock(&_mutex);
        pthread_exit(NULL);
      }
      pthread_cond_wait(&_con_cond, &_mutex);
    }
    void ConsumerWakeUp()
    {
      pthread_cond_signal(&_con_cond);
    }
};

void thr_handle(int data)
{
  srand(time(NULL));
  int sec = rand() % 5;
  printf("thr:%p--get data:%d--sleep:%dsec\n", pthread_self(), data, sec);
  sleep(sec);
}

int main()
{
  ThreadPool pool;
  pool.PoolInit();
  Task task;
  int i = 0;
  for(; i < 10; ++i)//添加10个任务
  {
    task.SetTask(i, thr_handle);
    pool.TaskPush(task);
  }
  pool.PoolQuit();
  return 0;
}
