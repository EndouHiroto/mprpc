#pragma once
#include <queue>
#include <mutex>    //pthread_mutex_t,保证线程安全
#include <thread>
#include <condition_variable>   //pthread_condition_t,做线程间通信

//异步写日志的日志队列
template<typename T>
class LockQueue
{
public:
    //多个worker线程都会写日志queue
    void Push(const T &data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);  //智能指针,出作用域自动解锁
        m_queue.push(data);
        m_condvariable.notify_one();
    }

    //一个线程读日志queue，写日志文件
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty())  //用while而不用if，防止线程的虚假唤醒。虚假唤醒是指线程在没有其他线程显式地对其进行唤醒操作的情况下被唤醒。
        {
            //日志队列为空，线程进入wait状态，并释放锁
            m_condvariable.wait(lock);
        }

        T data = m_queue.front();
        m_queue.pop();
        return data;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};
