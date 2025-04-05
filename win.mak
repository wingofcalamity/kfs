CC = cl
CCFLAGS = -Wall -std=c89

all: main.x

main.x: main.c kfs2.c
	$(CC) -o main.x $(CCFLAGS) main.c kfs2.c

.PHONY: clean test

clean:
	del /q *.x *.o *.txt *.kfs *.obj

	