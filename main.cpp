#include <iostream>
#include "RefCntBufferPool.h"

int main()
{ 
    {
        boost::shared_ptr<RefCntBufferPool> _pool(new RefCntBufferPool(5));
        /*
        for (int i = 0; i < 1000; i++)
        {
            boost::intrusive_ptr<RefCntBuffer> x;
            _pool->dequeue(x);
            x.reset();
        }
        */
    }
    std::cout << "done" << std::endl;
}
