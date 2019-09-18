.PHONY:all
all:exec exec_test

exec:exec.c
	gcc $^ -o $@

exec_test:exec_test.c
	gcc $^ -o $@

.PHONY:clean
clean:
	rm exec exec_test
