/* Test file for strided_range task.
 * @file
 * @date 2019-08-29
 * @author Anonymous
 */

#include <vector>

#include <boost/range/iterator_range.hpp>
#include <boost/range/irange.hpp>

#include <catch2/catch_all.hpp>

#include <strided_range.hpp>

//{
TEST_CASE("strided_range::stride_bigger_width")
{
    auto a = boost::irange(0, 8);
    CHECK(make_strided_range(a, 3, 4) == std::vector{0, 1, 2, 4, 5, 6});
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
    CHECK(boost::size(make_strided_range(a, 3, 4)) == 6);
}
//}

//{ Make strided_range from another strided_range.
TEST_CASE("strided_range::from_strided")
{
    auto a = {0, 1, 2, 3, 4, 5, 6, 7};
    auto rng = make_strided_range(a, 3, 3 + 1);
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

    auto cropped_range = boost::make_iterator_range(a, 13, -13);
    auto window = make_strided_range(cropped_range, 4, 6);
    CHECK(window == boost::irange(1, 17));
}
//}

//{
TEST_CASE("strided_range::corner_case1::zero_width")
{
    auto a = boost::irange(0, 10);
    CHECK(make_strided_range(a, 0, 1) == std::vector<int>{});
}
//}

//{
TEST_CASE("strided_range::corner_case2::zero_stride")
{
    auto a = boost::irange(0, 10);
    CHECK_THROWS_AS(make_strided_range(a, 1, 0), std::invalid_argument);
}
//}
