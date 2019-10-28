FLAGS = -std=c++11 -g -Wall -Werror -pthread
mapreduce: mapreduce.o threadpool.o distwc.o
	g++ $(FLAGS) -o mapreduce mapreduce.o threadpool.o distwc.o

threadpool: threadpool.o
	g++ $(FLAGS) -o threadpool threadpool.o

compile: mapreduce.o threadpool.o

threadpool.o: threadpool.cc threadpool.h
	g++ $(FLAGS) -c threadpool.cc

mapreduce.o: mapreduce.cc mapreduce.h
	g++ $(FLAGS) -c mapreduce.cc

distwc.o: distwc.c
	g++ $(FLAGS) -c distwc.c

clean:
	rm mapreduce.o threadpool.o mapreduce threadpool

