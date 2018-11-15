.PHONY: simulator clean

simulator: 
	gcc simulator.c -o simulator
clean:
	rm simulator
test1:
	./simulator 2048 64 64 1 < trace/429.mcf-184B.trace.txt/429.mcf-184B.trace.txt
test2:
	./P74XXXXXX -input trace/trace2.txt -output out2
test3:
	./P74XXXXXX -input trace/trace3.txt -output out3
