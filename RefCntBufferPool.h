#ifndef REF_CNT_BUFFER_POOL_Hxx
#define REF_CNT_BUFFER_POOL_Hxx

#include "ThreadSafePool.h"
#include "RefCntBuffer.h"

class RefCntBufferPool : public ThreadSafePool<RefCntBuffer>
{
public:
    static boost::shared_ptr<RefCntBufferPool> createPool(int initialBufs)
    {
        boost::shared_ptr<RefCntBufferPool> ret(new RefCntBufferPool());
        ret->_sharedThis = ret;
        ret->addToPool(initialBufs);
        return ret;
    }
protected:
    RefCntBufferPool::RefCntBufferPool()
        : ThreadSafePool<RefCntBuffer>()
    {
    }

    boost::intrusive_ptr<RefCntBuffer> allocateBuffer(void) const
    {
        static int _bufId;
        boost::intrusive_ptr<RefCntBuffer> bfrPtr(new RefCntBuffer(_sharedThis));
        return bfrPtr;
    }
    boost::shared_ptr<RefCntBufferPool> _sharedThis;
};

#endif