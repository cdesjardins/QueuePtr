/*
** This provides a thread safe queue
*/

#ifndef CB_THREAD_SAFE_QUEUE_Hxx
#define CB_THREAD_SAFE_QUEUE_Hxx

#include <list>
#ifndef Q_MOC_RUN
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/chrono/chrono.hpp>
#endif
/*
** On the wait functions in this class if timeout == -1, then wait forever...
** When dequeing a batch of messages, the return code is the total number of bytes
** in all messages in the batch.
*/

template <class T> class ThreadSafeQueue
{
public:
    ThreadSafeQueue()
        : _queue(),
        _queueMutex(),
        _msgNotification(),
        _numEnqueued(0)
    {
    }

    virtual ~ThreadSafeQueue()
    {
    }

    virtual void enqueue(const T &data)
    {
        { // create a new scope for the mutex
            boost::mutex::scoped_lock lock(_queueMutex);
            pushData(data);
            _msgNotification.notify_all();
        }
    }

    virtual bool dequeue(T &data)
    {
        bool ret = false;
        { // create a new scope for the mutex
            boost::mutex::scoped_lock lock(_queueMutex);
            ret = popData(data);
        }
        return ret;
    }

    virtual std::size_t dequeueBatch(std::vector<T> &dataVec)
    {
        std::size_t size = 0;
        { // create a new scope for the mutex
            boost::mutex::scoped_lock lock(_queueMutex);
            size = popDataBatch(dataVec);
        }
        return size;
    }

    virtual bool waitDequeue(T &data, const int msTimeout = -1)
    {
        bool ret = false;
        { // create a new scope for the mutex
            boost::mutex::scoped_lock lock(_queueMutex);
            waitForData(msTimeout);
            ret = popData(data);
        }
        return ret;
    }

    virtual bool waitFront(T &data, const int msTimeout = -1)
    {
        bool ret = false;
        { // create a new scope for the mutex
            boost::mutex::scoped_lock lock(_queueMutex);
            waitForData(msTimeout);
            ret = frontData(data);
        }
        return ret;
    }

    int size() const
    {
        return _numEnqueued;
    }

    /*
    ** This function allows you to perform operations on the
    ** vector in a thread safe way. The functor is a function
    ** with the following signature:
    ** int func(std::list<T> &);
    ** The return value is the number of elements added or removed
    ** from the list, for example if 5 elements were removed
    ** and 3 new elements were added then the return value should
    ** be -2.
    */
    template <typename Functor> void iterate(Functor functor)
    {
        { // create a new scope for the mutex
            boost::mutex::scoped_lock lock(_queueMutex);
            // the return value of this functor is added to the _numEnqueued
            // so if you add buffers then return the number of buffers added
            // or if you remove buffers then return -number of buffers removed
            int numChanged = functor(_queue);
            _numEnqueued += numChanged;
        }
    }

protected:

    void waitForData(const int msTimeout)
    {
        // This function assumes that _queueMutex is locked already!
        boost::chrono::system_clock::time_point timeLimit = boost::chrono::system_clock::now() + boost::chrono::milliseconds(msTimeout);
        while (_queue.empty() == true)
        {
            // if timeout is specified, then wait until the time is up
            // otherwise wait forever (forever is msTimeout = -1)
            if (msTimeout >= 0)
            {
                _msgNotification.wait_until(_queueMutex, timeLimit);
                if (boost::chrono::system_clock::now() >= timeLimit)
                {
                    break;
                }
            }
            else
            {
                _msgNotification.wait(_queueMutex);
            }
        }
    }

    void pushData(const T &data)
    {
        // This function assumes that _queueMutex is locked already!
        _queue.push_back(data);
        _numEnqueued++;
    }

    bool popData(T &data)
    {
        // This function assumes that _queueMutex is locked already!
        bool ret = false;
        if (_queue.empty() == false)
        {
            data = _queue.front();
            _queue.pop_front();
            _numEnqueued--;
            ret = true;
        }
        return ret;
    }

    std::size_t popDataBatch(std::vector<T>& dataVec)
    {
        // This function assumes that _queueMutex is locked already!
        std::size_t size = 0;
        T data;
        while (popData(data) == true)
        {
            size += sizeOfData(data);
            dataVec.push_back(data);
        }
        return size;
    }

    bool frontData(T &data)
    {
        // This function assumes that _queueMutex is locked already!
        bool ret = false;
        if (_queue.empty() == false)
        {
            data = _queue.front();
            ret = true;
        }
        return ret;
    }

    virtual std::size_t sizeOfData(const T&) const
    {
        return sizeof(T);
    }

    std::list<T> _queue;
    boost::mutex _queueMutex;
    boost::condition_variable_any _msgNotification;
    std::size_t _numEnqueued;
};

#endif
