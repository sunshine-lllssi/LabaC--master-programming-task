/* Test file for strided_range task.
 * @file
 * @date 2019-08-29
 * @author Anonymous
 */

#include <vector>

#include <boost/range/iterator_range.hpp>
#include <boost/range/irange.hpp>

#include <catch2/catch_all.hpp>
//диапазон, содержащий выбранные элементы по заданному 
#include <strided_range.hpp>

//{
TEST_CASE("strided_range::stride_bigger_width")
{
    auto a = boost::irange(0, 8);
    CHECK(make_strided_range(a, 3, 4) == std::vector{0, 1, 2, 4, 5, 6}); // берем 3 пропускаем 1 шаг 4
}
//}

//{
TEST_CASE("strided_range::width_bigger_stride")
{
    auto a = {0, 1, 5, 10, 11, 12, 53};
    CHECK(make_strided_range(a, 3, 2) == std::vector{0, 1, 5, 10, 11, 12, 53});
}
//}

//{
TEST_CASE("strided_range::stride_equal_width")
{
    auto a = boost::irange(0, 8);
    CHECK(make_strided_range(a, 2, 2) == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7});

}
//}

//{ Use boost::size.
TEST_CASE("strided_range::distance")
{
    auto a = boost::irange(0, 8);
    CHECK(boost::size(make_strided_range(a, 3, 4)) == 6); //проверка размера результата
//012456
}
//}

//{ Make strided_range from another strided_range.
TEST_CASE("strided_range::from_strided")
{
    auto a = {0, 1, 2, 3, 4, 5, 6, 7};
    auto rng = make_strided_range(a, 3, 3 + 1);//012456
    //второе применение make_strided_range к результату первого
    CHECK(make_strided_range(rng, 3 - 1, 3) == std::vector{0, 1, 4, 5});
}
//}

//{ Remove zeros from range.
TEST_CASE("strided_range::window")
{
    std::vector<int> a{0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0,
                  0, 1, 2, 3, 4, 0,
                  0, 5, 6, 7, 8, 0,
                  0, 9, 10, 11, 12, 0,
                  0, 13, 14, 15, 16, 0,
                  0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0};
    //диапазон от 13-го элемента до 13-го с конца (-нули)
    auto cropped_range = boost::make_iterator_range(a, 13, -13); 
    // [1,2,3,4,0, 0,5,6,7,8,0, 0,9,10,11,12,0, 0,13,14,15,16]
    auto window = make_strided_range(cropped_range, 4, 6);
    // должны быть числа от 1 до 16 включительн
    CHECK(window == boost::irange(1, 17));
    //[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
}
//}

//{
TEST_CASE("strided_range::corner_case1::zero_width")
{
    auto a = boost::irange(0, 10);
    // ожидаем пустой ветор, т.к 0 - не брать ни одного элемента в каждом блоке
    CHECK(make_strided_range(a, 0, 1) == std::vector<int>{}); // пустой диапазон
}
//}

//{
TEST_CASE("strided_range::corner_case2::zero_stride")
{
    auto a = boost::irange(0, 10);
    //обработка ошибки, в осн см 
    CHECK_THROWS_AS(make_strided_range(a, 1, 0), std::invalid_argument);
}
//}
