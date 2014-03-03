#ifndef THREADSAFEPOOL_H
#define THREADSAFEPOOL_H

#include "ThreadSafeQueue.h"
#include <boost/smart_ptr/intrusive_ptr.hpp>

template <class T> class ThreadSafePool : public ThreadSafeQueue<boost::intrusive_ptr<T> >
{
public:
#if 0
    virtual bool dequeue(boost::intrusive_ptr<T> &data)
    {
        return ThreadSafeQueue<boost::intrusive_ptr<T> >::dequeue(data);
    }
#endif
protected:
    ThreadSafePool()
        : _numBufs(0)
    {
    }
    void addToPool(int num, const int bufferSize)
    {
        for (int i = 0; i < num; i++)
        {
            ThreadSafeQueue<boost::intrusive_ptr<T> >::enqueue(allocateBuffer(bufferSize));
        }
        _numBufs += num;
    }

    virtual boost::intrusive_ptr<T> allocateBuffer(const int bufferSize) = 0;
    int _numBufs;
};

#endif // THREADSAFEPOOL_H
