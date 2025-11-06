/* Proxy object.
 * @file
 * @date 2018-08-07
 * @author Anonymous
 */

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <mutex> //специальный «замок» для многопоточных программ, который не даёт нескольким потокам 
// одновременно лезть в одни и те же данные

template<class T, class MutexInjection = std::mutex>
class ptr_holder
{
public:
    ptr_holder(T* ptr): ptr_(ptr) {}

    //{ describe proxy object - обертка другого обьекта(или это посредник, который выглядит как настоящий объект, но может делать дополнительные фишки)
    class proxy: private std::lock_guard<MutexInjection> //управление покидает область, в которой был создан объект lock_guard, lock_guard уничтожается, а мьютекс освобождается.
    {
    public:
        // Конструктор прокси -функция, которая создаёт временный объект-обёртку (proxy), автоматически захватывая мьютекс 
        //и сохраняя указатель на реальный объект
        //ссыла на мьютекс
        // Вызывает конструктор класса lock_guard, который блокирует мьютекс
        // Сохраняет указатель для последующего доступа
        proxy(T* ptr, MutexInjection& mtx): 
            std::lock_guard<MutexInjection>(mtx),
            ptr_(ptr) 
             
        {}
        // перегрузка оператора для прокси-объекта, возвращающий сырой указатель, который не управляет временем жизни обьекта
        T* operator ->() const
        {
            return ptr_;
        }
    private:
        T* ptr_;
    };
    // перегрузка для ptr_holder, возвращающий наш прокси-объект
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
