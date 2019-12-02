#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "coordinate_generator.hpp"
#include "lodepng.h"
#include "util.hpp"

std::vector<unsigned char> decode(std::string_view filename,
                                  unsigned int& width, unsigned int& height)
{
    std::vector<unsigned char> result;
    unsigned error = lodepng::decode(result, width, height, filename.data());

    if (error)
    {
        std::cerr << "decoder error " << error << ": "
                  << lodepng_error_text(error) << std::endl;
    }

    return result;
}

void encode(std::string_view filename, std::vector<unsigned char>& image,
            unsigned width, unsigned height)
{
    unsigned error = lodepng::encode(filename.data(), image, width, height);

    if (error)
    {
        std::cout << "encoder error " << error << ": "
                  << lodepng_error_text(error) << std::endl;
    }
}

void filter_positions(std::vector<unsigned char>& image,
                      size_t rows, size_t cols,
                      std::initializer_list<size_t> const& positions_to_filter)
{
    size_t n_pixels = rows * cols;
    size_t number_threads = std::thread::hardware_concurrency();
    size_t elements_per_thread = (size_t) std::ceil(double(n_pixels)
                                                    / number_threads);

    size_t cur_start = 0;

    std::vector<std::thread> threads;

    while (cur_start < n_pixels)
    {
        size_t cur_end = cur_start + elements_per_thread;
        if (cur_end >= n_pixels)
        {
            cur_end = n_pixels;
        }

        threads.emplace_back([&](size_t start, size_t end) {
            auto e = coordinate_generator(end, cols);
            for (auto it = coordinate_generator(start, cols); it != e; ++it)
            {
                size_t i;
                size_t j;
                std::tie(i, j) = *it;

                for (auto p : positions_to_filter)
                {
                    image[i * cols * 4 + j * 4 + p] = 0;
                }
            }
        },
        cur_start, cur_end);

        cur_start = cur_end;
    }

    for (auto& t : threads)
    {
        t.join();
    }
}
