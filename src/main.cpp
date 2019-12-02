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
    auto image = decode("../in.png", cols, rows);

    std::vector<unsigned char> red_filtered(image);
    for_each_pixel(red_filtered, rows, cols,
                   [](pixel_ref p, size_t, size_t) {
                       std::get<1>(p) = 0;
                       std::get<2>(p) = 0;
                   });

    std::vector<unsigned char> green_filtered(image);
    for_each_pixel(green_filtered, rows, cols,
                   [](pixel_ref p, size_t, size_t) {
                       std::get<0>(p) = 0;
                       std::get<2>(p) = 0;
                   });

    std::vector<unsigned char> blue_filtered(image);
    for_each_pixel(blue_filtered, rows, cols,
                   [](pixel_ref p, size_t, size_t) {
                       std::get<0>(p) = 0;
                       std::get<1>(p) = 0;
                   });


    std::thread write_red_filtered(encode, "../red_filter.png",
                                   std::ref(red_filtered), cols, rows);
    std::thread write_green_filtered(encode, "../green_filter.png",
                                     std::ref(green_filtered), cols, rows);
    std::thread write_blue_filtered(encode, "../blue_filter.png",
                                    std::ref(blue_filtered), cols, rows);

    write_red_filtered.join();
    write_green_filtered.join();
    write_blue_filtered.join();

    std::map<long long, std::map<long long, pixel>> image_rotation;
    for_each_pixel(
        image, rows, cols,
        [&](pixel_ref p, size_t row, size_t col)
        {
            long long new_col = std::floor(double(col) / std::sqrt(2.0)
                                           - double(row) / std::sqrt(2.0));
            long long new_row = std::floor(double(col) / std::sqrt(2.0)
                                           + double(row) / std::sqrt(2.0));

            image_rotation[new_row][new_col] = {std::get<0>(p), std::get<1>(p),
                                                std::get<2>(p), std::get<3>(p)};
        }
    );

    long long rotated_rows_min = image_rotation.begin()->first;
    long long rotated_rows_max = image_rotation.rend()->first + 1;
    long long rotated_cols_min =
        min_element(image_rotation.begin(),
                    image_rotation.end(),
                    [](std::pair<long long, std::map<long long, pixel>> const& l,
                       std::pair<long long, std::map<long long, pixel>> const& r) {
                           return l.second.begin()->first <
                                  r.second.begin()->first;
                    })->second.begin()->first;
    long long rotated_cols_max =
        max_element(image_rotation.begin(), image_rotation.end(),
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
