//
// Created by ken on 2019-10-12.
//
#include "threadpool.h"
#include <stdio.h>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>

using namespace std;


void *Thread_run(void *args){
    ThreadPool_t *tp = (ThreadPool_t *) args;
    while(true) {
        ThreadPool_work_t *task = ThreadPool_get_work(tp);
        task->func(task->arg);
    }
}

ThreadPool_t *ThreadPool_create(int num){
    ThreadPool_t *threadPool = new ThreadPool_t();
    threadPool->taskLock = PTHREAD_MUTEX_INITIALIZER;
    threadPool->taskCond = PTHREAD_COND_INITIALIZER;

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
    pthread_mutex_lock(&tp->taskLock);
    tp->workQueue.tasks.push(task);
    pthread_cond_signal(&tp->taskCond);
    pthread_mutex_unlock(&tp->taskLock);
    return true;
}

ThreadPool_work_t *ThreadPool_get_work(ThreadPool_t *tp){
    ThreadPool_work_t *task;

    pthread_mutex_lock(&tp->taskLock);
    while(!tp->finished && tp->workQueue.tasks.empty()){
        pthread_cond_wait(&tp->taskCond, &tp->taskLock);
    }
    if(tp->finished && tp->workQueue.tasks.empty()){
        pthread_mutex_unlock(&tp->taskLock);
        pthread_cond_signal(&tp->taskCond);
        pthread_exit(NULL);
    }
    task = tp->workQueue.tasks.front();
    tp->workQueue.tasks.pop();
    pthread_mutex_unlock(&tp->taskLock);
    return task;

}

void ThreadPool_destroy(ThreadPool_t *tp){
    pthread_mutex_lock(&tp->taskLock);
    tp->finished = true;
    pthread_cond_signal(&tp->taskCond);
    pthread_mutex_unlock(&tp->taskLock);

    for(auto& pid: tp->tid){
        if(pthread_join(pid, NULL) != 0){
            cout << "Could not join thread " << pid << endl;
        };
    }
    delete(tp);
}

