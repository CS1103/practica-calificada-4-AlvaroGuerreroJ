#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <string_view>
#include <thread>
#include <vector>

#include "coordinate_generator.hpp"
#include "lodepng.h"
#include "util.hpp"

int main()
{
    unsigned int cols;
    unsigned int rows;
    auto const image = decode("../in.png", cols, rows);

    //  Un comment if you want to check buffer content
    // for (size_t i = 0; i < rows; i++)
    // {
    //     for (size_t j = 0; j < cols * 4; j += 4)
    //     {
    //         int r = image[i * cols * 4 + j + 0];  // Red component
    //         int g = image[i * cols * 4 + j + 1];  // Green component
    //         int b = image[i * cols * 4 + j + 2];  // Blue component
    //         int a = image[i * cols * 4 + j + 3];  // Alpha component
    //         std::cout << r << " ";
    //         std::cout << g << " ";
    //         std::cout << b << " ";
    //         std::cout << a << "|";
    //     }
    //     std::cout << std::endl;
    // }

    size_t number_threads = std::thread::hardware_concurrency();
    size_t n_pixels = rows * cols;
    size_t elements_per_thread = (size_t) std::ceil(double(n_pixels)
                                                    / number_threads);

    std::vector<unsigned char> red_filtered(image);

    {
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

                    red_filtered[i * cols * 4 + j * 4 + 1] = 0;
                    red_filtered[i * cols * 4 + j * 4 + 2] = 0;
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

    std::vector<unsigned char> green_filtered(image);

    {
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

                    green_filtered[i * cols * 4 + j * 4 + 0] = 0;
                    green_filtered[i * cols * 4 + j * 4 + 2] = 0;
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

    std::vector<unsigned char> blue_filtered(image);

    {
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

                    blue_filtered[i * cols * 4 + j * 4 + 0] = 0;
                    blue_filtered[i * cols * 4 + j * 4 + 1] = 0;
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

    std::thread write_red_filtered(encode, "../red_filter.png",
                                   std::ref(red_filtered), cols, rows);
    std::thread write_green_filtered(encode, "../green_filter.png",
                                     std::ref(green_filtered), cols, rows);
    std::thread write_blue_filtered(encode, "../blue_filter.png",
                                    std::ref(blue_filtered), cols, rows);

    write_red_filtered.join();
    write_green_filtered.join();
    write_blue_filtered.join();

    return 0;
}
