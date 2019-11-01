#CMPUT 379 Assignment 2
## Introduction
In this assignment, we implement a simplified version of MapReduce using local threads and our own threadpool.
To run:
```
make wc
./wordcount sample1.txt sample2.txt sample3.txt sample4.txt sample5.txt sample6.txt sample7.txt sample8.txt sample9.txt sample10.txt sample11.txt sample12.txt sample13.txt sample14.txt sample15.txt sample16.txt sample17.txt sample18.txt sample19.txt sample20.txt
```
The above test program will show the word counts of each word in these 20 text files.
Other Map and Reduce functions can be defined in the distwc.c file.

## Implementation
### Intermediate Key/Value Pairs
In our mapreduce program, we require an intermediate data structure to store partitions of data. For every key
value, our hash function will generate an appropriate partition number. Each partition will be handled by a reducer 
thread, meaning we will have the same number of partitions as the number of reducer threads. The intermediate data
structure used by this program is defined in the `Partitions_t` object. The core of object is a vector 
of maps where each map is a partition. Each map is simply a key value pair where the key is a string and the value is 
another vector of strings. This is done since a key can have multiple values. This `Partitions_t` object also has a 
vector of mutexes. This allows for multiple threads to access the `Partitions_t` object at once, so long as they are 
accessing different partitions. This object also has a constructor to allow for easy allocation and initialization.
### MR_Emit
The `MR_Emit` function receives a key-value pair, determines the correct partition to place the key-value pair, and writes
it to the partition. `MR_Emit` first calls `MR_Partition` to get the partition. To analyze the runtime of this function,
we need to look at inserting a new key value pair into the partition object. According to 
[this source](https://en.cppreference.com/w/cpp/container/vector), random access of an element is constant time. Therefore,
we can grab a partition's map in constant time. However, when we get the partition's map and we need to insert a new key, 
insertion operations have [logarithmic complexity](https://en.cppreference.com/w/cpp/container/map). Pushing 
an item to the back of a vector is constant. Therefore, `MR_Emit` is `O( log(n) )`.

### MR_GetNext
`MR_GetNext` also accesses the `Partitions_t` object. Luckily, `MR_GetNext` only pops the last value off of the vector.
Since accessing the partition, the partition map, and the value from the map are all done in constant time, the time 
complexity of this is `O(1)`. 

### Threadpool
The synchronization primitives used in the threadpool are a mutex and a conditional. These two are used to control 
access to the task queue and notify threads when a threadpool is ready for access. We also have a boolean variable that 
is used to show that a the job queue has been completed and a vector of `pid_t`. The `Thread_run` function is assigned 
to every thread that is created. `Thread_run` will continuously call `Threadpool_get_work` and grab a task to execute.
When grabbing a task, the `taskLock` mutex is locked, meaning the thread will be locked until the task is acquired. 
While the task queue is empty and the tasks aren't all completed, the thread will wait for a conditional signal notifying
it that a task has been added or that the job queue is done. Then, it will move on to grab a task and return it or 
gracefully exit if there are no more tasks and the queue is done. When adding a task the threadpool using 
`Threadpool_add_work`, we lock the mutex before pushing to the task queue. After adding to the task queue, we send a 
conditional signal notifying the waiting threads that a task has been added and we unlock the mutex to allow other 
threads to access them. 

