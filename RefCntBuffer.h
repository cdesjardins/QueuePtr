#ifndef REF_CNT_BUFFER_Hxx
#define REF_CNT_BUFFER_Hxx

#include "IntrusivePtrBase.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio/buffer.hpp>

class RefCntBufferPool_;

class RefCntBuffer : public IntrusivePtrBase
{
public:
    RefCntBuffer(boost::shared_ptr<RefCntBufferPool_> pool, const int bufferSize)
        : _pool(pool),
        _dead(false),
        _bufferSize(bufferSize)
    {
        _backingData = new char[_bufferSize + 1];
        resetBackingData();
    }

    RefCntBuffer(const RefCntBuffer& rhs)
        : _pool(rhs._pool),
        _dead(false),
        _bufferSize(rhs._bufferSize)
    {
    }

    ~RefCntBuffer()
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
    boost::shared_ptr<RefCntBufferPool_> _pool;
    bool _dead;
    char* _backingData;
    const int _bufferSize;
    const RefCntBuffer operator=(const RefCntBuffer& rhs);
};

#endif
