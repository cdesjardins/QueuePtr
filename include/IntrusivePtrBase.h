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
#ifndef INTRUSIVE_PTR_BASE_Hxx
#define INTRUSIVE_PTR_BASE_Hxx

#include <atomic>

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
    mutable std::atomic_int _refCount;
};

#endif
