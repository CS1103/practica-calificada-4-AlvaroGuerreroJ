#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <functional>
#include <string>
#include <vector>

using pixel = std::tuple<unsigned char, unsigned char, unsigned char,
                         unsigned char>;
using pixel_ref = std::tuple<unsigned char&, unsigned char&, unsigned char&,
                             unsigned char&>;

std::vector<unsigned char> decode(std::string_view filename,
                                  unsigned int& width, unsigned int& height);
void encode(std::string_view filename, std::vector<unsigned char>& image,
            unsigned width, unsigned height);

void for_each_pixel(std::vector<unsigned char>& image,
                    size_t rows, size_t cols,
                    std::function<void(pixel_ref, size_t row, size_t col)> fn);

#endif  // __UTIL_HPP_
