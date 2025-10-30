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
    ptr_holder(T* ptr): ptr_(ptr) {}

    //{ describe proxy object - обертка другого обьекта(или это посредник, который выглядит как настоящий объект, но может делать дополнительные фишки)
    class proxy: private std::lock_guard<MutexInjection>
    {
    public:
        // Конструктор прокси:
        // 1. Вызывает конструктор базового класса std::lock_guard, который блокирует мьютекс.
        // 2. Сохраняет указатель для последующего доступа.
        proxy(T* ptr, MutexInjection& mtx): 
            std::lock_gruard<MutexInjection>(mtx),
            prt_(ptr) 
             
        {}
        // operator-> для прокси-объекта, возвращающий сырой указатель.
        // Это завершает "рекурсивную" цепочку вызовов.
        T* operator ->() const
        {
            return prt_;
        }
    private:
        T* prt_;
    };
    // operator-> для ptr_holder, возвращающий наш прокси-объект.
    // Вызов этого оператора создает временный прокси-объект, который
    // живет до конца полного выражения (до точки с запятой).
    proxy operator -> () const
    {
        return proxy(ptr_, mutex_);
    }
    //}

private:
    T* ptr_;
    mutable MutexInjection mutex_;
};

#endif // __PROXY_HPP__
