/* Image iterator.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <cmath>
#include <iterator>
#include <type_traits>
#include <utility>

#include <boost/iterator/iterator_adaptor.hpp>

//{ image iterator
template<class Iterator>
class image_iterator: public boost::iterator_adaptor<image_iterator<Iterator>,
    Iterator
>
{
public:
    image_iterator(Iterator it, size_t width, size_t stride) 
    : boost::iterator_adaptor<image_iterator<Iterator>, Iterator>(it), width(width), stride(stride), index(0)
    {}

private:
    friend class boost::iterator_core_access;
    size_t width;
    size_t stride;
    int index;

    void increment()
    {
        advance(1);
    }

    void decrement()
    {
        advance(-1);
    }

    void advance(typename image_iterator::difference_type n)
    {
        int row = floor(double(index + n) / width);
        this->base_reference() += row * (stride - width) + n;
        index += n - row * width;
    }

    typename image_iterator::difference_type distance_to(const image_iterator& i) const
    {
        return ((i.base_reference() - i.index) - (this->base_reference() - index)) / static_cast<ptrdiff_t>(stride) * width + (i.index - index);

    }
};
//}

#endif // __ITERATOR_HPP__
