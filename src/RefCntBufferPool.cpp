/*
    QueuePtr - Self returning buffer pool
    Copyright (C) 2014  Chris Desjardins
    http://blog.chrisd.info cjd@chrisd.info

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "RefCntBufferPool.h"
#include "ThreadSafePool.h"

/*
** RefCntBufferPool_
**  This class contains the implementation details of the pool mechanism.
**  Which is basically a ThreadSafePool that deals with intrusive_ptrs to
**  buffer objects.
*/

class RefCntBufferPool_ : public ThreadSafePool<RefCntBuffer>
{
public:
    static std::shared_ptr<RefCntBufferPool_> createPool(const int initialBufs, const int bufferSize)
    {
        std::shared_ptr<RefCntBufferPool_> ret(new RefCntBufferPool_());
        ret->_sharedThis = ret;
        ret->addToPool(initialBufs, bufferSize);
        return ret;
    }

    void destroyPool()
    {
        iterate(std::bind(&RefCntBufferPool_::freeBuffers, this, std::placeholders::_1));
        _sharedThis.reset();
    }

    ~RefCntBufferPool_()
    {
    }

    bool dequeue(boost::intrusive_ptr<RefCntBuffer>& data, const int msTimeout = 0)
    {
        bool ret = ThreadSafePool<RefCntBuffer>::dequeue(data, msTimeout);
        // Make sure the buffer is pointing to the start of the allocated block
        // and that the buffer is the correct size.
        if (data)
        {
            data->resetBackingData();
        }
        return ret;
    }

protected:

    RefCntBufferPool_()
        : ThreadSafePool<RefCntBuffer>()
    {
    }

    boost::intrusive_ptr<RefCntBuffer> allocateBuffer(const int bufferSize)
    {
        boost::intrusive_ptr<RefCntBuffer> bfrPtr(new RefCntBuffer(_sharedThis, bufferSize));
        return bfrPtr;
    }

    int freeBuffers(std::list<boost::intrusive_ptr<RefCntBuffer> >& pool)
    {
        int ret = pool.size();
        // Some buffers were not returned, could be a leak,
        // or could be that they arent fully processed, and
        // may get returned in time. If you care then you can
        // uncomment the assert and try to chase it...
        //assert(ret == _numBufs);

        std::list<boost::intrusive_ptr<RefCntBuffer> >::iterator it;
        for (it = pool.begin(); it != pool.end(); )
        {
            boost::intrusive_ptr<RefCntBuffer> b = *it;
            b->dead();
            it = pool.erase(it);
        }
        return -ret;
    }

    std::shared_ptr<RefCntBufferPool_> _sharedThis;
};

/*
** RefCntBufferPool
**  Implementation details of the RefCntBufferPool, this is just a pass through
**  to the RefCntBufferPool_ which does the real work. The only reason for this
**  pass through is to allow RAII style creation and deletion of a pool object.
**  When the RefCntBufferPool is deleted, then all of the buffers in the pool are
**  also deleted.
*/

RefCntBufferPool::RefCntBufferPool(const int initialBufs, const int bufferSize)
{
    _pool = RefCntBufferPool_::createPool(initialBufs, bufferSize);
}

RefCntBufferPool::~RefCntBufferPool()
{
    _pool->destroyPool();
    _pool.reset();
}

bool RefCntBufferPool::dequeue(boost::intrusive_ptr<RefCntBuffer>& data, const int msTimeout)
{
    data.reset();
    return _pool->dequeue(data, msTimeout);
}

void RefCntBufferPool::enqueue(const boost::intrusive_ptr<RefCntBuffer>& data)
{
    _pool->enqueue(data);
}

size_t RefCntBufferPool::size()
{
    return _pool->size();
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

