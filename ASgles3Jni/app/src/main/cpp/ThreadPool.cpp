//
// Created by mac on 16/8/17.
//

#include "ThreadPool.h"
#include "gles3jni.h"
#include <iostream>

void CTask::SetData(void * data)
{
    m_ptrData = data;
}

vector<CTask*> CThreadPool::m_vecTaskList;         //任务列表
bool CThreadPool::shutdown = false;

pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

/**
 * 线程池管理类构造函数
 */
CThreadPool::CThreadPool(int threadNum)
{
    this->m_iThreadNum = threadNum;
    LOGI("I will create %d threads", threadNum );
//    cout << "I will create " << threadNum << " threads" << endl;
//    Create();
}

/**
 * 线程回调函数
 */
void* CThreadPool::ThreadFunc(void* threadData)
{
    pthread_t tid = pthread_self();
    while (1)
    {
        pthread_mutex_lock(&m_pthreadMutex);
        while (m_vecTaskList.size() == 0 && !shutdown)
        {
            pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
        }

        if (shutdown)
        {
            pthread_mutex_unlock(&m_pthreadMutex);
            printf("thread %lu will exit/n", pthread_self());
            pthread_exit(NULL);
        }

        printf("tid %lu run/n", tid);
        vector<CTask*>::iterator iter = m_vecTaskList.begin();

        /**
        * 取出一个任务并处理之
        */
        CTask* task = *iter;
        if (iter != m_vecTaskList.end())
        {
            task = *iter;
            m_vecTaskList.erase(iter);
        }

        pthread_mutex_unlock(&m_pthreadMutex);

        task->Run(); /** 执行任务 */
        printf("tid:%lu idle/n", tid);
    }
    return (void*)0;
}

/**
 * 往任务队列里边添加任务并发出线程同步信号
 */
int CThreadPool::AddTask(CTask *task)
{
    pthread_mutex_lock(&m_pthreadMutex);
    this->m_vecTaskList.push_back(task);
    pthread_mutex_unlock(&m_pthreadMutex);
    pthread_cond_signal(&m_pthreadCond);
    return 0;
}

/**
 * 创建线程
 */
int CThreadPool::Create()
{
    pthread_id = (pthread_t*)malloc(sizeof(pthread_t) * m_iThreadNum);
    for(int i = 0; i < m_iThreadNum; i++)
    {
        pthread_create(&pthread_id[i], NULL, ThreadFunc, NULL);
    }
    return 0;
}

/**
 * 停止所有线程
 */
int CThreadPool::StopAll()
{
    /** 避免重复调用 */
    if (shutdown)
    {
        return -1;
    }
    printf("Now I will end all threads!!/n");
    /** 唤醒所有等待线程，线程池要销毁了 */
    shutdown = true;
    pthread_cond_broadcast(&m_pthreadCond);

    /** 阻塞等待线程退出，否则就成僵尸了 */
    for (int i = 0; i < m_iThreadNum; i++)
    {
        pthread_join(pthread_id[i], NULL);
    }

    free(pthread_id);
    pthread_id = NULL;

    /** 销毁条件变量和互斥体 */
    pthread_mutex_destroy(&m_pthreadMutex);
    pthread_cond_destroy(&m_pthreadCond);

    return 0;
}

/**
 * 获取当前队列中任务数
 */
int CThreadPool::getTaskSize()
{
    return m_vecTaskList.size();
}

