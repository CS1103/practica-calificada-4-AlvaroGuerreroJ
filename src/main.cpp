#include <algorithm>
#include <iostream>
#include <vector>
#include <string_view>

#include "lodepng.h"
#include "util.hpp"

int main()
{
    unsigned int w;
    unsigned int h;
    auto image = decode("../in.png", w, h);

    //  Un comment if you want to check buffer content
    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w * 4; j += 4)
        {
            int r = image[i * w * 4 + j + 0];  // Red component
            int g = image[i * w * 4 + j + 1];  // Green component
            int b = image[i * w * 4 + j + 2];  // Blue component
            int a = image[i * w * 4 + j + 3];  // Alpha component
            std::cout << r << " ";
            std::cout << g << " ";
            std::cout << b << " ";
            std::cout << a << "|";
        }
        std::cout << std::endl;
    }

    encode("../out.png", image, w, h);
    return 0;
}
