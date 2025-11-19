/* Variant classes.
 * @file
 * @date 2018-08-12
 * @author Anonymous
 */

#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>
#include <string>
#include <type_traits>

#include <boost/variant.hpp>

//{ number (вариант для чисел)
using number = std::variant<int, float>; //представляет собой типобезопасный союз
//Экземпляр variant в любой момент времени либо содержит значение одного из альтернативных типов, либо, в случае ошибки, не содержит никакого значения (этого состояния трудно достичь)
//контейнер(хранение в определенном порядке) последовательностей, который прячет/скрывает массивы динамического размера.
//}

//{ array (массив чисел (упорядоченная последовательность элементов одного типа, доступ к которым осуществляется по индексу) array - вектор элементов типа number)
using array = std::vector<number>;
//}

//{ recursive_array (Рекурсивный массив (с shared_ptr) — это структура данных, реализованная как вектор, элементы которого могут быть либо значениями (number), 
// либо указателями на другие массивы такого же типа (std::shared_ptr<recursive_array>))
struct recursive_array : public std::vector<std::variant<number, std::shared_ptr<recursive_array>>>
{
    using std::vector<std::variant<number, std::shared_ptr<recursive_array>>>::vector; //указатель, который сохраняет общее право собственности на объект через указатель
};
//}

//{ recursive_array2 (Рекурсивный массив (с Boost)— -//- элементы которого могут быть либо значениями (number), либо вложенным массивом такого же типа )
struct recursive_array2 : public std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>
{
    using std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>::vector;//разрешает циклическую зависимость, присущую рекурсивным типам
};
//}

//Массив: Простая структура фиксированного размера, хранящая элементы одного типа по индексу.
//Рекурсивный массив (с shared_ptr): Массив, элементы которого могут быть либо значениями, либо указателями на другие массивы
// такого же типа. Использует std::shared_ptr для ссылки на вложенные массивы. Вложенное содержимое не копируется при передаче указателя.
//Рекурсивный массив (с boost::recursive_wrapper): --//- Использует boost::recursive_wrapper для хранения вложенного массива внутри себя. 
// При встраивании, содержимое КОПИРУЕТСЯ.

//{ variant_decorator (обёртка вокруг std::variant, которая добавляет ему дополнительно безопасное извлечения значений нужного типа, включая обработку специальных случаев вроде boost::recursive_wrapper)
template<typename... Types>
struct variant_decorator : std::variant<Types...>
{
    using std::variant<Types...>::variant;

    // Неконстантная версия - изменения
    template<typename RetType>
    RetType& as() //возвращает ссылку на запрошенный тип
    {
        if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<RetType>, Types>...>) //вычисляется на этапе компиляции
        {
            return std::get<boost::recursive_wrapper<RetType>>(*this).get(); //извлечение по типу /.get() - ссылка на хранимый объект
        }
        else
        {
            return std::get<RetType>(*this);//извлечение
        }
    }

    // Константная версия - чтение
    template<typename RetType>
    const RetType& as() const //возвращает константную ссылку (нельзя изменять объект)
    {
        if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<RetType>, Types>...>)
        {
            return std::get<boost::recursive_wrapper<RetType>>(*this).get();
        }
        else
        {
            return std::get<RetType>(*this);
        }
    }
};
//}

//{ recursive_map
struct recursive_map : public std::map<std::string, variant_decorator<int, std::string, bool, boost::recursive_wrapper<recursive_map>>>
{
    using std::map<std::string, variant_decorator<int, std::string, bool, boost::recursive_wrapper<recursive_map>>>::map; //отсортированный ассоциативный контейнер, содержащий пары «ключ-значение» с уникальными ключами.
};
//}

#endif // __VARIANT_HPP__