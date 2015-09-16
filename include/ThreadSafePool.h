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
#ifndef THREADSAFEPOOL_H
#define THREADSAFEPOOL_H

#include "threadsafequeue.h"
#include "boost/smart_ptr/intrusive_ptr.hpp"

template <class T> class ThreadSafePool : public ThreadSafeQueue<boost::intrusive_ptr<T> >
{
public:

protected:
    ThreadSafePool()
        : _numBufs(0)
    {
    }

    void addToPool(int num, const int bufferSize)
    {
        for (int i = 0; i < num; i++)
        {
            ThreadSafeQueue<boost::intrusive_ptr<T> >::enqueue(allocateBuffer(bufferSize));
        }
        _numBufs += num;
    }

    virtual boost::intrusive_ptr<T> allocateBuffer(const int bufferSize) = 0;
    int _numBufs;
};

#endif // THREADSAFEPOOL_H

