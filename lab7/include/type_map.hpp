/* Map of types.
 * @file
 * @date 2018-08-25
 * @author Anonymous
 */

#ifndef __TYPE_MAP_HPP__
#define __TYPE_MAP_HPP__
//Ассоциативный контейнер — это структура данных, которая хранит пары «ключ — значение» и позволяет
//  быстро находить значение по ключу. type_map — это ассоциативный контейнер времени компиляции, где ключом является тип (int), 
// а значением — объект фиксированного типа (int), задаваемый во время выполнения.
#include <array>
#include <tuple>
//Пространства имён позволяют избежать конфликтов имён в крупных проектах.Объекты, объявленные внутри блока пространства
// имён, помещаются в область пространства имён, что предотвращает их путаницу с объектами с такими же 
// именами в других областях. обьявление
namespace detail
{
    template<class T, class Tuple>
    struct type_to_index;
    //искомый тип T совпадает с первым типом в кортеже
    template<class T, class... Ts> //объявление пакета типов, а если Ts... — раскрытие пакета типов
    struct type_to_index<T, std::tuple<T, Ts...>> //кортеж, где первый элемент точно T
    {
        constexpr static size_t v = 0;
    };
    // первый тип в кортеже U НЕ совпадает с искомым типом T
    template<class T, class U, class... Ts>
    struct type_to_index<T, std::tuple<U, Ts...>>//сам себя вызывается для распаковки кортеджа
    {
        constexpr static size_t v = 1 + type_to_index<T, std::tuple<Ts...>>::v;  //берем значение статической переменной v из специализации структуры 
        // type_to_index с параметрами <T, types>. :: оператор разрешения области видимости
    };
}

//{
template<class A, class ...Args> //пакет из нуля или более типов
class type_map : public std::array<A, sizeof...(Args)> //type_map наследуется от std::array<A, N>, 
// N = sizeof...(Args) — количество типов в пакете Args
{
public:
    using types = std::tuple<Args...>; //создаёт публичный псевдоним types для кортежа из всех типов-ключей.(нужно знать все типы-ключи)
    // возвращяет значение 
    template<class T>
    //возвращает ссылку на значение типа A и позволяет изменять значение
    A& as()
    {
        //this - Указатель на текущий объект type_map, type_map* (неконстантный указатель)
        //const_cast<const type_map*>(this) - снимает/добавляет константность и превращает type_map* в const type_map*
        // ->as<T>() - вызывает константную версию метода as<T>() и возвращает const A& (константную ссылку)
        //внешний const_cast<A&>() - принимает const A&, убирает константность и превращает  A& (неконстантную ссылку)
        return const_cast<A&>(const_cast<const type_map*>(this)->as<T>());
    }
    template<class T>
    //Возвращаемый тип: константная ссылка на A, не изменяет обект
    const A& as() const
    {
        //type_to_index<T, types> - принимает тип T (что ищем) и кортеж типов types и возвращ индекс типа T в кортеже types
        //detail:: — пространство имён с вспомогательными функциями
        //(*this — разыменовываем указатель this, получаем сам объект, а [...] — оператор индексации.) обратимся  к элем массива по индексу
        return (*this)[detail::type_to_index<T, types>::v];
    }
};
//}
//открытие пространства имён std
namespace std
{
    //{
    template<typename T, class TypeMap> //TypeMap — тип контейнера
    auto& get(TypeMap& tm) ///auto& — ссылка, get — перегрузка стандартной std::get, TypeMap& tm — ссылка на объект type_map
    {
        return tm.template as<T>();
        //.template — специальный синтаксис для вызова шаблонных методов
        // вызывает метод as<T>() у объекта tm
        // возвращает ссылку на значение типа T
    }
    //}
}

#endif // __TYPE_MAP_HPP__
