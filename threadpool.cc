//
// Created by ken on 2019-10-12.
//
#include "threadpool.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
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
    sem_init(&threadPool->writeLock, 0, 1);
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
    sem_wait(&(tp->writeLock));
    tp->workQueue.tasks.push(task);
    sem_post(&(tp->writeCond));
    sem_post(&(tp->writeLock));
}

ThreadPool_work_t *ThreadPool_get_work(ThreadPool_t *tp){
    sem_wait(&(tp->readLock));
    ThreadPool_work_t *task;
    sem_wait(&(tp->writeCond));
    task = tp->workQueue.tasks.front();
    tp->workQueue.tasks.pop();
    sem_post(&(tp->readLock));
    return task;

}

void ThreadPool_destroy(ThreadPool_t *tp){
    for(auto& id: tp->tid){
        pthread_cancel(id);
    }
    delete tp;
}
/*
int main(){

    ThreadPool_t *tp = ThreadPool_create(3);
    for(int i = 0; i < 2000; i++){
        int num = i;
        ThreadPool_add_work(tp, athread, (void *) num);
        usleep(rand()%10000);
    }
    sleep(2);
    ThreadPool_destroy(tp);
}
*/
