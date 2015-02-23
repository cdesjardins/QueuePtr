#include <iostream>
#include <thread>
#include "RefCntBufferPool.h"


void useBufs(std::shared_ptr<RefCntBufferPool> pool)
{
    int failCnt = 0;
    for (int i = 0; i < 1000; i++)
    {
        boost::intrusive_ptr<RefCntBuffer> x;
        pool->dequeue(x);
        pool->dequeue(x);
        if (x)
        {
            boost::intrusive_ptr<RefCntBuffer> y;
            y = x;
            y->_buffer = boost::asio::buffer(y->_buffer + 10, 100);
            boost::asio::buffer_copy(y->_buffer, boost::asio::buffer("hello"));
            x.reset();
        }
        else
        {
            failCnt++;
        }
    }
    std::cout << "done " << failCnt << std::endl;
}

int main()
{
    std::shared_ptr<RefCntBufferPool> pool(new RefCntBufferPool(700, 1024));
    std::thread t0[1024];
    for (size_t i = 0; i < (sizeof(t0) / sizeof(t0[0])); i++)
    {
        t0[i] = std::thread(std::bind(useBufs, pool));
    }
    for (size_t i = 0; i < (sizeof(t0) / sizeof(t0[0])); i++)
    {
        t0[i].join();
    }
}

