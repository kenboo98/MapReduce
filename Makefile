mapreduce: mapreduce.o threadpool.o
	g++ -std=c++11 -Wall -o mapreduce mapreduce.o threadpool.o

threadpool: threadpool.o
	g++ -std=c++11 -Wall -pthread -o threadpool threadpool.o

compile: builtin.o threadpool.o

threadpool.o: threadpool.cc threadpool.h
	g++  -std=c++11 -Wall -g -pthread  -c threadpool.cc

mapreduce.o: mapreduce.cc
	g++  -std=c++11 -Wall -pthread -c mapreduce.cc

clean:
	rm mapreduce.o threadpool.o mapreduce threadpool

