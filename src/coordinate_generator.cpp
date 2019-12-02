#include <utility>

#include "coordinate_generator.hpp"

coordinate_generator::coordinate_generator(size_t row, size_t col, size_t cols)
    : m_row(row), m_col(col), m_cols(cols)
{
}

coordinate_generator::coordinate_generator(size_t linearized, size_t cols)
    : m_cols(cols)
{
    m_col = linearized % m_cols;
    m_row = linearized / m_cols;
}

coordinate_generator::coordinate_generator(coordinate_generator const& other)
    : m_row(other.m_row), m_col(other.m_col), m_cols(other.m_cols)
{
}

bool coordinate_generator::operator==(coordinate_generator const& other)
{
    return this->m_row == other.m_row &&
           this->m_col == other.m_col &&
           this->m_cols == other.m_cols;
}

bool coordinate_generator::operator!=(coordinate_generator const& other)
{
    return !(*this == other);
}

coordinate_generator::value_type coordinate_generator::operator*()
{
    return std::make_pair(m_row, m_col);
}

coordinate_generator& coordinate_generator::operator++()
{
    if (m_col + 1 < m_cols)
    {
        m_col++;
    }
    else
    {
        m_row++;
        m_col = 0;
    }

    return *this;
}
