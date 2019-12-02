#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <string_view>
#include <thread>
#include <tuple>
#include <vector>

#include "coordinate_generator.hpp"
#include "lodepng.h"
#include "util.hpp"

int main()
{
    unsigned int cols;
    unsigned int rows;
    auto const image = decode("../in.png", cols, rows);

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

    using pixel = std::tuple<unsigned char, unsigned char,
                             unsigned char, unsigned char>;
    std::map<long long, std::map<long long, pixel>> image_rotation;

    for (auto it = coordinate_generator(0, cols);
         it != coordinate_generator(cols * rows, cols);
         ++it)
    {
        size_t row;
        size_t col;
        std::tie(row, col) = *it;

        long long new_col = std::floor(double(col) / std::sqrt(2.0)
                                       - double(row) / std::sqrt(2.0));
        long long new_row = std::floor(double(col) / std::sqrt(2.0)
                                       + double(row) / std::sqrt(2.0));

        image_rotation[new_row][new_col] = {image[row * cols * 4 + col * 4],
                                            image[row * cols * 4 + col * 4 + 1],
                                            image[row * cols * 4 + col * 4 + 2],
                                            image[row * cols * 4 + col * 4 + 3]};
    }

    long long rotated_rows_min = image_rotation.begin()->first;
    long long rotated_rows_max = image_rotation.rend()->first + 1;
    long long rotated_cols_min = min_element(image_rotation.begin(),
                                             image_rotation.end(),
                                             [](std::pair<long long, std::map<long long, pixel>> const& l,
                                                 std::pair<long long, std::map<long long, pixel>> const& r) {
                                                 return l.second.begin()->first <
                                                         r.second.begin()->first;
                                             })->second.begin()->first;
    long long rotated_cols_max = max_element(image_rotation.begin(), image_rotation.end(),
                                      [](std::pair<long long, std::map<long long, pixel>> const& l,
                                         std::pair<long long, std::map<long long, pixel>> const& r) {
                                          return l.second.rend()->first <
                                                 r.second.rend()->first;
                                      })->second.rend()->first + 1;

    std::vector<unsigned char> image_rotated;
    for (long long i = rotated_rows_min; i < rotated_rows_max; i++)
    {
        for (long long j = rotated_cols_min; j < rotated_cols_max; j++)
        {
            if (image_rotation.find(i) != image_rotation.end() &&
                image_rotation[i].find(j) != image_rotation[i].end())
            {
                image_rotated.push_back(std::get<0>(image_rotation[i][j]));
                image_rotated.push_back(std::get<1>(image_rotation[i][j]));
                image_rotated.push_back(std::get<2>(image_rotation[i][j]));
                image_rotated.push_back(std::get<3>(image_rotation[i][j]));
            }
            else
            {
                image_rotated.push_back(0);
                image_rotated.push_back(0);
                image_rotated.push_back(0);
                image_rotated.push_back(0);
            }
        }
    }

    encode("../rotated.png", std::ref(image_rotated),
           rotated_cols_max - rotated_cols_min,
           rotated_rows_max - rotated_rows_min);

    return 0;
}
