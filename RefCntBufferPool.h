#ifndef REF_CNT_BUFFER_POOL_Hxx
#define REF_CNT_BUFFER_POOL_Hxx

#include "ThreadSafePool.h"
#include "RefCntBuffer.h"
#include <boost/bind.hpp>

class RefCntBufferPool_;

class RefCntBufferPool
{
public:
    RefCntBufferPool(const int initialBufs, const int bufferSize);
    ~RefCntBufferPool();
    virtual bool dequeue(boost::intrusive_ptr<RefCntBuffer> &data);
    virtual void enqueue(const boost::intrusive_ptr<RefCntBuffer> &data);

protected:
    boost::shared_ptr<RefCntBufferPool_> _pool;
};

#endif
