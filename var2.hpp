/* Proxy object.
 * @file
 * @date 2018-08-07
 * @author Anonymous
 */

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <mutex>

template<class T, class MutexInjection = std::mutex>
class ptr_holder
{
public:
    ptr_holder(T* ptr) : ptr_(ptr) {}

    //{ describe proxy object
    class proxy : private MutexInjection
    {
    public:
        proxy(T* ptr, MutexInjection& mtx)
            : ptr_(ptr)
        {
            mtx.lock();
        }

        ~proxy()
        {
            static_cast<MutexInjection*>(this)->unlock();
        }

        T* operator->() const
        {
            return ptr_;
        }

    private:
        T* ptr_;
    };
    //}

    proxy operator->() const
    {
        return proxy{ptr_, mutex_};
    }

private:
    T* ptr_;
    mutable MutexInjection mutex_;
};

#endif // __PROXY_HPP__