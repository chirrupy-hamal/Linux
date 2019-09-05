.PHONY:all

all:SeqList_main SeqListD_main

SeqList_main:SeqList_main.c SeqList.c
	gcc $^ -o $@

SeqListD_main:SeqListD_main.c
	gcc $^ -o $@

.PHONY:clean
clean:
	rm SeqList_main SeqListD_main
