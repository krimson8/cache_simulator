.PHONY: simulator clean

simulator: 
	gcc simulator.c -o simulator
clean:
	rm simulator
test1:
	./simulator 2048 64 64 1 < trace/429.mcf-184B.trace.txt/429.mcf-184B.trace.txt
test2:
	./simulator 2048 64 64 1 < trace1.txt
test3:
	./P74XXXXXX -input trace/trace3.txt -output out3
