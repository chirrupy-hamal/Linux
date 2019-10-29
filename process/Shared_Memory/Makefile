.PHONY:all
all:write read

write:write.c
	gcc $^ -o $@

read:read.c
	gcc $^ -o $@

.PHONY:clean
clean:
	rm write read
