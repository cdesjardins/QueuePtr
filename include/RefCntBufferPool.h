#ifndef REF_CNT_BUFFER_POOL_Hxx
#define REF_CNT_BUFFER_POOL_Hxx

#include "RefCntBuffer.h"
#include <boost/smart_ptr/intrusive_ptr.hpp>

class RefCntBufferPool_;

class RefCntBufferPool
{
public:
    RefCntBufferPool(const int initialBufs, const int bufferSize);
    virtual ~RefCntBufferPool();
    virtual bool dequeue(boost::intrusive_ptr<RefCntBuffer> &data, const int msTimeout = 0);
    virtual void enqueue(const boost::intrusive_ptr<RefCntBuffer> &data);
    size_t size();
protected:
    boost::shared_ptr<RefCntBufferPool_> _pool;
};

#endif
