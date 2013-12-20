#ifndef REF_CNT_BUFFER_Hxx
#define REF_CNT_BUFFER_Hxx

#include "IntrusivePtrBase.h"
#include <boost/smart_ptr/shared_ptr.hpp>
class RefCntBufferPool;

class RefCntBuffer : public IntrusivePtrBase
{
public:
    RefCntBuffer(boost::shared_ptr<RefCntBufferPool> pool) 
        : _pool(pool)
    {
    }

    RefCntBuffer(const RefCntBuffer& rhs) 
        : _pool(rhs._pool)
    {
    }

    const RefCntBuffer operator=(const RefCntBuffer& rhs)
    {
        if(this != &rhs)
        {
            _pool = rhs._pool;
        }
        return *this;
    }
protected:
    virtual void finalRelease(IntrusivePtrBase* s) const;

private:
    boost::shared_ptr<RefCntBufferPool> _pool;
};

#endif