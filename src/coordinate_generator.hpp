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
    size_t m_row;
    size_t m_col;
    size_t m_cols;

public:
    coordinate_generator(size_t row, size_t col, size_t cols);
    coordinate_generator(size_t linearized, size_t cols);
    coordinate_generator(coordinate_generator const& other);

    bool operator==(coordinate_generator const& other);
    bool operator!=(coordinate_generator const& other);

    value_type operator*();
    coordinate_generator& operator++();
};

#endif  // __UTIL_HPP_
