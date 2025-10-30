/* Custom manipulators.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */
//обьекты, которые изменяют поведение потоков ввода/ввывода 
#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__

#include <iostream>
#include <string>
#include <cmath>    
#include <type_traits>

//{ endm manipulator
struct endm_t {}; //пустая структура для манипулятора
inline endm_t endm; //создает глобальный объект манипулятора

inline std::ostream& operator<<(std::ostream& os, endm_t) { //перегружает оператор <<
    return os << "[eol]\n";
}
//}

//{ sin manipulator
struct sin_manip {};

template<typename T> //шаблонная структура sin_expression с T
struct sin_expression {
    const T& value; //ссылка на значение для которого вычисляется sin
    sin_expression(const T& v) : value(v) {} //конструктор который активирует ссылку value передпнным значением v
};


template<typename T> // перегружает оператор(обьединение) << для вывода sin_manip в поток
sin_expression<T> operator<<(std::ostream& os, const sin_manip&) { // возвращает временный объект sin_expression с "заглушечным" значением типа T

    return sin_expression<T>{T{}}; // заглушка - значение будет заменено
}


struct sin_holder { //объявляет структуру, которая хранит указатель на поток вывода
    std::ostream* os;
};

// stream << sin_manipulator → sin_holder
inline sin_holder operator<<(std::ostream& os, const sin_manip&) {
    return sin_holder{&os};
}

// sin_holder << value → вычисление и вывод синуса
template<typename T>
std::ostream& operator<<(sin_holder holder, const T& value) {
    if constexpr (std::is_arithmetic_v<T>) { // T- числовой тип?
        double result = std::sin(static_cast<double>(value)); //вычисляет значения, преобразованного к double
        if (result == -0.0) result = 0.0; //исправляет отрицательный ноль на обычный
        *(holder.os) << result; //вывод результата
    } else {
        *(holder.os) << "sin(" << value << ")";
    }
    return *(holder.os); //вернет поток, чтобы продолжить операции
}

inline sin_manip sin_manipulator;
//}

//{ add manipulator //структура для хранения указателя (хранит адрес другой переменной) на потоке вывода
struct add_manip {};
inline add_manip add;

struct add_add { 
    std::ostream* os;
    explicit add_add(std::ostream& stream) : os(&stream) {} //конструктор, принимающий ссылку на поток и сохраняющий его адрес
};

// stream << поток + add → add_add с указателем на поток
inline add_add operator<<(std::ostream& os, add_manip) {
    return add_add{os};
}

// хранит поток и первый операнд сложения
template<typename T>
struct add_first {
    std::ostream* os;
    T first;
    add_first(std::ostream* o, T f) : os(o), first(std::move(f)) {} //конструктор, принимающий указатель на поток и первый операнд
};

// add_add + первый операнд << first → add_first с сохраненным операндом
template<typename T>
add_first<T> operator<<(add_add proxy, const T& first) {
    return add_first<T>{proxy.os, first}; // тип
}

// add_first + второй операнд << second → вывод результата
template<typename T, typename U>
std::ostream& operator<<(add_first<T>&& holder, const U& second) {
    if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) { // если оба операнда числовые - выполняет арифметическое сложение
        using Common = std::common_type_t<T, U>; //определяет общий тип операндов
        *(holder.os) << static_cast<Common>(holder.first) + static_cast<Common>(second); //сложение и вывод
    } else {
        *(holder.os) << holder.first << second; // для не числовых типов выполняется склеивание

    }
    return *(holder.os); //возвращает поток
}
//}

#endif // __IOMANIP_HPP__