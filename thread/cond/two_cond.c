//这个demo实现条件变量的基本使用
//以自己吃面和厨师做面为例：想要吃面，前提得有面，没有面则等待。
//                          厨师做好面之后，唤醒吃面的人
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

int _have_noodle = 0;//默认没有面

pthread_mutex_t mutex;
pthread_cond_t cond_eat;
pthread_cond_t cond_cook;

void *eat_noodle(void *arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&mutex);
    while(0 == _have_noodle)
    {
      //没有面，就不能吃面
      //usleep(1000);//等的时间短还得重新等，等的长了还浪费，有一种更加高效的方式
      //当我不能做就休眠，条件满足了你再唤醒我，条件变量登场的时候
      //continue;//占用cpu资源太高，再此基础上加上休眠函数
      
      //没有面，就不能出面
      
      //刚进来的时候是没有面的，假设先运行的是吃面的，也就是吃面线程先拿到时间片，
      //加锁之后发现没有面，就陷入等待，但是在等待之前不解锁的话，厨师线程就会在加锁那卡住，
      //在等待之前要先解锁，也就是先pthread_mutex_unlock(&mutex);pause();pthread_mutex_lock();
      //但是pthread_mutex_unlock(&mutex);pause();二者之间并非原子操作，有可能在刚解锁，还没来得急进入休眠，此时
      //做面的线程刚好占用时间片，紧接着唤醒吃面的人，但是吃面的线程压根就没陷入休眠呢，人家本来就是r状态，此时
      //做面的线程唤醒后依旧是r状态，吃面的拿到时间片之后就陷入休眠了，做面的发现还有面，也进行等待，程序就卡死在这了。
      //pthread_mutex_unlock(&mutex);pause();这两个操作应该是原子操作
      pthread_cond_wait(&cond_eat, &mutex);//集合了3个操作，其中解锁和休眠是原子操作
    }
    //能走下来，表示_have_noodle == 1表示有面可以吃
    printf("eat noodle, delicious\n");
    _have_noodle = 0;
    pthread_cond_signal(&cond_cook);
    pthread_mutex_unlock(&mutex);//先解锁和先唤醒无所谓
  }
}

void *cook_noodle(void *arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&mutex);
    while(1 == _have_noodle)
    {
      //现在有面，没人吃，不能继续做了
      //usleep(1000);
      //continue;//占用cpu资源太高，加上休眠
      
      //现在有面，没人吃，不能继续做了
      pthread_cond_wait(&cond_cook, &mutex);
    }
    printf("cook noodle, come on\n");
    _have_noodle = 1;
    pthread_cond_signal(&cond_eat);//唤醒吃面的人
    pthread_mutex_unlock(&mutex);
  }
}

int main()
{
  pthread_t tid1, tid2;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_eat, NULL);
  pthread_cond_init(&cond_cook, NULL);
  //printf("%d\n", pthread_cond_signal(&cond_cook));打印0
  //sleep(10);
  int ret;
  for(int i = 0; i < 4; ++i)
  {
    ret  = pthread_create(&tid1, NULL, eat_noodle, NULL);
    if(0 != ret)
    {
      printf("pthread create error\n");
      return -1;
    }
  }
  //现在是4个厨师，4个顾客
  //假设顾客1、2、3、4时间片依次使用发现没有面，都陷入等待，
  //假设厨师1做面，唤醒等待，唤醒是至少唤醒一个，假设唤醒4个，哪个顾客时间片到了，哪个顾客吃面
  //假设顾客1拿到时间片，随后加锁，其他顾客不能加锁，所以其他顾客陷入互斥锁加锁的等待，故可以吃完之后，唤醒厨师、解锁
  //枷锁之后，问题来了，厨师1、顾客2、3、4都能抢，这时候假设顾客2抢到锁，接下来顾客2开始吃面，此时吃面逻辑就错误了。
  //但是程序是不会卡死的。知识发生吃/做面逻辑的错误。
  //因为顾客的代码实现是if语句，所以顾客2就可以继续执行，也就是吃面。
  //吃面逻辑可能会出错，反过来，做面逻辑也会出错。
  //解决此问题的方法应该把做面和吃面线程的if改成while
  
  //改成while后仍有可能陷入卡死
  //假设顾客1发现没有面，就陷入等待，又假设厨师1做面，但是其他厨师也可能没闲着，他们也想做面，但是他们的时间片过来后
  //发现有面，也陷入等待，这时候厨师1拿到时间片了，接下来该执行唤醒操作，可是条件变量上的等待队列既有顾客还有厨师，
  //但是完全有可能唤醒的不是顾客，而是厨师。这时候就会程序就会卡死，因为while。这属于角色唤醒错误导致的。
  //准确地说，应该是角色等待错误，厨师和顾客在同一队列上等待。
  //反过来，也可能会在顾客处卡死。
  //解决此问题的办法是使用两个条件变量。
  for(int i = 0; i < 4; ++i)
  {
    ret  = pthread_create(&tid2, NULL, cook_noodle, NULL);
    if(0 != ret)
    {
      printf("pthread create error\n");
      return -1;
    }
  }
  pthread_join(tid1, NULL);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond_eat);
  pthread_cond_destroy(&cond_cook);
  return 0;
}
