.PHONY:all
all:producer_consumer threadpool 

producer_consumer:producer_consumer.cpp
	g++ $^ -o $@ -l pthread

threadpool:threadpool.cpp
	 g++ $^ -o $@ -l pthread -g
#链接库：-L+库路径 -l+库名
#因为lib64本身就是一个默认的库的搜索路径，可以去掉-L+库路径
#对于线程库的使用，l也可以去掉，直接-pthread

.PHONY:clean
clean:
	rm threadpool producer_consumer
