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

    std::vector<unsigned char> red_filtered(image);
    filter_positions(red_filtered, rows, cols, {1, 2});

    std::vector<unsigned char> green_filtered(image);
    filter_positions(green_filtered, rows, cols, {0, 2});

    std::vector<unsigned char> blue_filtered(image);
    filter_positions(blue_filtered, rows, cols, {0, 1});


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
