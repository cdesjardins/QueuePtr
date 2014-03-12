#include <iostream>
#include "RefCntBufferPool.h"

int main()
{ 
	boost::shared_ptr<RefCntBufferPool> _pool(new RefCntBufferPool(5, 1024));
	for (int i = 0; i < 1000; i++)
	{
		boost::intrusive_ptr<RefCntBuffer> x;
		_pool->dequeue(x);
		x->_buffer = boost::asio::buffer(x->_buffer + 10, 100);
		boost::asio::buffer_copy(x->_buffer, boost::asio::buffer("hello"));
		x.reset();
	}
    std::cout << "done" << std::endl;
}
