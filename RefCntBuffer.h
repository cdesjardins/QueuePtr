#ifndef REF_CNT_BUFFER_Hxx
#define REF_CNT_BUFFER_Hxx

#include "IntrusivePtrBase.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio/buffer.hpp>
class RefCntBufferPool_;

class RefCntBuffer : public IntrusivePtrBase
{
public:
    RefCntBuffer(boost::shared_ptr<RefCntBufferPool_> pool) 
        : _pool(pool),
        _dead(false)
    {
    }

    RefCntBuffer(const RefCntBuffer& rhs) 
        : _pool(rhs._pool),
        _dead(false)
    {
    }

    const RefCntBuffer operator=(const RefCntBuffer& rhs)
    {
        if(this != &rhs)
        {
            _pool = rhs._pool;
            _dead = false;
        }
        return *this;
    }
    ~RefCntBuffer()
    {
        std::cout << "RefCntBuffer gone" << std::endl;
    }

    boost::asio::mutable_buffer _buffer;

protected:
    friend class RefCntBufferPool_;
    void dead()
    {
        _dead = true;
        std::cout << "dead " << this << std::endl;
    }

    virtual void finalRelease(IntrusivePtrBase* s) const;

private:
    boost::shared_ptr<RefCntBufferPool_> _pool;
    bool _dead;
};

#endif
