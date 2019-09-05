.PHONY:all 
all:StackApplication StackApplication2

StackApplication:StackApplication.c
	gcc $^ -o $@ -g

StackApplication2:StackApplication2.c
	gcc $^ -o $@ -g

.PHONY:clean
clean:
	rm StackApplication StackApplication2
