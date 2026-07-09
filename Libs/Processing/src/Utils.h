#pragma once

#include <cstdint>
#include <mdspan>

class Layer;

namespace Utils {

    void convolution(Layer& image, std::mdspan<const int32_t, std::dextents<size_t, 2>> kernel);

}
