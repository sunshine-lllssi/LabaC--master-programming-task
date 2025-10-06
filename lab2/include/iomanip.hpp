/* Custom manipulators.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__

#include <iostream>
#include <string>
#include <cmath>    
#include <type_traits>

//{ endm manipulator
struct endm_t {};
inline endm_t endm;

inline std::ostream& operator<<(std::ostream& os, endm_t) {
    return os << "[eol]\n";
}
//}

//{ sin manipulator
struct sin_manip {};

template<typename T>
struct sin_expression {
    const T& value;
    sin_expression(const T& v) : value(v) {}
};


template<typename T>
sin_expression<T> operator<<(std::ostream& os, const sin_manip&) {

    return sin_expression<T>{T{}}; // Заглушка - значение будет заменено
}




struct sin_holder {
    std::ostream* os;
};

// stream << sin_manipulator → sin_holder
inline sin_holder operator<<(std::ostream& os, const sin_manip&) {
    return sin_holder{&os};
}

// sin_holder << value → вычисление и вывод синуса
template<typename T>
std::ostream& operator<<(sin_holder holder, const T& value) {
    if constexpr (std::is_arithmetic_v<T>) {
        double result = std::sin(static_cast<double>(value));
        if (result == -0.0) result = 0.0;
        *(holder.os) << result;
    } else {
        *(holder.os) << "sin(" << value << ")";
    }
    return *(holder.os);
}

inline sin_manip sin_manipulator;
//}

//{ add manipulator
struct add_manip {};
inline add_manip add;

struct add_add {
    std::ostream* os;
    explicit add_add(std::ostream& stream) : os(&stream) {}
};

// stream << add → add_add
inline add_add operator<<(std::ostream& os, add_manip) {
    return add_add{os};
}

// хранит поток и первый операнд
template<typename T>
struct add_first {
    std::ostream* os;
    T first;
    add_first(std::ostream* o, T f) : os(o), first(std::move(f)) {}
};

// add_add << first → add_first
template<typename T>
add_first<T> operator<<(add_add proxy, const T& first) {
    return add_first<T>{proxy.os, first};
}

// add_first << second → вывод результата
template<typename T, typename U>
std::ostream& operator<<(add_first<T>&& holder, const U& second) {
    if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
        using Common = std::common_type_t<T, U>;
        *(holder.os) << static_cast<Common>(holder.first) + static_cast<Common>(second);
    } else {
        *(holder.os) << holder.first << second;
    }
    return *(holder.os);
}
//}

#endif // __IOMANIP_HPP__