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
#ifndef REF_CNT_BUFFER_Hxx
#define REF_CNT_BUFFER_Hxx

#include "IntrusivePtrBase.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio/buffer.hpp>

class RefCntBufferPool_;

class RefCntBuffer : public IntrusivePtrBase
{
public:
    RefCntBuffer(std::shared_ptr<RefCntBufferPool_> pool, const int bufferSize)
        : _pool(pool),
        _dead(false),
        _bufferSize(bufferSize)
    {
        _backingData = new char[_bufferSize + 1];
        resetBackingData();
    }

    RefCntBuffer(const RefCntBuffer& rhs)
        : IntrusivePtrBase(rhs),
        _pool(rhs._pool),
        _dead(false),
        _bufferSize(rhs._bufferSize)
    {
    }

    virtual ~RefCntBuffer()
    {
        delete []_backingData;
    }

    boost::asio::mutable_buffer _buffer;

protected:
    friend class RefCntBufferPool_;
    void resetBackingData()
    {
        _buffer = boost::asio::buffer(_backingData, _bufferSize);
    }

    void dead()
    {
        _dead = true;
    }

    virtual void finalRelease(IntrusivePtrBase* s) const;

private:
    std::shared_ptr<RefCntBufferPool_> _pool;
    bool _dead;
    char* _backingData;
    const int _bufferSize;
    const RefCntBuffer operator=(const RefCntBuffer& rhs);
};

#endif
