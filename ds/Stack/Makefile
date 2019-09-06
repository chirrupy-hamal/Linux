.PHONY:all 
all:StackApplication StackApplication2

StackApplication:new_StackApplication.c
	gcc $^ -o $@ -g

StackApplication2:new_StackApplication2.c
	gcc $^ -o $@ -g

.PHONY:clean
clean:
	rm StackApplication StackApplication2
