FLAGS = -std=c++11 -g -Wall -Werror -pthread
wc: mapreduce.o threadpool.o distwc.o
	g++ $(FLAGS) -o wordcount mapreduce.o threadpool.o distwc.o

threadpool: threadpool.o
	g++ $(FLAGS) -o threadpool threadpool.o

compile: distwc.o mapreduce.o threadpool.o

threadpool.o: threadpool.cc threadpool.h
	g++ $(FLAGS) -c threadpool.cc

mapreduce.o: mapreduce.cc mapreduce.h
	g++ $(FLAGS) -c mapreduce.cc

distwc.o: distwc.c
	g++ $(FLAGS) -c distwc.c

clean:
	rm mapreduce.o threadpool.o wordcount

compress:
	zip a2_tellambu.zip mapreduce.cc threadpool.cc distwc.cc mapreduce.h threadpool.h README.md Makefile

