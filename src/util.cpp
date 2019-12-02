#include <iostream>
#include <string>
#include <vector>

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
