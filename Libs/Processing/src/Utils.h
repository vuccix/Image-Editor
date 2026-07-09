#pragma once

#include <cstdint>
#include <mdspan>

class Layer;

namespace Utils {

    template <typename T>
    void convolution(Layer& image, std::mdspan<const T, std::dextents<size_t, 2>> kernel);

}
