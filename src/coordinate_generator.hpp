#ifndef __COORDINATE_GENERATOR_HPP_
#define __COORDINATE_GENERATOR_HPP_

#include <iterator>
#include <utility>

class coordinate_generator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<size_t, size_t>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::pair<size_t, size_t>*;
    using reference = std::pair<size_t, size_t>&;

private:
    size_t m_x;
    size_t m_y;
    size_t m_cols;

public:
    coordinate_generator(size_t x, size_t y, size_t cols);
    coordinate_generator(coordinate_generator const& other);

    bool operator==(coordinate_generator const& other);
    bool operator!=(coordinate_generator const& other);

    value_type operator*();
    value_type operator++();
};

#endif  // __UTIL_HPP_
