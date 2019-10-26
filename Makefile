mapreduce: mapreduce.o threadpool.o distwc.o
	g++ -std=c++11 -Wall -pthread -o mapreduce mapreduce.o threadpool.o distwc.o

threadpool: threadpool.o
	g++ -std=c++11 -Wall -pthread -o threadpool threadpool.o

compile: mapreduce.o threadpool.o

threadpool.o: threadpool.cc threadpool.h
	g++  -std=c++11 -Wall -pthread  -c threadpool.cc

mapreduce.o: mapreduce.cc mapreduce.h
	g++  -std=c++11 -Wall -pthread -c mapreduce.cc

distwc.o: distwc.c
	g++  -std=c++11 -Wall -pthread -c distwc.c

clean:
	rm mapreduce.o threadpool.o mapreduce threadpool

