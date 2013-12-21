#include "RefCntBuffer.h"
#include "RefCntBufferPool.h"

/*
** RefCntBufferPool_
**  This class contains the implementation details of the pool mechanism.
**  Which is basically a ThreadSafePool that deals with intrusive_ptrs to
**  buffer objects.
*/

class RefCntBufferPool_ : public ThreadSafePool<RefCntBuffer>
{
public:
    static boost::shared_ptr<RefCntBufferPool_> createPool(const int initialBufs)
    {
        boost::shared_ptr<RefCntBufferPool_> ret(new RefCntBufferPool_());
        ret->_sharedThis = ret;
        ret->addToPool(initialBufs);
        return ret;
    }

    void destroyPool()
    {
        iterate(boost::bind(&RefCntBufferPool_::freeBuffers, this, _1));
        _sharedThis.reset();
    }

    ~RefCntBufferPool_()
    {
    }

    bool dequeue(boost::intrusive_ptr<RefCntBuffer> &data)
    {
        bool ret = ThreadSafePool<RefCntBuffer>::dequeue(data);
        // Make sure the buffer is pointing to the start of the allocated block
        // and that the buffer is the correct size.
        data->resetBackingData();
        return ret;
    }

protected:

    RefCntBufferPool_()
        : ThreadSafePool<RefCntBuffer>()
    {
    }

    boost::intrusive_ptr<RefCntBuffer> allocateBuffer()
    {
        static int _bufId;
        boost::intrusive_ptr<RefCntBuffer> bfrPtr(new RefCntBuffer(_sharedThis));
        return bfrPtr;
    }

    int freeBuffers(std::list<boost::intrusive_ptr<RefCntBuffer> > &pool)
    {
        int ret = pool.size();
        assert(ret == _numBufs);
        std::list<boost::intrusive_ptr<RefCntBuffer> >::iterator it;
        for (it = pool.begin(); it != pool.end();)
        {
            boost::intrusive_ptr<RefCntBuffer> b = *it;
            b->dead();
            it = pool.erase(it);
        }
        return -ret;
    }

    boost::shared_ptr<RefCntBufferPool_> _sharedThis;
};

/*
** RefCntBufferPool
**  Implementation details of the RefCntBufferPool, this is just a pass through
**  to the RefCntBufferPool_ which does the real work. The only reason for this
**  pass through is to allow RAII style creation and deletion of a pool object.
**  When the RefCntBufferPool is deleted, then all of the buffers in the pool are
**  also deleted.
*/

RefCntBufferPool::RefCntBufferPool(const int initialBufs)
{
    _pool = RefCntBufferPool_::createPool(initialBufs);
}

RefCntBufferPool::~RefCntBufferPool()
{
    _pool->destroyPool();
    _pool.reset();
}

bool RefCntBufferPool::dequeue(boost::intrusive_ptr<RefCntBuffer> &data)
{
    return _pool->dequeue(data);
}

void RefCntBufferPool::enqueue(const boost::intrusive_ptr<RefCntBuffer> &data)
{
    _pool->enqueue(data);
}


/*
** RefCntBuffer::finalRelease
**  Used to put unused buffers back on the pool if the pool is still "alive"
**  If the pool is "dead" then this also releases the memory for each buffer.
**  A "dead" pool is one that is no longer in use. 
**
**  This is stuck in this file because it needs to know the implementation 
**  of RefCntBufferPool_
*/
void RefCntBuffer::finalRelease(IntrusivePtrBase* s) const 
{
    if (_dead == true)
    {
        RefCntBuffer* p = dynamic_cast<RefCntBuffer*>(s);
        p->_pool.reset();
        delete p;
    }
    else
    {
        boost::intrusive_ptr<RefCntBuffer> p(dynamic_cast<RefCntBuffer*>(s));
        _pool->enqueue(p);
    }
}
