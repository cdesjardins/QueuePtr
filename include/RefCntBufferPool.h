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
