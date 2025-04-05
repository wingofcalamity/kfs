CC = gcc
CCFLAGS = -Wall -std=c89

all: main.x

main.x: main.c kfs2.c
	$(CC) -o main.x $(CCFLAGS) main.c kfs2.c

.PHONY: clean test

clean:
	rm -rf *.x *.o *.txt *.kfs

test: main.x
	rm -rf test.txt test.jpg test.kfs
	cp ./test/test.txt ./test.txt
	cp ./test/test.jpg ./test.jpg
	cp ./test/2.png ./2.png
	./main.x test.kfs -p test.txt test.jpg 2.png
	rm -rf test.txt test.jpg 2.png
	strace ./main.x test.kfs -u 2> strace.txt
	