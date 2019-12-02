#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <string>
#include <vector>

std::vector<unsigned char> decode(std::string_view filename,
                                  unsigned int& width, unsigned int& height);
void encode(std::string_view filename, std::vector<unsigned char>& image,
            unsigned width, unsigned height);

void filter_positions(std::vector<unsigned char>& image,
                      size_t rows, size_t cols,
                      std::initializer_list<size_t> const& positions_to_filter);

#endif  // __UTIL_HPP_
