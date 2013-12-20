#ifndef THREADSAFEPOOL_H
#define THREADSAFEPOOL_H

#include "ThreadSafeQueue.h"
#include <boost/smart_ptr/intrusive_ptr.hpp>

template <class T> class ThreadSafePool : public ThreadSafeQueue<boost::intrusive_ptr<T> >
{
public:
    virtual bool dequeue(boost::intrusive_ptr<T> &data)
    {
        bool ret = ThreadSafeQueue<boost::intrusive_ptr<T> >::dequeue(data);
        return ret;
    }

protected:
    ThreadSafePool()
    {
    }
    void addToPool(int num)
    {
        for (int i = 0; i < num; i++)
        {
            ThreadSafeQueue<boost::intrusive_ptr<T> >::enqueue(allocateBuffer());
        }
    }

    virtual boost::intrusive_ptr<T> allocateBuffer() const = 0;
};

#endif // THREADSAFEPOOL_H
