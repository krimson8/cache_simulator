simulator: 
	gcc simulator.c -o simulator
clean:
	rm simulator
test1:
	./P74XXXXXX -input trace/trace1.txt -output out1
test2:
	./P74XXXXXX -input trace/trace2.txt -output out2
test3:
	./P74XXXXXX -input trace/trace3.txt -output out3
