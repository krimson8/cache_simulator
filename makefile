CC = gcc
CFLAGS = -o
EXEC = simulator
SOURCE = simulator.c

.PHONY: all simulator clean

all: 
	$(CC) $(CFLAGS) $(EXEC) $(SOURCE)
clean:
	rm $(EXEC)
test1: all
	./$(EXEC) 2048 64 64 l < trace/429.mcf-184B.trace.txt/429.mcf-184B.trace.txt
test2: all
	./$(EXEC) 2048 64 64 l < try.txt
test3: all
	./$(EXEC) 2048 64 64 r < trace/429.mcf-184B.trace.txt/429.mcf-184B.trace.txt
gen:
	$(CC) $(CFLAGS) gen trace_gen.c
	./gen > try.txt
	rm gen
