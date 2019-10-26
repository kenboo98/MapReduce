//
// Created by ken on 2019-10-12.
//
#include "threadpool.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

void athread (void* arg) {
    int i; pid_t pid; pthread_t tid;
    pid = getpid();
    tid = pthread_self(); // obtain the handle (ID) of the calling thread
    printf("Process ID: %d, thread ID: %d, Value %d\n", (unsigned int) pid,
           (unsigned int) tid, (int *) arg);
}

void *Thread_run(void *args){
    ThreadPool_t *tp = (ThreadPool_t *) args;
    while(true) {
        ThreadPool_work_t *task = ThreadPool_get_work(tp);
        task->func(task->arg);
    }
}

ThreadPool_t *ThreadPool_create(int num){
    ThreadPool_t *threadPool = new ThreadPool_t();
    sem_init(&threadPool->readLock, 0, 1);
    sem_init(&threadPool->writeCond, 0, 0);
    thread_func_t func(athread);
    for(int i = 0; i < 2000; i++){
        int num = i;
        ThreadPool_add_work(threadPool, athread, (void *) num);
    }

    for(int i = 0; i < num; i++){
        pthread_t tid;
        pthread_create(&tid, NULL, Thread_run, threadPool);
        threadPool->tid.push_back(tid);
    }
    for(int i = 0; i < 2000; i++){
        int num = i;
        ThreadPool_add_work(threadPool, athread, (void *) num);
    }

    return threadPool;

}
bool ThreadPool_add_work(ThreadPool_t *tp, thread_func_t func, void *arg){
    ThreadPool_work_t *task = new ThreadPool_work_t();
    task->func = func;
    task->arg = arg;
    tp->workQueue.tasks.push_back(task);
    sem_post(&(tp->writeCond));
}

ThreadPool_work_t *ThreadPool_get_work(ThreadPool_t *tp){
    sem_wait(&(tp->readLock));
    ThreadPool_work_t *task;
    sem_wait(&(tp->writeCond));
    task = tp->workQueue.tasks.back();
    tp->workQueue.tasks.pop_back();
    sem_post(&(tp->readLock));
    return task;

}

int main(){
    ThreadPool_t *tp = ThreadPool_create(3);
    for(auto& id: tp->tid){
        void *returnVal;
        pthread_join(id, &returnVal);
    }

}