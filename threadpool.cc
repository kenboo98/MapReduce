//
// Created by ken on 2019-10-12.
//
#include "threadpool.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;
void *athread (void *arg) {
    int i; pid_t pid; pthread_t tid;
    pid = getpid();
    tid = pthread_self(); // obtain the handle (ID) of the calling thread
    printf("Process ID: %d, thread ID: %u\n", (unsigned int) pid,
           (unsigned int) tid);
}
ThreadPool_t *ThreadPool_create(int num){
    ThreadPool_t threadPool;
    for(int i = 0; i < num; i++){
        pthread_t tid;
        pthread_create(&tid, NULL, athread, NULL);
        threadPool.tid.push_back(tid);
    }
    for(auto& id: threadPool.tid){
        void *returnVal;
        pthread_join(id, &returnVal);
    }
    return &threadPool;
}
int main(){
    ThreadPool_create(5);
}