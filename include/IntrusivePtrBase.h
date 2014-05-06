#ifndef INTRUSIVE_PTR_BASE_Hxx
#define INTRUSIVE_PTR_BASE_Hxx

#include <boost/detail/atomic_count.hpp>

struct IntrusivePtrBase
{
    IntrusivePtrBase(): _refCount(0) 
    {
    }
    IntrusivePtrBase(IntrusivePtrBase const&)
        : _refCount(0) 
    {
    }

    IntrusivePtrBase& operator=(IntrusivePtrBase const& )
    { 
        return *this;
    }

    friend void intrusive_ptr_add_ref(IntrusivePtrBase const* s)
    {
        ++s->_refCount;
    }

    friend void intrusive_ptr_release(IntrusivePtrBase* s)
    {
        if (--s->_refCount == 0)
        {
            s->finalRelease(s);
        }
    }

    int refcount() const 
    { 
        return _refCount;
    }

protected:
    virtual void finalRelease(IntrusivePtrBase* s) const = 0;
    
private:
    ///should be modifiable even from const intrusive_ptr objects
    mutable boost::detail::atomic_count _refCount;
};

#endif
