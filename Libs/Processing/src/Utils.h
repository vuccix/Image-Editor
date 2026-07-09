#pragma once

#include <cstdint>
#include <vector>
#include <mdspan>

class Layer;

namespace Utils {

    template <typename T, typename U>
    std::vector<T> convolution(std::mdspan<const T, std::dextents<size_t, 2>> image,
                               std::mdspan<const U, std::dextents<size_t, 2>> kernel);

}
