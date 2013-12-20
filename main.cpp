#include "RefCntBufferPool.h"

int main()
{ 
    boost::shared_ptr<RefCntBufferPool> _pool = RefCntBufferPool::createPool(100);
    boost::intrusive_ptr<RefCntBuffer> x;
    boost::intrusive_ptr<RefCntBuffer> y;
    _pool->dequeue(x);
    y = x;
    x.reset();
}