#include <utility>

#include "coordinate_generator.hpp"

coordinate_generator::coordinate_generator(size_t x, size_t y, size_t cols)
    : m_x(x), m_y(y), m_cols(cols)
{
}

coordinate_generator::coordinate_generator(coordinate_generator const& other)
    : m_x(other.m_x), m_y(other.m_y), m_cols(other.m_cols);
{
}

bool coordinate_generator::operator==(coordinate_generator const& other)
{
    return this->m_x == other.m_x &&
           this->m_y == other.m_y &&
           this->m_cols == other.m_cols;
}

bool coordinate_generator::operator!=(coordinate_generator const& other)
{
    return !(*this == other);
}

coordinate_generator::value_type coordinate_generator::operator*()
{
    return std::make_pair(m_x, m_y);
}

coordinate_generator::value_type coordinate_generator::operator++()
{
    if (m_y + 1 < m_cols)
    {
        m_y++;
    }
    else
    {
        m_x++;
        m_y = 0;
    }
}
