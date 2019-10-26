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

void athread (void *arg) {
    int i; pid_t pid; pthread_t tid;
    pid = getpid();
    tid = pthread_self(); // obtain the handle (ID) of the calling thread
    printf("Process ID: %d, thread ID: %u \n", (unsigned int) pid,
           (unsigned int) tid, (char *) arg);
}

void *Thread_run(void *args){
    ThreadPool_t *tp = (ThreadPool_t *) args;
    ThreadPool_get_work(tp);

}

ThreadPool_t *ThreadPool_create(int num){
    ThreadPool_t *threadPool = new ThreadPool_t();
    sem_init(&threadPool->semaphore, 0, 1);
    char *arg = "hello";
    thread_func_t func(athread);
    ThreadPool_add_work(threadPool, athread, arg);
    for(int i = 0; i < num; i++){
        pthread_t tid;
        pthread_create(&tid, NULL, Thread_run, threadPool);
        threadPool->tid.push_back(tid);
    }
    return threadPool;

}
bool ThreadPool_add_work(ThreadPool_t *tp, thread_func_t func, void *arg){
    ThreadPool_work_t *task = new ThreadPool_work_t();
    task->func = func;
    task->arg = arg;
    sem_wait(&(tp->semaphore));
    tp->workQueue.tasks.push_back(task);
    sem_post(&(tp->semaphore));
}

void *ThreadPool_get_work(ThreadPool_t *tp){
    sem_wait(&(tp->semaphore));
    ThreadPool_work_t *task;
    bool run_task = false;
    if (tp->workQueue.tasks.size() > 0) {
        task = tp->workQueue.tasks.back();
        tp->workQueue.tasks.pop_back();
        run_task = true;
    }
    sem_post(&(tp->semaphore));
    if(run_task){
        task->func(task->arg);
    }

}

int main(){
    ThreadPool_t *tp = ThreadPool_create(3);
    for(auto& id: tp->tid){
        void *returnVal;
        pthread_join(id, &returnVal);
    }

}