/* Strided range.
 * @file
 * @date 2021-12-17
 * @author Anonymous
 */

#ifndef __STRIDED_RANGE_HPP__
#define __STRIDED_RANGE_HPP__

#include <stdexcept>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>

template<class Range> // работает с любым типом диапазона (width - сколько последовательных элементов брать, stride - общий шаг (период) паттерна)
auto make_strided_range(Range& rng, size_t width, size_t stride)
{
    if (stride == 0)
        throw std::invalid_argument("stride != 0");

    using namespace boost::adaptors; //позволяет объектам с несовместимыми интерфейсами работать вместе
    //определяет, попадает ли элемент в "окно"
    const auto strided = [width, stride](const auto& x) { return x.index() % stride < width; };
    //лямбда-функция для преобразования: извлекает значение из indexed элемента (x - объект с индексом и значением)
    const auto valued = [](const auto& x) { return x.value(); };
    // доб индексы к элементам, фильтруем по страйоковому паттерну, извлекаем только значения (без индексов)
    return rng | indexed() | filtered(strided) | transformed(valued);
}

#endif // __STRIDED_RANGE_HPP__

// суть паттерна в адапторе: Адаптер преобразует интерфейс одного класса в интерфейс, ожидаемый клиентом, выступая 
// как "переходник" между несовместимыми компонентами.
//Писать один код для разных типов, Избежать дублирования кода
//страйдированный паттерн(шаговый)  работает как
// [ВЗЯТЬ width элементов] -> [ПРОПУСТИТЬ (stride - width) элементов] -> повторить
//Страйд - это полный цикл или период паттерна выборки.
//Паттерн - это правило выбора элементов из последовательности