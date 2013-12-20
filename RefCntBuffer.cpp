#include "RefCntBuffer.h"
#include "RefCntBufferPool.h"

void RefCntBuffer::finalRelease(IntrusivePtrBase* s) const 
{
    boost::intrusive_ptr<RefCntBuffer> p(dynamic_cast<RefCntBuffer*>(s));
    _pool->enqueue(p);
}